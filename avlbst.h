#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here


    void rotateLeft(AVLNode<Key, Value>* x);
    void rotateRight(AVLNode<Key, Value>* x);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); 
    void removeFix(AVLNode<Key, Value>* n, int diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{


  const Key& key = new_item.first;
  const Value& value = new_item.second;

// case if tree is empty
  if(this->root_ == NULL){
    this->root_ = new AVLNode<Key, Value>(key, value, NULL);
    return;
  }


    // normal insertion wiht a new node
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* newNode = NULL;
    while(newNode == NULL){
      if(key < curr->getKey()){
        if(curr->getLeft() == NULL){
          newNode = new AVLNode<Key, Value>(key, value, curr);
          curr->setLeft(newNode);
        }else{
           curr = curr->getLeft();
        }
         
      
    }

        else if(curr->getKey() < key){
          if(curr->getRight() == NULL){
            newNode = new AVLNode<Key, Value>(key, value, curr);
            curr->setRight(newNode);
          }
          else {
            curr = curr->getRight();
          }
        } else{
          // last else would be if the key already exisit so we would just overwtitrw the value
          curr->setValue(value);
          return;
        }
      }




// next would be to update the parent node
AVLNode<Key, Value>* parent = curr;
if(parent->getBalance() != 0){
   parent->setBalance(0);
  return;
}if (newNode == parent->getLeft()){
  parent->setBalance(-1);
}else{
  parent->setBalance(1);
}
insertFix(parent, newNode);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key, Value>* n){
  if(p==NULL)   return;
  AVLNode<Key, Value>* g= p->getParent();
  if(g== NULL) return;


  // case when its the left child
    if(p==g->getLeft()){
      g->updateBalance(-1);

        if(g->getBalance() == 0){
          return;
        }
          if(g->getBalance() == -1){
            insertFix(g,p);
            return;
          }


  


  // case when the balance has been violtaed at -2
  if(n == p->getLeft()){
    rotateRight(g);
  p->setBalance(0);
  g->setBalance(0);
  }
  else{
    rotateLeft(p);
    rotateRight(g);
    if(n->getBalance() == -1){
      p->setBalance(0);
      g->setBalance(1);
    }else if(n->getBalance() == 0){
      p->setBalance(0);
      g->setBalance(0);
    }else {
      p->setBalance(-1);
      g->setBalance(0);

    } n->setBalance(0);
  }

} else {
  g->updateBalance(1);
    if(g->getBalance() == 0){
      return;
    } if(g->getBalance() == 1){
      insertFix(g,p);
      return;
    }

    // case when th balance is +2 w two operations
    if(n == p->getRight()){
      rotateLeft(g);
      p->setBalance(0);
      g->setBalance(0);
    }
    else{
      rotateRight(p);
      rotateLeft(g);
      if(n->getBalance() == 1){
        p->setBalance(0);
        g->setBalance(-1);

      }else if(n->getBalance() == 0){
        p->setBalance(0);
        g->setBalance(0);
      }
      else{
        p->setBalance(1);
        g->setBalance(0);
      }
      n->setBalance(0);
    }



}}




template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x){


    AVLNode<Key, Value>* y = x->getRight();
    AVLNode<Key, Value>* parent = x->getParent();

    // next step is changing the child subtree
x->setRight(y->getLeft());
if(y->getLeft() != NULL){
  y->getLeft()->setParent(x);
}



      //move the old 
  y->setParent(parent);
  if(parent == NULL){
    this->root_ = y;
  }
  else if(parent->getLeft()==x) {
      parent->setLeft(y);
  }else{
    parent->setRight(y);
  }



// the x node would become ys left child  
    y->setLeft(x);
    x->setParent(y);



}

