#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


// create the helper fx to track the num of nodes per path...just declaring 
int heightOfPath(Node* node);



bool equalPaths(Node * root)
{
    // Add your code below
// determine if all paths from leaves to root are the same
// first case if out tree is emtpty OR 1 root node only 
  if(root == nullptr){
    return true;
  }

  //here we are returning the result 
  return heightOfPath(root) != -1;

}

  // as for the helper we wanr to return the length of its subtree and the information we can
  //  pass back up os if the lenghts are of the same
int heightOfPath(Node* node){
// in the case of the leaf so the node just has one leaf
if(node->left == nullptr && node->right == nullptr){
  return 1;
}

// other case if we just have a right child 
if(node->left == nullptr){
// recusrively call or heightOfPath on each node to return the count and save into the varibale has
  int h = heightOfPath(node->right);
  if(h== -1) return -1; 
  return h + 1;
}


// case if we just have a left child
if(node->right == nullptr){
  int h = heightOfPath(node->left);
  if(h == -1) return -1;
  return h + 1;
}


// both children exist



int leftChild = heightOfPath(node->left);
int rightChild = heightOfPath(node->right);

if(leftChild == -1 || rightChild == -1 || leftChild != rightChild){
  return -1;
}

return leftChild + 1;

}





