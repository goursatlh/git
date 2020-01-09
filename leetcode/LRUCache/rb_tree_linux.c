#include <stdio.h>
#include <stdlib.h>

struct rb_node
{
        unsigned long  rb_parent_color;
#define RB_RED          0
#define RB_BLACK        1
        struct rb_node *rb_right;
        struct rb_node *rb_left;
        int key;
        int value;
};

struct rb_root 
{       
        struct rb_node *rb_node;
        struct rb_node *rb_leftmost;
};      

struct Node
{
    int key;
    int value;
    struct rb_node run_node;
};

void rb_link_node(struct rb_node * node, struct rb_node * parent,
                                struct rb_node ** rb_link)
{
        node->rb_parent_color = (unsigned long )parent;
        node->rb_left = node->rb_right = NULL;
        
        *rb_link = node;
} 

#define rb_parent(r)   ((struct rb_node *)((r)->rb_parent_color & ~3))
#define rb_color(r)   ((r)->rb_parent_color & 1)
#define rb_is_red(r)   (!rb_color(r))
#define rb_is_black(r) rb_color(r)
#define rb_set_red(r)  do { (r)->rb_parent_color &= ~1; } while (0)
#define rb_set_black(r)  do { (r)->rb_parent_color |= 1; } while (0)
                                
static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
{                               
        rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned long)p;
}
static inline void rb_set_color(struct rb_node *rb, int color)
{                       
        rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}               
      
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                      \
                const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
                        (type *)( (char *)__mptr - offsetof(type,member) );})
#define RB_ROOT (struct rb_root) { NULL, NULL}
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

#define RB_EMPTY_ROOT(root)     ((root)->rb_node == NULL)
#define RB_EMPTY_NODE(node)     (rb_parent(node) == node)
#define RB_CLEAR_NODE(node)     (rb_set_parent(node, node))

struct rb_node *rb_prev(const struct rb_node *node)                                                                             
{
        struct rb_node *parent;

        if (rb_parent(node) == node)
                return NULL;

        /* If we have a left-hand child, go down and then right as far
           as we can. */
        if (node->rb_left) {
                node = node->rb_left;
                while (node->rb_right)
                        node=node->rb_right;
                return (struct rb_node *)node;
        }

        /* No left-hand children. Go up till we find an ancestor which
           is a right-hand child of its parent */
        while ((parent = rb_parent(node)) && node == parent->rb_left)
                node = parent;

        return parent;
}

struct rb_node *rb_next(const struct rb_node *node)
{
        struct rb_node *parent;

        if (rb_parent(node) == node)
                return NULL;

        if (node->rb_right) 
        {
                node = node->rb_right;
                while (node->rb_left)
                        node=node->rb_left;
                return (struct rb_node *)node;
        }

        while ((parent = rb_parent(node)) && node == parent->rb_right)                                                          
                node = parent;

        return parent;
}

static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
        struct rb_node *right = node->rb_right;
        struct rb_node *parent = rb_parent(node);

        if ((node->rb_right = right->rb_left))
                rb_set_parent(right->rb_left, node);
        right->rb_left = node;
        
        rb_set_parent(right, parent);
        
        if (parent)
        {
                if (node == parent->rb_left)
                        parent->rb_left = right;
                else
                        parent->rb_right = right;
        }               
        else    
                root->rb_node = right;
        rb_set_parent(node, right);
}


static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
        struct rb_node *left = node->rb_left;
        struct rb_node *parent = rb_parent(node);

        if ((node->rb_left = left->rb_right))
                rb_set_parent(left->rb_right, node);
        left->rb_right = node;

        rb_set_parent(left, parent);

        if (parent)
        {
                if (node == parent->rb_right)
                        parent->rb_right = left;
                else
                        parent->rb_left = left;
        }
        else
                root->rb_node = left;
        rb_set_parent(node, left);
}


