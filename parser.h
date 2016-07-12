// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include "parserDef.h"

void push (int a);
int pop();
int top();
void printTree(treeNode* p);
void insertTerm(tokenInfo t);
void insertNode(int rulenumber,tokenInfo t);
void callUnion(int i,int j);
void helperFirst(int i);
void callUnion_1(int i,int j);
void nontermFirst(int i);
void createFirst();
void followunion(int k,int k2,int flag);
void computefollow(int k);
void createFollow();
void populateParseTable();
void printParseTable();
void fillRHS();
void populateGrammar();
void createParseTable();
void parseInputSourceCode(char *testcaseFile);
void printParseTree(treeNode* root);
void printTokenList(char* fname);
void printCommentFreeCode(char* fname);
void printFirstSet();
void printFollowSet();
void calculateSizeParseTree(treeNode* root);
