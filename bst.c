/*bst.c*/

//
// Binary Search Tree ADT implementation file.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bst.h"


//
// BSTCreate: dynamically creates and returns an empty
// binary search tree:
//
BST *BSTCreate()
{
  BST *tree;

  tree = (BST *)malloc(sizeof(BST));
  tree->Root = NULL;
  tree->Count = 0;

  return tree;
}


//
// BSTCompareKeys: compares key1 and key2, returning
//   value < 0 if key1 <  key2
//   0         if key1 == key2
//   value > 0 if key1 >  key2
//
int BSTCompareKeys(BSTKey key1, BSTKey key2)
{
  if (strcmp(key1, key2) < 0)
    return -1;
  else if (strcmp(key1, key2) == 0)
    return 0;
  else
    return 1;
}


//
// BSTSearch: searches the binary search tree for a node containing the
// same key.  If a match is found, a pointer to the node is returned,
// otherwise NULL is returned.
//
BSTNode *BSTSearch(BST *tree, BSTKey key)
{
  BSTNode *cur = tree->Root;

  //
  // search the tree to see if it contains a matching key:
  //
  while (cur != NULL)
  {
    if (BSTCompareKeys(key, cur->Key) == 0)  // found!
      return cur;
    else if (BSTCompareKeys(key, cur->Key) < 0)  // smaller, go left:
    {
      cur = cur->Left;
    }
    else  // larger, go right:
    {
      cur = cur->Right;
    }
  }

  // if we get here, we fell out of the tree, and didn't find it:
  return NULL;
}

//searches for the prefix and returns the first
//node with that prefix.
BSTNode *BSTPartialSearch(BST *tree, BSTKey key)
{
  BSTNode *cur = tree->Root;
  //
  // search the tree to see if it contains a matching key:
  //
  while (cur != NULL)
  {


    if (strncmp(cur->Key, key, strlen(key)) == 0)  // found!
      return cur;
    else if (strncmp(key, cur->Key, strlen(key)) < 0) // smaller, go left:
    {
      cur = cur->Left;
    }
    else  // larger, go right:
    {
      cur = cur->Right;
    }
  }

  // not found
  return NULL;
}

//counts the nodes in a sub tree
int BSTNodeCount(BSTNode *root)
{
    int count = 1;
    if(root->Left != NULL)
    {
        count += BSTNodeCount(root->Left);
    }
    if(root->Right != NULL)
    {
        count+= BSTNodeCount(root->Right);
    }

    return count;
}

//stores all the prefix matches in an array
int _Matches(BSTNode *root, BSTValue *values, int *c, BSTKey key)
{

   if(root == NULL)
   {
      return *c;
   }
   else
   {
      if (strncmp(root->Key, key, strlen(key)) == 0)
      {
          values[*c] = root->Value;
          *c = *c+ 1;
      }

      _Matches(root->Left, values, c, key);



      _Matches(root->Right, values, c, key);
      return *c;
   }
}

//returns the array of matched values and number of matches
BSTValue *Matches(BSTNode *root, int size, BSTKey key, int* matches)
{
  BSTValue *values = (BSTValue *)malloc(size * sizeof(BSTValue));
  int c = 0;
  _Matches(root, values, &c, key);
  *matches = c;

  return values;
}




//
// BSTSearchDepth: searches the binary search tree for a node containing the
// same key, returning the depth of that node if found.  The root node is
// considered depth 0, the next level down is a depth of 1, and so forth.
// If a matching key is not found, the function returns a depth of -1.
//
int BSTSearchDepth(BST *tree, BSTKey key)
{
  BSTNode *cur = tree->Root;
  int depth = 0;

  //
  // search the tree to see if it contains a matching key:
  //
  while (cur != NULL)
  {
    if (BSTCompareKeys(key, cur->Key) == 0)  // found!
      return depth;
    else if (BSTCompareKeys(key, cur->Key) < 0)  // smaller, go left:
    {
      cur = cur->Left;
    }
    else  // larger, go right:
    {
      cur = cur->Right;
    }

    depth++;
  }

  // if we get here, we fell out of the tree, and didn't find it:
  return -1;
}