void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
        struct rb_node *parent, *gparent;
        printf("%s/%d node %p root %p color %p\n", __FUNCTION__, __LINE__, node, root, node->rb_parent_color);

        while ((parent = rb_parent(node)) && rb_is_red(parent))
        {                    
                printf("%s/%d\n", __FUNCTION__, __LINE__);
                gparent = rb_parent(parent);
                
                if (parent == gparent->rb_left)
                {
                        {
                                register struct rb_node *uncle = gparent->rb_right;
                                if (uncle && rb_is_red(uncle)) 
                                {
                                        rb_set_black(uncle);
                                        rb_set_black(parent);
                                        rb_set_red(gparent);
                                        node = gparent;
                                        continue;
                                }
                        }

                        if (parent->rb_right == node)
                        {
                                register struct rb_node *tmp;
                                __rb_rotate_left(parent, root);
                                tmp = parent;
                                parent = node;
                                node = tmp;
                        }

                        rb_set_black(parent);
                        rb_set_red(gparent);
                        __rb_rotate_right(gparent, root);
                } 
                else 
                {
                        {
                                register struct rb_node *uncle = gparent->rb_left;
                                if (uncle && rb_is_red(uncle))
                                {
                                        rb_set_black(uncle);
                                        rb_set_black(parent);
                                        rb_set_red(gparent);
                                        node = gparent;
                                        continue;
                                }
                        }

                        if (parent->rb_left == node)
                        {
                                register struct rb_node *tmp;
                                __rb_rotate_right(parent, root);
                                tmp = parent;
                                parent = node;
                                node = tmp;
                        }

                        rb_set_black(parent);
                        rb_set_red(gparent);
                        __rb_rotate_left(gparent, root);
                }
        }
        printf("%s/%d node %p root %p color %p\n", __FUNCTION__, __LINE__, node, root, node->rb_parent_color);

        rb_set_black(root->rb_node);
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
                             struct rb_root *root)
{
        struct rb_node *other;

        while ((!node || rb_is_black(node)) && node != root->rb_node)
        {
                if (parent->rb_left == node)
                {
                        other = parent->rb_right;
                        if (rb_is_red(other))
                        {
                                rb_set_black(other);
                                rb_set_red(parent);
                                __rb_rotate_left(parent, root);
                                other = parent->rb_right;
                        }
                        if ((!other->rb_left || rb_is_black(other->rb_left)) &&
                            (!other->rb_right || rb_is_black(other->rb_right)))
                        {
                                rb_set_red(other);
                                node = parent;
                                parent = rb_parent(node);
                        }
                        else
                        {
                                if (!other->rb_right || rb_is_black(other->rb_right))
                                {
                                        rb_set_black(other->rb_left);
                                        rb_set_red(other);
                                        __rb_rotate_right(other, root);
                                        other = parent->rb_right;
                                }
                                rb_set_color(other, rb_color(parent));
                                rb_set_black(parent);
                                rb_set_black(other->rb_right);
                                __rb_rotate_left(parent, root);
                                node = root->rb_node;
                                break;
                        }
                }
                else
                {
                        other = parent->rb_left;
                        if (rb_is_red(other))
                        {
                                rb_set_black(other);
                                rb_set_red(parent);
                                __rb_rotate_right(parent, root);
                                other = parent->rb_left;
                        }
                        if ((!other->rb_left || rb_is_black(other->rb_left)) &&
                            (!other->rb_right || rb_is_black(other->rb_right)))
                        {
                                rb_set_red(other);
                                node = parent;
                                parent = rb_parent(node);
                        }
                        else
                        {
                                if (!other->rb_left || rb_is_black(other->rb_left))
                                {
                                        rb_set_black(other->rb_right);
                                        rb_set_red(other);
                                        __rb_rotate_left(other, root);
                                        other = parent->rb_left;
                                }
                                rb_set_color(other, rb_color(parent));
                                rb_set_black(parent);
                                rb_set_black(other->rb_left);
                                __rb_rotate_right(parent, root);
                                node = root->rb_node;
                                break;
                        }
                }
        }
        if (node)
                rb_set_black(node);
}

