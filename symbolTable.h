// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

# include "symbolTableDef.h"

int hash(char a[]);
entry* createNewNode(astNode* p);
void fillEntry(astNode *p,entry a[],int index);
int computeWidth(entry a[]);
void traverseAst(astNode* p,astNode * p1,entry symTable[],int offset);
void traverseAST2(astNode* p,astNode *q, entry *symbolT,int offset,char scope[30]);
char * typeGen (entry st[]);
void fillSymbolTable();
void printST(entry st[], char a[]);
void printSymbolTable();