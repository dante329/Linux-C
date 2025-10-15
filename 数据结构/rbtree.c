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

    RBTREE_ENTRY(,_rbtree_node);

#endif
}rbtree_node;  

typedef struct _rbtree
{
    struct _rbtree_node *root;
    struct _rbtree_node *nil;
}rbtree;

void rbtree_left_rotate(rbtree *T, rbtree_node *x)
{
    rbtree_node *y = x->right;
    
    x->right = y->left;
    if(y->left != T->nil) y->left->parent = x;

    y->parent = x->parent;
    if(x->parent == T->nil) T->root = y;
    else if(x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rbtree_right_rotate(rbtree *T, rbtree_node *y)
{
    rbtree_node *x = y->left;
    
    y->left = x->right;
    if(x->right != T->nil) x->right->parent = y;

    x->parent = y->parent;
    if(y->parent == T->nil) T->root = x;
    else if(y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;

    x->right = y;
    y->parent = x;
}

void rbtree_insert_fixup(rbtree *T, rbtree_node *z)
{
    // z == RED
    while(z->parent->color == RED)
    {
        if(z->parent == z->parent->parent->left) //父节点是祖父结点的左孩子
        {
            rbtree_node *y = z->parent->parent->right; //定义叔结点y
            if(y->color == RED) // y == RED
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent; //迭代调整，并且z始终保持RED
            }
            else // y == BLACK
            {
                if(z == z->parent->right) //当前结点是右孩子
                {
                    z = z->parent;
                    rbtree_left_rotate(T,z); //左旋转化成“当前结点是左孩子”的情况
                }
                //当前结点是左孩子
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                rbtree_right_rotate(T,z->parent->parent); //两个红色结点相连比较重，所以将祖父结点往重的那一边右旋
            }
        }
        else //父节点是祖父结点的右孩子
        {
            rbtree_node *y = z->parent->parent->left; // 叔节点是左孩子
            if (y->color == RED) 
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else 
            {
                if (z == z->parent->left) 
                {
                    z = z->parent;
                    rbtree_right_rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_left_rotate(T, z->parent->parent);
            }
        }
    }
}

void rbtree_insert(rbtree *T, rbtree_node *z)
{
    rbtree_node *x = T->root;
    rbtree_node *y = T->root;

    while(x != T->nil)
    {
        y = x;
        if(z->key > x->key) x = x->right;
        else if(z->key < x->key) x = x->left;
        else return; //Exist
    }

    if(y == T->nil) T->root = z;

    if(z->key > y->key) y->right = z;
    else y->left = z;

    z->parent = y;
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
}