#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 2
#define MAX_CHILDREN MAX_KEYS + 1

typedef struct node
{
    int num_keys;
    int keys[MAX_KEYS];
    struct node *children[MAX_CHILDREN];
    struct node *parent;
} Node;





Node *create_node(int key, Node *parent)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->num_keys = 1;
    new_node->keys[0] = key;
    new_node->parent = parent;
    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        new_node->children[i] = NULL;
    }
    return new_node;
}



void insert(Node **root, int key)
{
    if (*root == NULL)
    {
        *root = create_node(key, NULL);
        return;
    }
    Node *node = *root;
    while (node != NULL)
    {
        if (node->num_keys == MAX_KEYS)
        {
            if (node->parent == NULL)
            {
                *root = create_node(node->keys[1], NULL);
                (*root)->children[0] = node;
                node->parent = *root;
            }
            Node *new_node = create_node(node->keys[2], node->parent);
            new_node->children[0] = node->children[2];
            new_node->children[1] = node->children[3];
            if (node->children[2] != NULL)
            {
                node->children[2]->parent = new_node;
            }
            if (node->children[3] != NULL)
            {
                node->children[3]->parent = new_node;
            }
            node->num_keys = 1;
            node->children[2] = NULL;
            node->children[3] = NULL;
            if (node == *root)
            {
                *root = node->parent;
            }
            node = node->parent;
            continue;
        }
        int i;
        for (i = 0; i < node->num_keys; i++)
        {
            if (key < node->keys[i])
            {
                break;
            }
        }
        for (int j = node->num_keys; j > i; j--)
        {
            node->keys[j] = node->keys[j - 1];
        }
        node->keys[i] = key;
        node->num_keys++;
        break;
    }
}



void deleted(Node **root, int key)
{
    if (*root == NULL)
    {
        return;
    }
    Node *node = *root;
    while (node != NULL)
    {
        int i;
        for (i = 0; i < node->num_keys; i++)
        {
            if (key == node->keys[i])
            {
                break;
            }
        }

        if (i < node->num_keys)
        {
            if (node->children[i] == NULL)
            {
                for (int j = i; j < node->num_keys - 1; j++)
                {
                    node->keys[j] = node->keys[j + 1];
                }
                node->num_keys--;
                if (node->num_keys == 0 && node->parent != NULL)
                {
                    if (node == node->parent->children[0])
                    {
                        node->parent->children[0] = node->parent->children[1];
                    }
                    else
                    {
                        node->parent->children[1] = NULL;
                    }
                    free(node);
                    node = node->parent;
                    continue;
                }

                break;
            }

            Node *successor = node->children[i];
            while (successor->children[0] != NULL)
            {
                successor = successor->children[0];
            }

            node->keys[i] = successor->keys[0];
            node = successor;
            key = successor->keys[0];
            continue;
        }

        else
        {
            node = node->children[i];
        }
    }
}






int retrieve(Node *root, int key)
{
    if (root == NULL)
    {
        return -1;
    }
    Node *node = root;
    while (node != NULL)
    {
        int i;
        for (i = 0; i < node->num_keys; i++)
        {
            if (key == node->keys[i])
            {
                return key;
            }
        }
        if (i == node->num_keys)
        {
            node = node->children[i];
        }
    }
    return -1;
}





void inorder_traversal(Node *root)
{
    if (root != NULL)
    {
        inorder_traversal(root->children[0]);
        for (int i = 0; i < root->num_keys; i++)
        {
            printf("%d ", root->keys[i]);
            inorder_traversal(root->children[i + 1]);
        }
    }
}

int main()
{
    Node *root = NULL;
    insert(&root, 7);
    insert(&root, 4);
    insert(&root, 12);
    insert(&root, 2);
    insert(&root, 6);
    insert(&root, 9);
    insert(&root, 18);
    insert(&root, 3);
    insert(&root, 5);
    insert(&root, 8);
    insert(&root, 10);
    insert(&root, 14);
    insert(&root, 21);
    deleted (&root, 9);
    deleted (&root, 3);
    deleted (&root, 18);
    inorder_traversal(root);
    return 0;
}




/*


This implementation creates a struct `Node` to represent the nodes in the tree. Each node has an array `keys` to store the keys, an array `children` to store the child nodes, a pointer `parent` to point to the parent node, and an integer `num_keys` to keep track of the number of keys in the node.

The `create_node` function creates a new node with one key and sets the parent pointer to the given parent node.

The `insert` function inserts a new key into the tree by traversing down the tree until it finds a leaf node to insert the key. If the leaf node is full (has two keys), it splits the node into two new nodes and inserts the median key into the parent node. If the parent node is also full, the process continues recursively up the tree until a node is found that is not full.

The `delete` function removes a key from the tree by traversing down the tree until it finds the node containing the key. If the node is a leaf node, it simply removes the key. If the node is an internal node, it finds the successor (i.e., the smallest key in the right subtree) and replaces the key with the successor. If the successor is in a leaf node, it simply removes the successor key. If the successor is in an internal node, it continues recursively down the tree until it finds a leaf node to remove the successor key.

The `retrieve` function searches the tree for a given key and returns the key if it is found or -1 if it is not found.

The `inorder_traversal` function traverses the tree in order (i.e., left child, currentnode, right child) and prints out each key.

Finally, the main function creates an empty root node and inserts a series of keys into the tree. It then deletes some keys and prints out the keys in order.

This implementation assumes that the keys in the tree are integers. If you want to use a different type of key, you will need to modify the code accordingly.


*/