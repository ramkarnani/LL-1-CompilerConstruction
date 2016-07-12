// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include "ast.c"
#define size 128


typedef enum funpara {input=1,output} funpar;

typedef struct Entry
{
	char name[30];
	datatype type;
	int width;
	int offset;
	int occupied;
	char recordType[30];
	funpar iopar;
	struct Entry* next;
	struct Entry* head;
} entry;

entry symbolTable[size];	// to store the varialbles corresponding to main
entry globalST[size];		// to store  global variables 
entry funST[size];			// to store variables corresponding to functions
entry recST[size];			// to store variables corresponding to records
int globalOffset = 0;
int semanticError;