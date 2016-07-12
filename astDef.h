// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include"parser.c"

typedef enum data {integer,real,record,func} datatype;
//CREATING A STRUCTURE FOR AST NODE ADT
typedef struct syntaxtreeN{

    char lexemeCurrentNode[30];
    int lineno;
    int isLeafNode;
    char NodeSymbol[30];
    datatype type;
    char valueIfNumber[15];
    char recordType[30];
    int enumVal;

    struct syntaxtreeN* parentast;
    struct syntaxtreeN* childast;
    struct syntaxtreeN* siblingast;
    int numChild;

}astNode;


astNode* astroot;             //POINTER TO ROOT OF THE AST

int termNT[] =  { TK_CALL,TK_CL,TK_COLON,TK_COMMA,TK_LIST,TK_INPUT,TK_OP,TK_OUTPUT,TK_PARAMETER,TK_PARAMETERS,TK_SEM,
	TK_SQL,TK_SQR,TK_THEN,TK_TYPE,TK_WITH};

int ntermNT[] = { allVar,constructedDatatype,dataType,factor,global_or_not,highPrecedenceOperators,inputParameters,input_par,logicalOp,lowPrecedenceOperators,mainFunction,more_ids,optionalReturn,output_par,primitiveDatatype,
	relationalOp,remaining_list,stmt,temp,var};
int astSize = 0;
int astNodes =0;