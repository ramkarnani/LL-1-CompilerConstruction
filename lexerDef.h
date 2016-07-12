// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#define TK_LEXERROR 105
#define TK_PATERROR 105
#define TK_LENGTH 105
#define TK_COMMENT 104

typedef int buffersize;			// VARIABLE TO STORE THE SIZE OF BUFFER
typedef char* buffer;			// CHARACTER ARRAY REPESENTING THE BUFFER

int lexflag=1;					// A VARIABLE TO CHECK WHETHER THE INPUT HAS BEEN CONSUMED OR NOT
int lineNo;
static buffer firstB;			// THE FIRST BUFFER
static buffer secondB;			// THE SECOND BUFFER
char* forward;
buffersize bsize;

int lexError=0;
//ARRAY TO STORE THE KEYWORDS OF THE LANGUAGE
static char *lexemes[23]={"call","else","end","endif","endrecord","endwhile","global","if","input","int","list","output","parameter","parameters","read","real","record","return","then","type","while","with","write"};


//ENUM TO STORE THE TERMINALS AND NON-TERMINALS AS INTEGERS
typedef enum gS { all,allVar,arithmeticExpression,assignmentStmt,booleanExpression,conditionalStmt,constructedDatatype,dataType
,declaration,declarations,elsePart,expPrime,factor,fieldDefinition,fieldDefinitions,funCallStmt,function
,global_or_not,highPrecedenceOperators,idList,inputParameters,input_par,ioStmt,iterativeStmt,logicalOp
,lowPrecedenceOperators,mainFunction,moreFields,more_ids,optionalReturn,otherFunctions,otherStmts
,outputParameters,output_par,parameter_list,primitiveDatatype,program,relationalOp,remaining_list,returnStmt
,new_24,singleOrRecId,stmt,stmts,temp,term,termPrime,typeDefinition,typeDefinitions,var,$,TK_AND,TK_ASSIGNOP,TK_CALL,TK_CL,TK_COLON,TK_COMMA,TK_DIV,TK_DOT,TK_ELSE,TK_END,TK_ENDIF,TK_ENDRECORD,
TK_ENDWHILE,TK_EQ,TK_FIELDID,TK_FUNID,TK_GE,TK_GLOBAL,TK_GT,TK_ID,TK_IF,TK_LIST,TK_INPUT,TK_INT,TK_LE,TK_LT,TK_MAIN,TK_MINUS,TK_MUL,
TK_NE,TK_NOT,TK_NUM,TK_OP,TK_OR,TK_OUTPUT,TK_PARAMETER,TK_PARAMETERS,TK_PLUS,TK_READ,TK_REAL,TK_RECORD,TK_RECORDID,TK_RETURN,TK_RNUM,TK_SEM,TK_SQL,
TK_SQR,TK_THEN,TK_TYPE,TK_WHILE,TK_WITH,TK_WRITE,eps} grammarSymbols;

//#############################################################################################

// STRUCTURE TO STORE INFORMATION ABOUT THE TOKEN BEING READ FROM PROGRAM
typedef struct lexemeToken
{
  grammarSymbols tokenName;
  char lexemeName[100];
  int lineno;
} tokenInfo;

//##############################################################################################


// ARRAY OF ENUMS TO STORE THE TOKENS

grammarSymbols tokens[23]=
{TK_CALL,TK_ELSE,TK_END,TK_ENDIF,TK_ENDRECORD,TK_ENDWHILE,TK_GLOBAL,TK_IF,TK_INPUT,TK_INT,TK_LIST,TK_OUTPUT,TK_PARAMETER,TK_PARAMETERS,TK_READ,TK_REAL,TK_RECORD,TK_RETURN,TK_THEN,TK_TYPE,TK_WHILE,TK_WITH,TK_WRITE};


//##############################################################################################

// ARRAY OF CHARACTERS TO MAP THE TERMINALS AND NON-TERMINALS TO RESPECTIVE STRINGS USING THE ENUM GS DECLARED ABOVE

const char* alphabet[104]={ "all","allVar","arithmeticExpression","assignmentStmt","booleanExpression","conditionalStmt","constructedDatatype","dataType",
"declaration","declarations","elsePart","expPrime","factor","fieldDefinition","fieldDefinitions","funCallStmt","function","global_or_not",
"highPrecedenceOperators","idList","inputParameters","input_par","ioStmt","iterativeStmt","logicalOp","lowPrecedenceOperators",
"mainFunction","moreFields","more_ids","optionalReturn","otherFunctions","otherStmts","outputParameters","output_par",
"parameter_list","primitiveDatatype","program","relationalOp","remaining_list","returnStmt","new_24","singleOrRecId","stmt","stmts",
"temp","term","termPrime","typeDefinition","typeDefinitions","var","$","TK_AND","TK_ASSIGNOP","TK_CALL","TK_CL","TK_COLON","TK_COMMA",
"TK_DIV","TK_DOT","TK_ELSE","TK_END","TK_ENDIF","TK_ENDRECORD","TK_ENDWHILE","TK_EQ","TK_FIELDID","TK_FUNID","TK_GE","TK_GLOBAL",
"TK_GT","TK_ID","TK_IF","TK_LIST","TK_INPUT","TK_INT","TK_LE","TK_LT","TK_MAIN","TK_MINUS","TK_MUL","TK_NE","TK_NOT","TK_NUM","TK_OP","TK_OR",
"TK_OUTPUT","TK_PARAMETER","TK_PARAMETERS","TK_PLUS","TK_READ","TK_REAL","TK_RECORD","TK_RECORDID","TK_RETURN","TK_RNUM","TK_SEM","TK_SQL",
"TK_SQR","TK_THEN","TK_TYPE","TK_WHILE","TK_WITH","TK_WRITE","eps"};