void rb_erase(struct rb_node *node, struct rb_root *root)
{
        struct rb_node *child, *parent;
        int color;

        if (!node->rb_left)
                child = node->rb_right;
        else if (!node->rb_right)
                child = node->rb_left;
        else
        {
                struct rb_node *old = node, *left;
               
                // choose the smallest one in the right sub-tree
                node = node->rb_right;
                while ((left = node->rb_left) != NULL)
                        node = left;
        
                if (rb_parent(old)) { 
                        if (rb_parent(old)->rb_left == old)
                                rb_parent(old)->rb_left = node;
                        else
                                rb_parent(old)->rb_right = node;
                } else
                        root->rb_node = node;

                child = node->rb_right;
                parent = rb_parent(node);
                color = rb_color(node);

                if (parent == old) {
                        parent = node;
                } else {
                        if (child)
                                rb_set_parent(child, parent);
                        parent->rb_left = child;

                        node->rb_right = old->rb_right;
                        rb_set_parent(old->rb_right, node);
                }

                node->rb_parent_color = old->rb_parent_color;
                node->rb_left = old->rb_left;
                rb_set_parent(old->rb_left, node);

                goto color;
        }

        parent = rb_parent(node);
        color = rb_color(node);

        if (child)
                rb_set_parent(child, parent);
        if (parent)
        {
                if (parent->rb_left == node)
                        parent->rb_left = child;
                else
                        parent->rb_right = child;
        }
        else
                root->rb_node = child;

 color:
        if (color == RB_BLACK)
                __rb_erase_color(child, parent, root);
}

void rb_delMin(struct rb_root *root)
{
    struct rb_node *node_min = root->rb_leftmost;
    struct rb_node *node = NULL;
    struct Node *entry = NULL;

    if (node_min)
    {
        entry = rb_entry(node_min, struct Node, run_node);
        printf("del min node: %d %d \n", entry->key, entry->value);
        node = rb_next(node_min);
        root->rb_leftmost = node;
        rb_erase(node_min, root);
    }
    return;
}

void show(struct rb_node *node)
{
    struct Node *entry = NULL;
    if (node)
    {
        show(node->rb_left);
        entry = rb_entry(node, struct Node, run_node);
        printf("%d %d %d\n", entry->key, entry->value, rb_color(node));
        show(node->rb_right);
    }
    return;
}

void rb_show(struct rb_root *root)
{
    struct rb_node *node = NULL;
    struct Node *entry = NULL;
    printf("rbtree begin: \n");
    show(root->rb_node);
    printf("rbtree end\n");
#if 0
    node = root->rb_leftmost;
    entry = rb_entry(node, struct Node, run_node);
    printf("rbtree min is: %d %d\n", entry->key, entry->value);
    node = rb_next(node);
    entry = rb_entry(node, struct Node, run_node);
    printf("rbtree next is: %d %d\n", entry->key, entry->value);
    node = rb_prev(node);
    entry = rb_entry(node, struct Node, run_node);
    printf("rbtree next is: %d %d\n", entry->key, entry->value);
#endif

    return;
}

int main()
{
    int num = 0;
    int a = 0;
    struct rb_root root = RB_ROOT;
    struct Node *node = NULL;
    struct Node *entry = NULL;
    struct rb_node **link = &(root.rb_node);
    struct rb_node *parent = NULL;
    int leftmost = 1;
    // test the insert operation
    printf("please input the number you want to play: \n");
    do {
        scanf("%d", &num);
    } while (num <= 0);

    printf("%d element to build the Red-Black tree.\n", num);

    while (num--)
    {
        node = (struct Node *)malloc(sizeof(struct Node));
        if (node == NULL)
        {
            return -1;
        }
        memset(node, 0, sizeof(node));
        node->key = rand()%100;
        node->value = rand()%100;
        //node->run_node.rb_parent_color =  RB_RED;
        printf("insert %d %d \n", node->key, node->value);

        leftmost = 1;
        link = &(root.rb_node);
        // find the place to insert the new node
        while (*link) 
		{
                parent = *link;
                entry = rb_entry(parent, struct Node, run_node);
                if (node->key < entry->key) 
                {
                        link = &parent->rb_left;
                } 
                else 
                {
                        link = &parent->rb_right;
                        leftmost = 0;
                }
        }

        if (leftmost == 1)
        {
            root.rb_leftmost = &node->run_node; 
        }


        // insert the new node to the tree
        rb_link_node(&node->run_node, parent, link);
        // make the tree balance
        rb_insert_color(&node->run_node, &root);

    }
    rb_show(&root);
    rb_delMin(&root);
    rb_show(&root);
    return 0;
}

