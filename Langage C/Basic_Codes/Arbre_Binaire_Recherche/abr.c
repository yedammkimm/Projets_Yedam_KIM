#define _GNU_SOURCE
#include "abr.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


typedef int type_t; 

btree * createNode( int key, type_t content , btree * fg , btree * fd )
{
  btree * n =( btree *) malloc (sizeof(btree)) ;
  n->key = key;
  n->content = content ;
  n->fg = fg ;
  n->fd = fd;
  return n;
}


int isBalanced(btree* t)
{
  int lh;
  int rh;
  if(!t)
    return 1;
  lh = height(t->fg);
  rh = height(t->fd);
  return abs(lh - rh) <= 1 && isBalanced(t->fg) && isBalanced(t->fd);
}


int isBSTUtil(btree * t, int min, int max)
{
  if(t == NULL)
    return 1;
  if(t->key < min || t->key > max)
    return 0;
  return isBSTUtil(t->fg,min,t->key) &&  isBSTUtil(t->fd,t->key,max);
}

int isBST(btree * t)
{
  return(isBSTUtil(t, INT_MIN, INT_MAX)); 
}


void deleteTree(btree* tree){
  if(tree != NULL){
    deleteTree(tree->fg);
    deleteTree(tree->fd);
  }
    free(tree);
}


btree* insert(btree* tree, int key, type_t content) {
    if (tree == NULL) {
        return createNode(key, content, NULL, NULL);
    }
    if (key < tree->key) {
        tree->fg = insert(tree->fg, key, content);
    } else if (key > tree->key) {
        tree->fd = insert(tree->fd, key, content);
    }
    return tree;
}



/*Exercice 5*/

void infix_print(btree* tree){
    printf("%d ",tree->key);
    if(tree->fg != NULL){
        print(tree->fg);
    }
    if(tree->fd != NULL){
        print(tree->fd);
    }
}


void print(btree* tree){
    if(tree->fg != NULL){
        print(tree->fg);
    }
    printf("%d ",tree->key);
    if(tree->fd != NULL){
        print(tree->fd);
    }
}


/*Exercice 6*/

btree* search(btree* tree, int key){
  btree* pointer = tree;
    if (tree->key > key){
      pointer = search(tree->fg,key);
    }else if (tree->key == key){
      return tree; 
    }else{
      pointer = search(tree->fd,key);
    }
    return pointer;
}

/*Exercice 7*/

btree* maxBST(btree* tree){
  while(tree->fd != NULL && tree != NULL){
    tree = tree->fd;
  }
  return tree;
}

btree* minBST(btree* tree){
  while(tree->fg != NULL && tree != NULL){
    tree = tree->fg;
    }
  return tree;
}


btree* delete(btree* tree, int key){
    if(tree == NULL){
        printf("Empty Tree");
        return tree;
    }
    if(tree->key > key){
      tree->fg = delete(tree->fg,key);
    }else if(tree->key < key){
      tree->fd = delete(tree->fd,key);
    }else{
      if(tree->fg == NULL){                 
        btree* t = tree->fd;
        free(tree);
        return t;
      }else if(tree->fd == NULL){                                      
        btree* t = tree->fg;
        free(tree);
        return t;
    }
    btree* t = maxBST(tree->fg);
    tree->content = t->content; 
    tree->key = t->key;
    tree->fg = delete(tree->fg,t->key);
    }
    return tree;
}


int max(int x , int y){
  if(x < y){
    return y;
  }else{
    return x; 
  }
}

int height(btree * t){
  if(t == NULL){
    return -1; 
  }else{
      return max(height(t->fd),height(t->fg)) + 1; 
  }
}

btree* rotationLeft(btree *t){
    if (t == NULL || t->fd == NULL){
        return t;
    }
    btree* newtree = t->fd;
    t->fd = newtree->fg;
    newtree->fg = t;
    return newtree;
}
btree* rotationRight(btree *t){
    if (t == NULL || t->fg == NULL){
        return t;
    }
    btree* newtree = t->fg;
    t->fg = newtree->fd;
    newtree->fd = t;
    return newtree;
}

btree* insertBalanced(btree* t, int key, type_t content) {
    t = insert(t, key, content);
    int balance = abs(height(t->fg) - height(t->fd));
    if (balance == 2) {
        if (height(t->fg->fg) < height(t->fg->fd)) {
            t->fg = rotationLeft(t->fg);
        }
        t = rotationRight(t);
    }
    else if (balance == -2) { 
        if (height(t->fd->fd) < height(t->fd->fg)) {
            t->fd = rotationRight(t->fd);
        }
        t = rotationLeft(t);
    }
    return t;
}











