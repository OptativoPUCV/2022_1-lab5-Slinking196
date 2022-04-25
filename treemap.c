#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * newTree = (TreeMap *) malloc(sizeof(TreeMap));

    if (newTree == NULL) exit(1);
    newTree->current = NULL;
    newTree->root = NULL;
    newTree->lower_than = lower_than;

    return newTree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *newNode = createTreeNode(key, value);
    TreeNode *aux;

    searchTreeMap(tree, key);
    aux = tree->current;
    newNode->parent = aux;
    if (tree->lower_than(aux->pair->key, key) == 1) {
        aux->right = newNode;
    }else if (tree->lower_than(key, aux->pair->key) == 1) {
        aux->left = newNode;
    }
    
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;

    while (x->left != NULL) {
        x = x->left;
    }

    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode *parentNode = node->parent;

    if(tree->lower_than(parentNode->pair->key, node->pair->key) == 1)
    {
        // Solo tiene hijo izquierdo
        if(node->left != NULL && node->right == NULL)
        {
            node->parent->left = node->left;
            node->left->parent = node->parent;
        }
        // Solo tiene hijo derecho
        else if(node->left == NULL && node->right != NULL )
        {
            node->parent->left = node->right;
            node->right->parent = node->parent;
        }
        // Tiene ambos hijos
        else if(node->left != NULL && node->right != NULL)
        {
            TreeNode * aux = minimum(node->right);
            node->pair->key = aux->pair->key;
            node->pair->value = aux->pair->value;
            removeNode(tree, aux);
        }
        // No tiene hijos
        else
        {
            node->parent->left = NULL;
        }
    }
    else
    {
        if(node->left != NULL && node->right == NULL)
        {
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }
        else if(node->left == NULL && node->right != NULL )
        {
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }
            
        else if(node->left != NULL && node->right != NULL)
        {
            TreeNode * aux = node->left;

            aux = minimum(node->right);
            node->pair->key = aux->pair->key;
            node->pair->value = aux->pair->value;
            removeNode(tree, aux);
        }
        else
        {
            node->parent->right = NULL;
        }
    }

    free(node);



    /*if (node->left == NULL && node->right == NULL) {
        if (tree->lower_than(parentNode->pair->key, node->pair->key) == 1) {
            parentNode->right = NULL; 
        }else if (tree->lower_than(node->pair->key, parentNode->pair->key) == 1) {
            parentNode->left = NULL; 
        }
    }else if ((node->left != NULL && node->right == NULL) || (node->left == NULL && node->right != NULL)) {
        if (node->left == NULL) {
            if (tree->lower_than(parentNode->pair->key, node->pair->key) == 1) {
                parentNode->right = node->right; 
            }else if (tree->lower_than(node->pair->key, parentNode->pair->key) == 1) {
                parentNode->left = node->right; 
            }
            node->right->parent = parentNode; 
        }
        if(node->right == NULL) {
            if (tree->lower_than(parentNode->pair->key, node->pair->key) == 1) {
                parentNode->right = node->left; 
            }else if (tree->lower_than(node->pair->key, parentNode->pair->key) == 1) {
                parentNode->left = node->left;
            }
            node->left->parent = parentNode;
        }
        free(node);
    } 
    else {
        TreeNode *min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }*/

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->root;

    while (aux != NULL) {
        tree->current = aux;
        if (tree->lower_than(aux->pair->key, key) == 1) {
            aux = aux->right;
        }else if (tree->lower_than(key, aux->pair->key) == 1) {
            aux = aux->left;
        }else break;
    }

    if (aux == NULL) return NULL;
    tree->current = aux;
    
    return aux->pair;
}

Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *ub_node;
    Pair *flag;

    if (searchTreeMap(tree, key) == NULL) {
        firstTreeMap(tree);
        while ((flag = nextTreeMap(tree)) != NULL) {
            if (tree->lower_than(tree->current->pair->key, key) == 0) {
                break;
            }
        }

        if(flag == NULL) return NULL;
    } 
    ub_node = tree->current;
    
    if(ub_node == NULL) return NULL;
    return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->root;
    tree->current = minimum(aux);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->current;
    TreeNode *parentNode = aux->parent;
    
    if (aux->right != NULL) {
        tree->current = minimum(aux->right);
    } 
    else if(aux->right == NULL){
        while(parentNode != NULL) {
            if (tree->lower_than(parentNode->pair->key, aux->pair->key) == 1) {
                parentNode = parentNode->parent;
                
            }
            else 
                break;
        }
        if (parentNode == NULL) return NULL;
        tree->current = parentNode;
    }
    return tree->current->pair;
}