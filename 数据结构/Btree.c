#include<stdlib.h>
#include<malloc.h>

#define SUB_M   3

typedef struct _btree_node
{
    int *keys; // int key[SUB_M * 2 -1];
    struct _btree_node **children; // struct _btree_node *children[SUB_M * 2]
    int num; // key中存的数据个数
    int leaf; // 是否是叶子结点
} btree_node;

typedef struct _btree
{
    struct _btree_node *root;
} btree;

// leaf决定新节点是否为叶子结点
btree_node* btree_node_create(int leaf) 
{   
    btree_node *node = (btree_node*)calloc(1, sizeof(btree_node));
    if (node == NULL) return NULL; // 内存不够
    
    node->leaf = leaf;
    node->keys = (int*)calloc(2 * SUB_M - 1, sizeof(int));
    node->children = (btree_node**)calloc(2 * SUB_M, sizeof(btree_node*));
    node->num = 0;
    
    return node; // 修复：缺少返回值
}

void btree_destroy_node(btree_node *node)
{
    free(node->children);
    free(node->keys);
    free(node);
}

// T：某个B树，x：需要被split的结点的父节点，idx：x的第几个孩子（0-based） 
void btree_split_child(btree *T, btree_node *x, int idx) 
{
    btree_node *y = x->children[idx]; // 声明y：需要被split的那个结点
    btree_node *z = btree_node_create(y->leaf); // 创建一个新节点，来储存分裂的部分

    // z节点初始化
    z->num = SUB_M - 1;

    // 分SUB_M-1个数据过去
    for (int i = 0; i < SUB_M - 1; i++) 
    {
        z->keys[i] = y->keys[SUB_M + i];
    }

    // 如果是在内结点发生split，需要将相应的children一并分裂
    if (y->leaf == 0) 
    {
        // 分SUB_M个分支过去
        for (int i = 0; i < SUB_M; i++) 
        {
            z->children[i] = y->children[SUB_M + i];
        }
    }

    y->num = SUB_M; // 通过限制y的num使y变短 
    
    // 父节点x的处理：为新节点z腾出位置
    for (int i = x->num; i >= idx + 1; i--)
    {
        x->children[i + 1] = x->children[i];
    }
    x->children[idx + 1] = z;

    // 为中间关键字腾出位置并插入
    for (int i = x->num - 1; i >= idx; i--)
    {
        x->keys[i + 1] = x->keys[i];
    }
    x->keys[idx] = y->keys[SUB_M]; //y->keys[SUB_M]就是要上到父节点中的值
    x->num += 1;
}

void btree_insert(btree *T, int key)
{
    btree_node *r = T->root;

    // 如果根节点已满，先分裂
    if (r->num == 2 * SUB_M - 1)
    {
        btree_node *node = btree_node_create(0);
        T->root = node;
        node->children[0] = r;
        btree_split_child(T, node, 0);
        
        // 分裂后，需要确定插入到哪个子节点
        btree_insert_nonfull(T, node, key);  
    }
    else
    {
        // 根节点未满，直接插入
        btree_insert_nonfull(T, r, key); 
    }
}

void btree_insert_nonfull(btree *T, btree_node *x, int key)
{
    int i = x->num - 1;
    
    if (x->leaf) 
    {
        // 叶子节点：找到位置并插入
        while (i >= 0 && key < x->keys[i]) //从右向左扫描，为key找到正确位置，移动比它大的键值，然后插入。如果从左往右遍历会覆盖数据
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = key;
        x->num++;
    } 
    else 
    {
        // 内部节点：找到合适的子节点
        while (i >= 0 && key < x->keys[i]) 
        {
            i--;
        }
        i++;
        
        // 如果子节点已满，先分裂
        if (x->children[i]->num == 2 * SUB_M - 1) 
        {
            btree_split_child(T, x, i);
            if (key > x->keys[i]) 
            {
                i++;
            }
        }
        
        // 递归插入到子节点
        btree_insert_nonfull(T, x->children[i], key);
    }
}

// x: 父节点, idx: 要合并的两个子节点在x中的位置
// 将x->children[idx]和x->children[idx+1]合并，同时将x->keys[idx]下移
void btree_merge(btree *T, btree_node *x, int idx)
{
    btree_node *left = x->children[idx];      // 左子节点
    btree_node *right = x->children[idx + 1]; // 右子节点
    
    // 步骤1: 将父节点的关键字keys[idx]下移到左子节点
    left->keys[SUB_M - 1] = x->keys[idx];
    left->num++;
    
    // 步骤2: 将右子节点的所有关键字复制到左子节点
    for (int i = 0; i < right->num; i++) 
    {
        left->keys[SUB_M + i] = right->keys[i];
    }
    
    // 步骤3: 如果非叶子节点，复制右子节点的所有子节点指针
    if (!left->leaf) 
    {
        for (int i = 0; i <= right->num; i++) 
        {
            left->children[SUB_M + i] = right->children[i];
        }
    }
    
    // 更新左子节点的关键字数量
    left->num += right->num;
    
    // 步骤4: 在父节点中删除下移的关键字和右子节点指针
    for (int i = idx; i < x->num - 1; i++) 
    {
        x->keys[i] = x->keys[i + 1];
    }
    
    // 记住分叉的遍历永远比关键字多1
    for (int i = idx + 1; i < x->num; i++) 
    {
        x->children[i] = x->children[i + 1];
    }
    
    x->num--;
    
    // 步骤5: 如果父节点是根节点且变空，更新根节点
    if (x == T->root && x->num == 0) 
    {
        T->root = left;
        btree_destroy_node(x);
    }
    
    // 步骤6: 释放右子节点
    btree_destroy_node(right);
}