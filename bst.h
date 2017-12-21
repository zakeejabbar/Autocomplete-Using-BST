/*bst.h*/

#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED
#endif // BST_H_INCLUDED

//
// Binary Search Tree ADT header file.
//
// BST type declarations:
//
typedef char*  BSTKey;
typedef struct BSTValue
{
  long long int weight;
  char* key;
} BSTValue;

typedef struct BSTNode
{
  BSTKey    Key;
  BSTValue  Value;
  struct BSTNode  *Left;
  struct BSTNode  *Right;
} BSTNode;

typedef struct BST
{
  BSTNode *Root;
  int      Count;
} BST;


//
// BST API: function prototypes
//
BST *BSTCreate();
int  BSTCompareKeys(BSTKey key1, BSTKey key2);
BSTNode *BSTSearch(BST *tree, BSTKey key);
int  BSTSearchDepth(BST *tree, BSTKey key);
int  BSTInsert(BST *tree, BSTKey key, long long weight);

void BSTPrintInorder(BST *tree, void(*pf)(BSTNode*));
void BSTPrintPreorder(BST *tree, void(*pf)(BSTNode*));
BSTNode *BSTPartialSearch(BST *tree, BSTKey key);

int       BSTCount(BST *tree);
BSTValue *Matches(BSTNode *root, int size, BSTKey key, int* matches);
int BSTHeight(BST *tree);
int BSTNodeCount(BSTNode *root);
