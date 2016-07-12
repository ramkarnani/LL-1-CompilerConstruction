// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P


#include "astDef.h"

int binarySearchT(int key);
void setSentinel(treeNode* root);
void traverseTree1(treeNode* p);
void traverseTree2(treeNode*p, astNode* presentPtr );
int binarySearchNT(int key);
void traverseTree3(astNode* p,astNode*q);
void traverseTree4(astNode *p,astNode *p1);
void assignType(astNode *p, astNode*q);
void shrinkTree(astNode *p);
astNode* createNode (astNode *parent,treeNode* current);
astNode* createTree(astNode *p);
astNode* createBoolTree(astNode *p);
void printAST(astNode *p, astNode *q);
void calculateASTsize(astNode *p, astNode *p1);
void printRatios();