//
// BSTInsert: inserts the given (key, value) pair into the binary search
// tree.  Returns true (non-zero) if the insert was successful, returns
// false (0) if the given key is already in the tree -- in which case the
// given (key, value) pair is not inserted.
//
int BSTInsert(BST *tree, BSTKey key, long long weight)
{
  BSTNode *prev = NULL;
  BSTNode *cur = tree->Root;

  //
  // first we search the tree to see if it already contains key:
  //
  while (cur != NULL)
  {
    if (BSTCompareKeys(key, cur->Key) == 0)  // already in tree, failed:
      return 0;
    else if (BSTCompareKeys(key, cur->Key) < 0)  // smaller, go left:
    {
      prev = cur;
      cur = cur->Left;
    }
    else  // larger, go right:
    {
      prev = cur;
      cur = cur->Right;
    }
  }

  //
  // If we get here, tree does not contain key, so insert new node
  // where we fell out of tree:
  //
  BSTNode *T = (BSTNode *)malloc(sizeof(BSTNode));
  T->Key = malloc(strlen(key) + 1);
  T->Value.key = malloc(strlen(key) + 1);
  strcpy(T->Key, key);
  strcpy(T->Value.key, key);
  T->Value.weight = weight;
  T->Left = NULL;
  T->Right = NULL;

  //
  // link T where we fell out of tree -- after prev:
  //
  if (prev == NULL)  // tree is empty, insert @ root:
  {
    tree->Root = T;
  }
  else if (BSTCompareKeys(key, prev->Key) < 0)  // smaller, insert to left:
  {
    prev->Left = T;
  }
  else  // larger, insert to right:
  {
    prev->Right = T;
  }

  tree->Count++;

  return 1;  // success:
}


//
// BSTPrintInorder: prints the nodes of the given binary search
// tree inorder to the console.
//
// NOTE: "pf" is a print function that must be declared like this
// (though the name "pf" doesn't really matter):
//
//  void pf(BSTNode *cur)
//  { ...}
//
// When you call, pass pf and then you'll be able to control
// what gets printed when a node is "visited".
//
void _BSTPrintInorder(BSTNode *root, void (*pf)(BSTNode*))
{
  if (root == NULL)  // base case: empty tree
    return;
  else  // recursive case: non-empty tree
  {
    _BSTPrintInorder(root->Left, pf);
    pf(root);
    _BSTPrintInorder(root->Right, pf);
  }
}

void BSTPrintInorder(BST *tree, void(*pf)(BSTNode*))
{
  printf(">>Inorder: %d node(s)\n", tree->Count);

  _BSTPrintInorder(tree->Root, pf);

  printf(">><<\n");
}


//
// BSTPrintPreorder: prints the nodes of the given binary search
// tree pre-order to the console.
//
// NOTE: "pf" is a print function that must be declared like this
// (though the name "pf" doesn't really matter):
//
//  void pf(BSTNode *cur)
//  { ...}
//
// When you call, pass pf and then you'll be able to control
// what gets printed when a node is "visited".
//
void _BSTPrintPreorder(BSTNode *root, void(*pf)(BSTNode*))
{
  if (root == NULL)  // base case: empty tree
    return;
  else  // recursive case: non-empty tree
  {
    pf(root);
    _BSTPrintPreorder(root->Left, pf);
    _BSTPrintPreorder(root->Right, pf);
  }
}

void BSTPrintPreorder(BST *tree, void(*pf)(BSTNode*))
{
  printf(">>Preorder: %d node(s)\n", tree->Count);

  _BSTPrintPreorder(tree->Root, pf);

  printf(">><<\n");
}


//
// BSTCount: returns # of nodes in the tree.
//
int BSTCount(BST *tree)
{
  return tree->Count;
}

//helper function to count height
int _max(int x, int y)
{
   return (x>y) ? x : y;
}

//computes the height of the tree
int _BSTHeight(BSTNode *root)
{
   if(root == NULL)
   {
      return -1;
   }
   else
   {
      return 1 + _max(_BSTHeight(root->Left), _BSTHeight(root->Right));
   }
}

//returns the height of the tree.
int BSTHeight(BST *tree)
{
   return _BSTHeight(tree->Root);
}
