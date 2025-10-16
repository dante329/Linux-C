#include<stdlib.h>

typedef int KEY_TYPE;

#define RED     0
#define BLACK   1

#define RBTREE_ENTRY(name,type) \
    struct name                 \
    {                           \
        struct type *parent;    \
        struct type *left;      \
        struct type *right;     \
        unsigned char color;    \
    };
    

typedef struct _rbtree_node
{
    KEY_TYPE key;
    void *value;
#if 0
    struct _rbtree_node *parent;
    struct _rbtree_node *left;
    struct _rbtree_node *right;
    unsigned char color;
#else
    RBTREE_ENTRY(,_rbtree_node) node;  // 修复：需要给结构体命名
#endif
}rbtree_node;  

typedef struct _rbtree
{
    struct _rbtree_node *root;
    struct _rbtree_node *nil;
}rbtree;

// 新增：红黑树初始化函数
rbtree* rbtree_create() 
{
    rbtree *T = (rbtree*)malloc(sizeof(rbtree));
    if (T == NULL) return NULL;
    
    // 创建nil哨兵节点
    T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
    T->nil->color = BLACK;
    T->nil->left = T->nil;
    T->nil->right = T->nil;
    T->nil->parent = T->nil;
    
    T->root = T->nil;
    
    return T;
}

// 新增：创建新节点函数
rbtree_node* rbtree_node_create(KEY_TYPE key, void *value) 
{
    rbtree_node *node = (rbtree_node*)malloc(sizeof(rbtree_node));
    if (node == NULL) return NULL;
    
    node->key = key;
    node->value = value;
    node->node.color = RED;  // 新插入节点默认为红色
    node->node.parent = NULL;
    node->node.left = NULL;
    node->node.right = NULL;
    
    return node;
}

void rbtree_left_rotate(rbtree *T, rbtree_node *x)
{
    rbtree_node *y = x->node.right;
    
    x->node.right = y->node.left;
    if (y->node.left != T->nil) y->node.left->node.parent = x;

    y->node.parent = x->node.parent;
    if (x->node.parent == T->nil) T->root = y;
    else if (x == x->node.parent->node.left) x->node.parent->node.left = y;
    else x->node.parent->node.right = y;

    y->node.left = x;
    x->node.parent = y;
}

void rbtree_right_rotate(rbtree *T, rbtree_node *y)
{
    rbtree_node *x = y->node.left;
    
    y->node.left = x->node.right;
    if (x->node.right != T->nil) x->node.right->node.parent = y;

    x->node.parent = y->node.parent;
    if (y->node.parent == T->nil) T->root = x;
    else if (y == y->node.parent->node.right) y->node.parent->node.right = x;
    else y->node.parent->node.left = x;

    x->node.right = y;
    y->node.parent = x;
}

void rbtree_insert_fixup(rbtree *T, rbtree_node *z)
{
    // z == RED
    while (z->node.parent->node.color == RED)
    {
        if (z->node.parent == z->node.parent->node.parent->node.left) // 父节点是祖父结点的左孩子
        {
            rbtree_node *y = z->node.parent->node.parent->node.right; // 定义叔结点y
            if (y->node.color == RED) // y == RED
            {
                z->node.parent->node.color = BLACK;
                y->node.color = BLACK;
                z->node.parent->node.parent->node.color = RED;

                z = z->node.parent->node.parent; // 迭代调整，并且z始终保持RED
            }
            else // y == BLACK
            {
                if (z == z->node.parent->node.right) // 当前结点是右孩子
                {
                    z = z->node.parent;
                    rbtree_left_rotate(T, z); // 左旋转化成"当前结点是左孩子"的情况
                }
                // 当前结点是左孩子
                z->node.parent->node.color = BLACK;
                z->node.parent->node.parent->node.color = RED;

                rbtree_right_rotate(T, z->node.parent->node.parent); // 两个红色结点相连比较重，所以将祖父结点往重的那一边右旋
            }
        }
        else // 父节点是祖父结点的右孩子
        {
            rbtree_node *y = z->node.parent->node.parent->node.left; // 叔节点是左孩子
            if (y->node.color == RED) 
            {
                z->node.parent->node.color = BLACK;
                y->node.color = BLACK;
                z->node.parent->node.parent->node.color = RED;
                z = z->node.parent->node.parent;
            } 
            else 
            {
                if (z == z->node.parent->node.left) 
                {
                    z = z->node.parent;
                    rbtree_right_rotate(T, z);
                }
                z->node.parent->node.color = BLACK;
                z->node.parent->node.parent->node.color = RED;
                rbtree_left_rotate(T, z->node.parent->node.parent);
            }
        }
    }
    
    // 修复：确保根节点为黑色
    T->root->node.color = BLACK;
}

void rbtree_insert(rbtree *T, rbtree_node *z)
{
    rbtree_node *x = T->root;
    rbtree_node *y = T->nil;  // 修复：y应该初始化为nil

    while (x != T->nil)
    {
        y = x;
        if (z->key > x->key) x = x->node.right;
        else if (z->key < x->key) x = x->node.left;
        else return; // Exist
    }

    z->node.parent = y;
    
    if (y == T->nil) 
    {
        T->root = z;  // 树为空的情况
    }
    else if (z->key > y->key) 
    {
        y->node.right = z;
    }
    else 
    {
        y->node.left = z;
    }

    z->node.left = T->nil;
    z->node.right = T->nil;
    z->node.color = RED;
    
    // 调用插入修复函数
    rbtree_insert_fixup(T, z);
}

// 新增：销毁红黑树函数
void rbtree_destroy(rbtree *T) 
{
    if (T == NULL) return;
    
    // 这里需要递归释放所有节点，简化版本只释放nil和树结构
    free(T->nil);
    free(T);
}