// now we can do the right rotation 


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x){


    AVLNode<Key, Value>* y = x->getLeft();
    AVLNode<Key, Value>* parent = x->getParent();

        // next step is changing the child subtree
    x->setLeft(y->getRight());
    if(y->getRight() != NULL){
      y->getRight()->setParent(x);
    }



      //move the old 
  y->setParent(parent);
  if(parent == NULL){
    this->root_ = y;
  }
  else if(parent->getLeft()==x) {
      parent->setLeft(y);
  }else{
    parent->setRight(y);
  }



// the x node would become ys left child  
    y->setRight(x);
    x->setParent(y);



}





/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
   
  AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if(target == NULL){
    return;
  }

  if(target->getLeft() !=NULL && target->getRight() != NULL){
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(
      BinarySearchTree<Key, Value>::predecessor(target)
    );
          nodeSwap(target,pred);
  }




  // see whihc side the parent is shorter
    AVLNode<Key, Value>*parent = target->getParent();
    int diff = 0;
      if(parent != NULL){
        if(parent->getLeft()==target){
          diff = 1;
        }else{
          diff = -1;
        }
      }

      // now we can remove the target 

      AVLNode<Key, Value>* child = target->getLeft();
      if(child == NULL){
        child = target->getRight();
      }if (child != NULL){
        child-> setParent(parent);
      }if(parent == NULL){
        this->root_ = child;
      }
      else if(parent->getLeft() == target){
        parent->setLeft(child);

      }else{
          parent->setRight(child);
      }
        delete target;
        removeFix(parent, diff);

    
}


// now the rmeoveFix fx


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix( AVLNode<Key,Value>* n, int diff)
{
if(n == NULL) return;

    // calc the difference for the next levels
    AVLNode<Key, Value>* p = n->getParent();
    int nextDiff = 0;
    if(p != NULL){
      if(p->getLeft() == n){
        nextDiff = 1;

      }else{
        nextDiff = -1;
      }
    }


    // now for the rhs    
        if(diff == -1){
          if(n->getBalance() + diff == -2){
            AVLNode<Key, Value>* c = n->getLeft();
            if(c->getBalance() == -1){
              rotateRight(n);
              n->setBalance(0);
              c->setBalance(0);
              removeFix(p, nextDiff);
            }
            else if(c->getBalance() == 0){
              rotateRight(n);
              n->setBalance(-1);
              c->setBalance(1);
            
            } else{
              AVLNode<Key, Value>* g = c->getRight();
              rotateLeft(c);
              rotateRight(n);
              if(g->getBalance() == 1){
                n->setBalance(0);
                c->setBalance(-1);
              } else if (g->getBalance() == 0){
                n->setBalance(0);
                c->setBalance(0);
              }
              else{
                n->setBalance(1);
                c->setBalance(0);
              }
              g->setBalance(0);
              removeFix(p, nextDiff);
            } 
          }
          else if(n->getBalance() + diff == -1){
            n->setBalance(-1);
          }else{
            n->setBalance(0);
            removeFix(p, nextDiff);
          }
        }


    // lhs
    else{
      if(n->getBalance() + diff == 2){
        AVLNode<Key, Value>* c = n->getRight();
        if(c->getBalance() == 1 ){
          rotateLeft(n);
          n->setBalance(0);
          c->setBalance(0);
          removeFix(p, nextDiff);
        }
        else if(c->getBalance() == 0){
          rotateLeft(n);
          n->setBalance(1);
          c->setBalance(-1);
        }
        else{
          AVLNode<Key, Value>* g = c->getLeft();
          rotateRight(c);
          rotateLeft(n);
          if(g->getBalance() == -1){
            n->setBalance(0);
            c->setBalance(1);
          }
          else if(g->getBalance()==0){
            n->setBalance(0);
            c->setBalance(0);
          }else {
            n->setBalance(-1);
            c->setBalance(0);
          }
          g->setBalance(0);
          removeFix(p, nextDiff);
        }
      }
      else if (n->getBalance() + diff == 1 ){
        n->setBalance(1);
      }else{
        n->setBalance(0);
        removeFix(p, nextDiff);
      }
    }



  

}





template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
