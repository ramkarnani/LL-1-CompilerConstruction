// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include "semanticAnalysisDef.h"


void getList(astNode*p, int count);
void whileCheck(astNode* p,int count);
void checkExistence(astNode* p,int count);
void semanticErr();
void getfunList(astNode*p);
void checkFunUpdate(astNode* p);
void funCheck(astNode* p);
void funErr();
int typeAssignment(astNode *p,entry st[]);
entry* search (entry st[],int offset);
int numPar(entry st[],int x);
int numParMain(astNode *p);
void typeCheck(entry st[], astNode *inp, astNode *out);
void traverseAst1(astNode *p, astNode *q, char scope[]);
void traverseAst2(astNode *p, astNode *q, char scope[]);
void extractBoolType(astNode *p);
void extractType(astNode *p);
void printAst(astNode* p,astNode* p1);
void semantics();