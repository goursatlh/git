#include <stdio.h>
#include <stdlib.h>

struct Node
{
    struct Node *left, *right;
    int color;
    int key;
    int value;
#define RED 0
#define BLACK 1
};

struct Node *root;

int is_red(struct Node *x)
{
	if (x == NULL)
		return 0;
	return (x->color == RED);
}

struct Node *rotate_right(struct Node *node)
{
    struct Node *x = node;
    struct Node * t = x->left;
    x->left = t->right;
    t->right = x;
    t->color = x->color;
    x->color = RED;
    //printf("rotate_right\n");
    return t;
}

struct Node *rotate_left(struct Node *node)
{
    struct Node *h = node;
    struct Node *x = h->right;
    //printf("rotate_left %x\n", h);
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = RED;
    //printf("rotate_left return %x %x\n", x, x->left);
    return x;
}

void flip_color(struct Node *x)
{
    x->color = RED;
    x->left->color = BLACK;
    x->right->color = BLACK;
    return;
}

struct Node * put(struct Node *node, int key, int value)
{
    struct Node *x = node;
    //printf("put %x\n", x);
    if (x == NULL)
    {
       x = (struct Node *)malloc(sizeof(struct Node));
       if (x)
       {
           x->key = key;
           x->value = value;
           x->color = RED;
           x->left = x->right = NULL;
           //printf("new node: %x, key %d value %d\n", x, x->key, x->value);
       }
       return x;
    }
    if (key < x->key)
    {
       //printf("go into left sub-tree.\n");
       x->left = put(x->left, key, value);
    }
    else if (key > x->key)
    {
       //printf("go into right sub-tree.\n");
       x->right = put(x->right, key, value);
    }
    else 
       printf("duplicate key, can't insert to tree");

    //printf("come here %x, %x\n", x, x->left);
    if (x->left)
    {
        //printf("come here2: %x\n", x->left->left);
    }
    if (is_red(x->right) && !is_red(x->left))
        x = rotate_left(x);
    if (is_red(x->left) && is_red(x->left->left))
        x = rotate_right(x);
    if (is_red(x->left) && is_red(x->right))
        flip_color(x);
    return x;
}

int get(struct Node *x, int key)
{
    if (x)
    {
        if (key < x->key)
           get(x->left, key);
        else if (key > x->key) 
           get(x->right, key);
        else
        {
           printf("key %d value %d\n", key, x->value);
           return x->value;
        }
    }
    return -1;
}

int rb_get(int key)
{
    int value = 0;
    value = get(root, key);
    return value;
}

void rb_put(int key, int value)
{
    printf("\nput: key %d value %d root %x\n", key, value, root);
    root = put(root, key, value);
    if (root)
    {
        root->color = BLACK;
    }
    return;
}

void show(struct Node *x)
{
    if (x)
    {
        show(x->left);
        printf("%d %d %d\n", x->key, x->value, x->color);
        show(x->right);
    }
    return;
}

void rb_show()
{
    printf("show the whole tree: %x\n", root);
    show(root);
}

int main(int argc, char **argv)
{
    int num = 0;
    int a = 0;
    root = NULL;
    // test the insert operation
    printf("please input the number you want to play: \n");
    do {
        scanf("%d", &num);
    } while (num <= 0);

    printf("%d element to build the Red-Black tree.\n", num);
    while (num--)
    {
        rb_put(rand()%100, rand()%100);
    }
    rb_show();
	return 0;
}
