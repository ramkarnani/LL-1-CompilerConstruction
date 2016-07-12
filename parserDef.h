// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"lexerDef.h"
#include"lexer.c"



static int rules[90][10];           //DATA STRUCTURE FOR STORING GRAMMAR RULES.
static char firstSetR[90][54];      //STORING THE RULE-WISE FIRST SET ASSOCIATED WITH A NON-TERMINAL
static char firstNT[50][54];        //STORING THE FIRST SET ASSOCIATED WITH A NON-TERMINAL
static char follow[50][54];         //STORING THE FOLLOW SET ASSOCIATED WITH A NON-TERMINAL
static char followS[90][54];        //STORING THE RULE-WISE FOLLOW SET ASSOCIATED WITH A NON-TERMINAL
static int parseTable[50][53];      //STORING THE PARSE TABLE
static int rhs[90];                 //KEEPING COUNT OF NUMBER OF VARIABLES ON RHS OF RULES-->TO BE USED IN CONSTRUCTION OF PARSE TREE

//CREATING A STRUCTURE FOR STACK ADT
typedef struct stNode{
    int stackValue;
    struct stNode* next;
}stackNode;

stackNode* tos;              //TOP OF STACK POINTER
int stackcount=0;            //NO. OF ELEMENTS IN THE STACK


//CREATING A STRUCTURE FOR TREE NODE ADT
typedef struct treeN{

    char lexemeCurrentNode[30];
    int lineno;
    char token[15];
    char parentNodeSymbol[30];
    int isLeafNode;
    char NodeSymbol[30];
    char valueIfNumber[15];
    int visited;
    int index;
    int enumVal;

    struct treeN* parent;
    struct treeN** child;
    int numChild;

}treeNode;

treeNode* currentPtr=NULL;     //POINTER TO KEEP TRACK OF THE POINT OF ADDITION IN PARSE TREE
treeNode* rootptr;             //POINTER TO ROOT OF THE PARSE TREEE

int parseTreeSize=0;
int parseNodes=0;
int parseError=0;


