#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "abr.h"


void testInsertion(){
  btree *root  =  createNode(15,0,NULL,NULL);
   insert(root,10,0);  insert(root,8,0);  insert(root,19,0);
   assert(root->key == 15); 
   assert(root->fg->key == 10); 
   assert(root->fg->fg->key == 8); 
   assert(root->fg->fd == NULL); 
   assert(root->fd->key == 19); 
   assert(isBST(root)); 
   deleteTree(root);
   printf("*** \x1B[32m Pass \e[0m : Test insertion ***\n"); 
 }

void testMinandMaxBST() 
 { 
   btree *t  =  createNode(15,0,NULL,NULL); 
   insert(t,10,0);  insert(t,8,0);  insert(t,12,0); 
   insert(t,9,0);  insert(t,19,0);  insert(t,13,0); 
   assert(maxBST(t)->key == 19); 
   assert(minBST(t)->key == 8); 
   deleteTree(t); 
   printf("*** \x1B[32m Pass \e[0m : Test Min and Max m***\n"); 
 } 

 void testSearch() 
 { 
   btree *t  =  createNode(15,0,NULL,NULL); 
   insert(t,10,0);  insert(t,8,0);  insert(t,12,0); 
   insert(t,9,0);  insert(t,19,0);  insert(t,13,0); 
   assert(search(t,9)->key == 9); 
   /*
   assert(search(t,22) == NULL); 
   deleteTree(t); 
   */
   printf("*** \x1B[32m Pass \e[0m : Test Search ***\n"); 
 } 

 void testDelete() 
 { 
   btree *t  =  createNode(15,0,NULL,NULL); 
   insert(t,10,0);  
   insert(t,8,0);  
   insert(t,12,0); 
   insert(t,9,0);  
   insert(t,19,0);  
   insert(t,13,0); 
   insert(t,22,0); 
   insert(t,31,0); 
   insert(t,17,0); 
   assert(t->fg->fg->fd->key == 9);
   delete(t,9); 
   assert(t->fg->fg->fd == NULL); 
   assert(t->fd->fd->key == 22); 
   delete(t,22); 
   assert(t->fd->fd->key == 31); 
   assert(t->key == 15); 
   delete(t,15); 
   assert(t->key == 13); 
   assert(isBST(t)); 
   deleteTree(t); 
   printf("*** \x1B[32m Pass \e[0m : Test Delete ***\n"); 
  } 

 void testHeight() 
 { 
   btree *t  =  createNode(15,0,NULL,NULL); 
   insert(t,10,0);  insert(t,8,0);  insert(t,12,0); 
   insert(t,9,0);  insert(t,19,0);  insert(t,13,0); 
   insert(t,22,0); insert(t,31,0); insert(t,17,0); 
   assert(height(t) == 3); 
   deleteTree(t); 
   printf("*** \x1B[32m Pass \e[0m : Test Height ***\n"); 
 } 

 void testEquilibre() 
 { 
   btree *t  =  createNode(15,0,NULL,NULL); 
   insert(t,10,0);  insert(t,8,0);  insert(t,7,0); 
   insert(t,6,0);  insert(t,19,0);  insert(t,13,0); 
   insert(t,22,0); insert(t,31,0); insert(t,17,0); 
   assert(!(isBalanced(t))&& isBST(t)); 
   deleteTree(t); 
   btree * tt =  createNode(15,0,NULL,NULL); 
   insertBalanced(tt,10,0); insertBalanced(tt,12,0); 
   insertBalanced(tt,9,0);  insertBalanced(tt,19,0); 
   insertBalanced(tt,13,0);insertBalanced(tt,8,0); 
   assert(!(isBalanced(tt)) && isBST(tt)); 
   deleteTree(tt); 
   printf("*** \x1B[32m Pass \e[0m : Test Balanced Tree ***\n"); 
 } 

int main(void)
{
   testInsertion(); 
   testMinandMaxBST(); 
   testSearch(); 
   testDelete(); 
  testHeight();
  testEquilibre(); 

  return EXIT_SUCCESS;
}
