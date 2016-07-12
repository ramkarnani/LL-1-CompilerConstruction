// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include"parser.h"


//################################ PARSING TABLE ####################################################################


//FUNCTION TO CREATE THE PARSE TABLE AS PER THE SPECIFICATIONS
void createParseTable(){
	
	memset(rules,-1,sizeof(rules));

	populateGrammar();
	createFirst();
	createFollow();
	populateParseTable();

	int i,k;
}


//################################ PARSING PROCESS START ##############################################################


//INTEGRATING THE LEXER AND PARSER FUNCTIONS HERE
void parseInputSourceCode(char *testcaseFile){

	//INITIALIZING THE TWIN BUFFERS
	bsize =4096;
	lineNo=1;
	firstB = (buffer)malloc(sizeof(char)*bsize);
	secondB = (buffer)malloc(sizeof(char)*bsize);
	secondB[0]=EOF;
	forward = secondB;                           //marks the end of second buffer so that we can load the first buffer in subsequent code.

	FILE* fp = fopen(testcaseFile,"r");
	if(fp==NULL){
		printf("File not found.\n");
		exit(-1);
	}
	fp = getStream(fp,firstB,bsize);
	forward=firstB;

//####################################PARSING PROCESS BEGINS #########################

	push($);
	push(program);
	

	tokenInfo t;
	t = getNextToken(fp);
	int prevline;
	int i;

	// PARSING LOOP->FETCH THE TOKENS, MATCH WITH THE GRAMMAR RULES USING STACK ADT AND PUSH THE NODE TO PARSE TREE
	while(1){
		i=9;

		//WHEN LEXICAL ERRORS (LOOK FOR LEXICAL ERRORS CODE)
		while(t.tokenName>103 && lexflag !=0) {
			prevline=t.lineno;
			t = getNextToken(fp);
		}

		//WHEN SUCCESSFUL COMPILATION
		if(top()==$ && lexflag==0){
			printf("Compiled Successfully\n");
			break;
		}
		else if((top()==$ && lexflag !=0)){
			parseError =1;
			printf("ERROR_4: Stack is consumed while tokens are getting generated.\n");
			break;
		}
		else if( (top() != $ && lexflag ==0)){

			parseError=1;
			printf("ERROR_4: Input is consumed while it is expected to have token <TK_END> at line number %d.\n",prevline+1);
			break;
		}

		//TERMINALS-> TOP OF STACK AND TOKEN MATCHES
		else if(top()==t.tokenName){
			pop();
			insertTerm(t);    //INSERT INTO THE PARSE TREE
			prevline=t.lineno;
			t = getNextToken(fp);
		}
		//TERMINALS-> TOP OF STACK AND TOKEN DON'T MATCH
		else if(top() > 50 && top() < 103){

			int a = pop();
			currentPtr->visited=1;
			currentPtr->numChild=0;
			parseError=1;
			printf("ERROR_5: The token %s for lexeme <%s> does not match at line %d. The expected token here is %s.\n",alphabet[t.tokenName],t.lexemeName,t.lineno,alphabet[a]);

			//SETTING THE CURRENTPTR FOR PARSE TREE APPROPRIATELY->CHECKED FOR CORNER CASES
			if(currentPtr->index==-1){
		        while(currentPtr->parent->index==-1)
		            currentPtr = currentPtr->parent;
		        currentPtr=currentPtr->parent->parent->child[++(currentPtr->parent->index)];
		    }
		    else
		    	currentPtr=currentPtr->parent->child[++(currentPtr->index)];

		}

		//NON-TERMINALS-> NO GRAMMAR RULE APPLICABLE
		else if(parseTable[top()][t.tokenName-50]==-1){

			if(firstNT[top()][eps-50]=='1'){
				
				int a =pop();
				currentPtr->visited=1;
				currentPtr->numChild=0;
				parseError=1;
				printf("ERROR_5: The token %s for lexeme <%s> does not match at line %d. The expected token here is %s.\n",alphabet[t.tokenName],t.lexemeName,t.lineno,alphabet[a]);

				//SETTING THE CURRENTPTR FOR PARSE TREE APPROPRIATELY->CHECKED FOR CORNER CASES
				if(currentPtr->index==-1){
		            while(currentPtr->parent->index==-1)
		                currentPtr = currentPtr->parent;
		            currentPtr=currentPtr->parent->parent->child[++(currentPtr->parent->index)];
		        }
		        else
		            currentPtr=currentPtr->parent->child[++(currentPtr->index)];
			}
			else{
				prevline=t.lineno;
				t = getNextToken(fp);
			}
		}

		//NON-TERMINALS-> WHEN FIND THE SYCHRONIZATION VALUE IN THE PARSE TABLE
		else if(parseTable[top()][t.tokenName-50]==-100){

			if(stackcount==1){
				prevline=t.lineno;
				t= getNextToken(fp);

			}
			else{
				int a =pop();
				parseError=1;
				printf("ERROR_5: The token %s for lexeme <%s> does not match at line %d. The expected token here is %s.\n",alphabet[t.tokenName],t.lexemeName,t.lineno,alphabet[a]);

				currentPtr->visited=1;
				currentPtr->numChild=0;

				//SETTING THE CURRENTPTR FOR PARSE TREE APPROPRIATELY->CHECKED FOR CORNER CASES
				if(currentPtr->index==-1){
		            while(currentPtr->parent->index==-1)
		                currentPtr = currentPtr->parent;
		            currentPtr=currentPtr->parent->parent->child[++(currentPtr->parent->index)];
		        }
		        else
		            currentPtr=currentPtr->parent->child[++(currentPtr->index)];
			}
		}
		//NON-TERMINALS-> WHEN PARSE TABLE PROVIDES WITH THE APPROPRIATE RULE
		else if(parseTable[top()][t.tokenName-50] !=-1 && parseTable[top()][t.tokenName-50] !=-100){
			int s = parseTable[top()][t.tokenName-50];
			pop();
			insertNode(s,t);           //INSERT THE NODE INTO THE PARSE TREE

			//SHIFTING CURRENTPTR TO COVER THE CORNER CASES
			if(rules[s][1]==eps){
				currentPtr->visited=0;
				    while(currentPtr->parent->index==-1)
		                currentPtr = currentPtr->parent;
		            currentPtr=currentPtr->parent->parent->child[++(currentPtr->parent->index)];
			}

			//PUSHING THE RHS OF THE GRAMMAR RULE INTO THE STACK
			while(i>0){
				int s2 = rules[s][i];
				if(s2 !=-1 && s2 != eps){
					push(s2);
				}
				i--;
			}
		}
	}
}

//##################################### PRINTING THE PARSE TREE ######################################################

void printParseTree(treeNode* root){

    printTree(rootptr);

}



/////////////////////////////////////////////////// HELPER FUNCTIONS /////////////////////////////////////////////////////////////////////////////



//########################################## HELPER FUNCTIONS FOR PARSE TREE #####################################################


//FUNCTION TO COMPUTE THE TOTAL NUMBER OF VARIBALES ON THE RHS OF A GIVEN RULE.
void fillRHS(){
	int i;
	for(i=0;i<90;i++){
		int j=0;
		while(rules[i][j]!=-1)
			j++;
		rhs[i]=j-1;

	}
}


//INSERTING A TERMINAL INTO THE PARSE TREE
void insertTerm(tokenInfo t){

        currentPtr->isLeafNode=1;
        currentPtr->visited=0;

        //CHECKING FOR INTEGER AND REAL NUMBER
        if(t.tokenName==TK_NUM || t.tokenName==TK_RNUM){
            strcpy(currentPtr->lexemeCurrentNode,"");
            strcpy(currentPtr->valueIfNumber,t.lexemeName);
            //currentPtr->valueIfNumber= atoi(t.lexemeName);
        }
        else{
        	strcpy(currentPtr->valueIfNumber,"-");
            //currentPtr->valueIfNumber=-100;
            strcpy(currentPtr->lexemeCurrentNode,t.lexemeName);
        }
        strcpy(currentPtr->token,alphabet[t.tokenName]);

        //new change
        currentPtr->enumVal = t.tokenName;
        currentPtr->lineno = t.lineno;
        currentPtr->numChild=0;

        //HELPS IN MIGRATING THE CURRENTPTR TO APPROPRIATE LOCATION WHERE NEXT NODE WOULD BE ADDED IN THE PARSE TREE
        if(currentPtr->index==-1){

            if(t.tokenName==TK_END && strcmp(currentPtr->parentNodeSymbol,"mainFunction")==0) currentPtr = NULL;  //CHECK FOR CORNER CASES
            else{
                while(currentPtr->parent->index==-1)
                currentPtr = currentPtr->parent;
                currentPtr=currentPtr->parent->parent->child[++(currentPtr->parent->index)];
            }
        }
        else{
            currentPtr=currentPtr->parent->child[++(currentPtr->index)];
        }
}


//INSERTING A NON-TERMINAL INTO THE PARSE TREE
void insertNode(int rulenumber,tokenInfo t){

    int i=1;
    //WHEN "PROGRAM" IS ADDED AT THE CURRENTPTR
    if(currentPtr==NULL){
        currentPtr= (treeNode*)malloc(sizeof(treeNode));
        rootptr= currentPtr;
        currentPtr->parent = NULL;
        currentPtr->index=-1;
        strcpy(currentPtr->parentNodeSymbol,"ROOT");
        strcpy(currentPtr->NodeSymbol,alphabet[rules[rulenumber][0]]);
        //currentPtr->enumVal = rules[rulenumber][0];
    }

    currentPtr->child=(treeNode**)malloc(sizeof(treeNode*)*rhs[rulenumber]);  //CREATING A POINTER FOR ALL THE CHILD FROM THE PARENT NODE
    
    //CREATING THE CHILD NODES, INITIALIZE THEM AND ADD THEM AS CHILD OF THE PARENT NODE.
    while(i <=rhs[rulenumber] ){
        treeNode* temp = (treeNode*)malloc(sizeof(treeNode));
        temp->parent = currentPtr;
        temp->child=NULL;
        temp->visited=1;
        if(i==rhs[rulenumber]) temp->index=-1;
        else
            temp->index=i-1;
        strcpy(temp->parentNodeSymbol,currentPtr->NodeSymbol);
        strcpy(temp->NodeSymbol,alphabet[rules[rulenumber][i]]);
        if(rules[rulenumber][i] == eps) {
        	temp->enumVal = eps;
        	temp->isLeafNode=1;
        }
        strcpy(currentPtr->token,"-");
        strcpy(currentPtr->valueIfNumber,"-");
        currentPtr->child[i-1] = temp;
        i++;
    }

    //BASIC INITIALIZATION    
    currentPtr->numChild=rhs[rulenumber];
    strcpy(currentPtr->lexemeCurrentNode,"----");
    currentPtr->isLeafNode=0;
    currentPtr->visited=0;
    currentPtr->lineno = t.lineno;
    strcpy(currentPtr->token,"-");
    strcpy(currentPtr->valueIfNumber,"-");
    currentPtr->enumVal = rules[rulenumber][0];
    currentPtr=currentPtr->child[0];

}


//########################################## HELPER FUNCTIONS TO PRINT PARSE TREE ####################################################

//Left the valueifnumber case

//PERFORMING THE DEPTH FIRST TRAVERSAL OVER THE PARSE TREE
void printTree(treeNode* p){

    int i;
	char leaf[][4]={"no","yes"};    	
   	
   	if(p->enumVal !=-1){
   		fprintf(stdout,"%-20s  %-10d  %-15s  %-20s  %-20s  %-10s  %-20s  \n\n", p->lexemeCurrentNode,p->lineno,p->token,p->valueIfNumber,p->parentNodeSymbol,leaf[p->isLeafNode],p->NodeSymbol);
   	}
    for(i=0;i<p->numChild;i++){
        if(p->child[i]->visited==0){
            printTree(p->child[i]);
        }
    }
    p->visited=1;
}

void calculateSizeParseTree(treeNode* p){

    int i;
	char leaf[][4]={"no","yes"};    	
   	
   	if(p->enumVal !=-1){
   		parseTreeSize += sizeof(p);
   		parseNodes +=1;
   	}
    for(i=0;i<p->numChild;i++){
        if(p->child[i]->visited==0){
            calculateSizeParseTree(p->child[i]);
        }
    }
    p->visited=1;
}
//########################################## HELPER FUNCTIONS FOR PARSE TABLE ####################################################


//HELPER FUNCTION TO POPULATE THE PARSE TABLE
//SETTING THE RULE NUMBERS AS PER GRAMMAR RULES.
//SETTING THE SYNCHRONIZATION VALUE

void populateParseTable(){

	memset(parseTable,-1,sizeof(parseTable));

	int i,j;
	for(i=0;i<90;i++){
		for(j=0;j<53;j++){
			if(firstSetR[i][j]=='1'){
				parseTable[rules[i][0]][j] = i;
			}
		}
	}

	//WHEN THE FIRSTSET CONTAINS THE "eps"
	for(i=0;i<90;i++){
		if(firstSetR[i][eps-50]=='1'){
			for(j=0;j<53;j++){
				if(followS[i][j]=='1'){
					parseTable[rules[i][0]][j]=i;
				}
			}
		}
	}

	//SETTING SYNCHRONIZATION VALUES
	for(i=0;i<90;i++){
		for(j=0;j<53;j++){
			if(followS[i][j]=='1'){
				if(parseTable[rules[i][0]][j]==-1)
					parseTable[rules[i][0]][j] =-100;
			}
		}
		if(parseTable[rules[i][0]][TK_SEM-50]==-1)
			parseTable[rules[i][0]][TK_SEM-50]==-100;
	}
}

//HELPER FUNCTION TO PRINT THE PARSE TABLE
void printParseTable(){

	int i,j;
	for(i=0;i<50;i++){
		for(j=0;j<53;j++){
			if(parseTable[i][j]!= -1){
				printf("%d %s %s \n",parseTable[i][j]+1,alphabet[i],alphabet[j+50]);
			}
		}
	}

}


//############################################ FUNCTIONS FOR FIRST SET ######################################################

//FUNCTION TO COMPUTE FIRST SET FOR NON-TERMINALS(INSTEAD OF RULE-WISE COMPUTATIONS)
void createFirst(){

	memset(firstSetR,'*',sizeof(firstSetR));
	memset(firstNT,'*',sizeof(firstNT));
	int i;
	for(i=0;i<90;i++){
		helperFirst(i);
	}

	for (i=0;i<50;i++){
		nontermFirst(i);
	}

}

//++++++++++++++++++++++++ HELPER FOR FIRST SET +++++++++++++++++++++++++++++++


//UNION FUNCTION FOR FIRSTSET COMPUTATION
void callUnion(int i,int j){

	int k;
	for (k = 0; k < 54; ++k)
	{
		if(firstSetR[j][k]=='1')
			firstSetR[i][k]= '1';
	}

}

//HELPER FUNCTION TO COMPUTE THE FIRST SET
void helperFirst(int i){

	int l,flag=0;
	int k=1;

	//NON-TERMINAL DETCECTION
	if(rules[i][k] >49){
		firstSetR[i][rules[i][k]-50]='1';
	}

	//FOR ALL THE NON-TERMINALS
	for(l=0;l<54;l++){
		if(firstSetR[i][l]=='1')
			return;
	}

	//MAXIMUM LENGTH OF THE GRAMMAR RULES.
	while(k<10){
		
		if(rules[i][k] >49){
			firstSetR[i][rules[i][k]-50]='1';
			return;
		}

		int j=i+1;
		int m = i-1;
	
	//CHECK FOR SOME CORNER CASES-> (<moreFields>, <allVar>)
	if(i==22||i==50){
		while(m >=i-14){
				if(rules[i][k]==rules[m][0]){
					for(l=0;l<54;l++){
						if(firstSetR[m][l]!='*'){
							flag=1;
							break;
						}				
					}
					if(flag){
							callUnion(i,m);
					}
				}
				m--;
			}
	}

	//COMPUTING FIRST SET FOR ALL THE RULES, RULE WISE COMPUTATIONS.
	while(j<90){
		flag=0;
		if(rules[i][k]==rules[j][0]){
			for(l=0;l<54;l++){
				if(firstSetR[j][l]!='*'){
					flag=1;
					break;
				}				
			}
			if(flag){
					callUnion(i,j);
			}
			else{
				helperFirst(j);
				callUnion(i,j);
			}
		}
		j++;
	}

	//ITERATING ALONG THE i'th RULE.
	if(firstSetR[i][eps-50]=='1'){
		k++;
		if(rules[i][k] != -1)
			firstSetR[i][eps-50]='*';
		if(rules[i][k]==-1){
			return;
		}
	}
	else{
		return;
		}
	}
}

//HELPER FUNCTION TO COMPUTE FIRST SET FOR NON-TERMINALS(INSTEAD OF RULE-WISE COMPUTATIONS)
void callUnion_1(int i,int j){

	int k;
	for (k = 0; k < 54; ++k)
	{
		if(firstSetR[j][k]=='1')
			firstNT[i][k]= '1';
	}

}

//HELPER FUNCTION TO COMPUTE FIRST SET FOR NON-TERMINALS(INSTEAD OF RULE-WISE COMPUTATIONS)
void nontermFirst(int i){
	int j=0;
	while(j<90){
		if(rules[j][0]==i){
			 callUnion_1(i,j);
		}
		j++;
	}
}


//################################################ FUNCTIONS FOR FOLLOW SET ################################################


//FUNCTION TO AUTOMATE THE FOLLOW SET
void createFollow(){

	memset(follow,'*',sizeof(follow));
	int i;
	for(i=0;i<90;i++){
		//printf("%d\n",i );
		computefollow(i);
	}

	int k;
	memset(followS,'*',sizeof(followS));             //COMPUTING RULE-WISE FOLLOW SETS.
	for(i=0;i<90;i++){
				for(k=0;k<54;k++){
					if(follow[rules[i][0]][k]!='*') followS[i][k]='1';
				}
	}
}

//++++++++++++++++++++++++++ HELPER FOR FOLLOW SET ++++++++++++++++++++++++++++++


//UNION FUNCTION FOR FOLLOW SET COMPUTATIONS
//FLAG TELLS WHETHER TO TAKE UNION WITH FIRST SET OR FOLLOW SET
void followunion(int k,int k2,int flag){

	int i;
	if(flag){
		for(i=0;i<54;i++){
			if(follow[k2][i]=='1')
				follow[k][i]='1';
		}
	}
	else{
		for(i=0;i<54;i++){
			if(firstNT[k2][i]=='1' && i+50 !=eps)
				follow[k][i]='1';
		}
	}
}

//HELPER FUNCTION TO COMPUTE THE NON-TERMINAL WISE FOLLOW SET
void computefollow(int k){

	//BASE CASE FOR "program" NON-TERMINAL
	if(k==program){
		follow[k][$-50]='1';
		return;
	}
	int i,j,s,flag;

	//BASE CASE->IF ALREADY FOLLOW COMPUTED, DO NOT CALCULATE AGAIN
	for(s=0;s<54;s++){
		if(follow[k][s] != '*'){
			return;
		}
	}

	//ITERATE OVER ALL THE RULES AND FIND THE POSITIONS OF THE NON-TERMINAL
	for(i=0;i<90;i++){
		for(j=1;j<10;j++){

			flag=0;
			if(rules[i][j]== k){

				//IF THE NON-TERMINAL IS AT THE END OF THE GRAMMAR RULE
				if((j< 9 && rules[i][j+1]==-1 )){

					if(!(rules[i][0]==rules[i][j])){

						for(s=0;s<54;s++){
							if(follow[rules[i][0]][s] != '*'){
								flag=1;
								break;
							}
						}					

						if(flag)followunion(k,rules[i][0],1);
						else{
							computefollow(rules[i][0]);
							followunion(k,rules[i][0],1);

						}
					}
						
				}
				//IF THE FIRST SET OF THE VARIABLE FOLLOWING OUR NON-TERMINAL CONTAINS "eps"
				//MULTIPLE TRIALS DONE TO COVER ALL THE CORNER CASES
				else if((j<9 && rules[i][j+1]<50 && firstNT[rules[i][j+1]][eps-50]=='1' )){


					followunion(k,rules[i][j+1],0);

					//IF THE NON-TERMIANL AND START OF THE RULE IS NOT SAME
					if(!(rules[i][0]==rules[i][j]) && rules[i][j+2]==-1 ){

						for(s=0;s<54;s++){
							if(follow[rules[i][0]][s] != '*'){
								flag=1;
								break;
							}
						}					
						followunion(k,rules[i][j+1],0); 

						if(flag)followunion(k,rules[i][0],1);
						else{
							computefollow(rules[i][0]);
							followunion(k,rules[i][0],1);
						}
					}
					
					//ITERATING OVER THE RULE FOR COVERING THE CASES WHERE THE NON-TERMINAL IS FOLLOWED BY MULTIPLE TERMINALS AND ALL OR SOME ARE GOING TO NULL.
					int sl=j+2;
					while(sl<9){

						if(rules[i][sl] !=-1){

							if(rules[i][sl]>=50){
								follow[k][rules[i][ sl ]-50]='1';
								break;
							}

							//IF THE FIRST SET CONTAINS THE "eps"
							else if(firstNT[rules[i][sl]][eps-50] =='1' ){

								followunion(k,rules[i][ sl ],0);
								sl++;
								if(rules[i][sl]==-1){

									for(s=0;s<54;s++){
										if(follow[rules[i][0]][s] != '*'){
										flag=1;
										break;
										}
									}	
									if(flag)followunion(k,rules[i][0],1);
									else{
										computefollow(rules[i][0]);
										followunion(k,rules[i][0],1);
									}
									break;
								}
								else continue;
							}

							else{
								followunion(k,rules[i][sl],0);
								break;
							}
						}
						else
							break;
					}

				}
				//IF TERMINAL IS FOLLOWING THE NON-TERMINAL IN CONSIDERATION
				else if(rules[i][j+1]>=50 && j<10){
					follow[k][rules[i][j+1]-50]='1';
				}
				//IF THE FIRST SET OF NON-TERMINAL DOES NOT CONTAIN THE "eps"
				else {
					followunion(k,rules[i][j+1],0);
				}

			}
		}
	}
}

//##########################################################################################################################//

void printTokenList(char* fname){


	bsize =4096;
	lineNo=1;
	firstB = (buffer)malloc(sizeof(char)*bsize);
	secondB = (buffer)malloc(sizeof(char)*bsize);

	secondB[0]=EOF;

	forward = secondB; //marks the end of second buffer so that we can load the first buffer in subsequent code.
	

	FILE* fp = fopen(fname,"r");

	fp = getStream(fp,firstB,bsize);
	forward=firstB;

	while(lexflag){
		
		tokenInfo t = getNextToken(fp);
		if(t.tokenName >=0 && t.tokenName < 105 && strcmp(t.lexemeName,"")!=0 && lexflag!=0) {
			printf("%d  %s  %s \n",t.lineno, alphabet[t.tokenName] , t.lexemeName);
		}
	}
}

//##########################################################################################################################//

void printCommentFreeCode(char* fname){

    FILE *fp = fopen(fname,"r");
    if(fp == NULL)
    {
      printf("Error opening file");
      return;
    }

    char s[4096];
    char* ptr;
    int index;
    int i;
    while(!feof(fp)){
         while(fgets(s,4096,fp)!= NULL ){

                ptr = strchr(s,'%');
                if(ptr == NULL)
                    printf("%s",s);
                else{
                    index = ptr - s;
                    i = 0;
                    while(i<index){
                        printf("%c",s[i]);
                        i++;
                    }
                    printf("\n");
                    continue;
                }
         }
    }
    fclose(fp);
}

//##########################################################################################################################//

void printFirstSet(){

	int i,j;
	for(i=0;i<50;i++){
		printf("%s\t-->\t",alphabet[i]);
		for(j=0;j<54;j++){
			if(firstNT[i][j]=='1'){
				printf("%s\t",alphabet[j+50]);
			}
		}
		printf("\n");
	}
}


void printFollowSet(){

	int i,j;
	for(i=0;i<50;i++){
		printf("%s\t-->\t",alphabet[i]);
		for(j=0;j<54;j++){
			if(follow[i][j]=='1'){
				printf("%s\t",alphabet[j+50]);
			}
		}
		printf("\n");
	}
}


//##########################################################################################################################//

//######################################## STACK ADT ####################################################################
//STACK ADT FUNCTIONS
int top (){
    if (tos == NULL)
        return -1;
    else
        return(tos->stackValue);
}

void push(int a){

    stackNode* temp = (stackNode*) malloc(sizeof(stackNode));
    temp->stackValue=a;

    temp->next =tos;
    tos = temp;    
    stackcount++;
}

int pop(){
    
    int a;
    stackNode* temp = tos;
    if(tos==NULL){
        return -1;
    }
    else{
        a = tos->stackValue;
        tos = tos->next;
        free(temp);
        stackcount--;
        return a;
    }
}

//##########################################################################################################################

//##########################################POPULATING THE GRAMMAR ##################################################

void populateGrammar(){

rules[ 0 ][ 0 ]=program;
rules[ 0 ][ 1 ]=otherFunctions;
rules[ 0 ][ 2 ]=mainFunction
;
rules[ 1 ][ 0 ]=mainFunction;
rules[ 1 ][ 1 ]=TK_MAIN;
rules[ 1 ][ 2 ]=stmts;
rules[ 1 ][ 3 ]=TK_END
;
rules[ 2 ][ 0 ]=otherFunctions;
rules[ 2 ][ 1 ]=function;
rules[ 2 ][ 2 ]=otherFunctions
;
rules[ 3 ][ 0 ]=otherFunctions;
rules[ 3 ][ 1 ]=eps
;
rules[ 4 ][ 0 ]=function;
rules[ 4 ][ 1 ]=TK_FUNID;
rules[ 4 ][ 2 ]=input_par;
rules[ 4 ][ 3 ]=output_par;
rules[ 4 ][ 4 ]=TK_SEM;
rules[ 4 ][ 5 ]=stmts;
rules[ 4 ][ 6 ]=TK_END
;
rules[ 5 ][ 0 ]=input_par;
rules[ 5 ][ 1 ]=TK_INPUT;
rules[ 5 ][ 2 ]=TK_PARAMETER;
rules[ 5 ][ 3 ]=TK_LIST;
rules[ 5 ][ 4 ]=TK_SQL;
rules[ 5 ][ 5 ]=parameter_list;
rules[ 5 ][ 6 ]=TK_SQR
;
rules[ 6 ][ 0 ]=output_par;
rules[ 6 ][ 1 ]=TK_OUTPUT;
rules[ 6 ][ 2 ]=TK_PARAMETER;
rules[ 6 ][ 3 ]=TK_LIST;
rules[ 6 ][ 4 ]=TK_SQL;
rules[ 6 ][ 5 ]=parameter_list;
rules[ 6 ][ 6 ]=TK_SQR
;
rules[ 7 ][ 0 ]=output_par;
rules[ 7 ][ 1 ]=eps
;
rules[ 8 ][ 0 ]=parameter_list;
rules[ 8 ][ 1 ]=dataType;
rules[ 8 ][ 2 ]=TK_ID;
rules[ 8 ][ 3 ]=remaining_list
;
rules[ 9 ][ 0 ]=dataType;
rules[ 9 ][ 1 ]=primitiveDatatype
;
rules[ 10 ][ 0 ]=dataType;
rules[ 10 ][ 1 ]=constructedDatatype
;
rules[ 11 ][ 0 ]=primitiveDatatype;
rules[ 11 ][ 1 ]=TK_INT
;
rules[ 12 ][ 0 ]=primitiveDatatype;
rules[ 12 ][ 1 ]=TK_REAL
;
rules[ 13 ][ 0 ]=constructedDatatype;
rules[ 13 ][ 1 ]=TK_RECORD;
rules[ 13 ][ 2 ]=TK_RECORDID
;
rules[ 14 ][ 0 ]=remaining_list;
rules[ 14 ][ 1 ]=TK_COMMA;
rules[ 14 ][ 2 ]=parameter_list
;
rules[ 15 ][ 0 ]=remaining_list;
rules[ 15 ][ 1 ]=eps
;
rules[ 16 ][ 0 ]=stmts;
rules[ 16 ][ 1 ]=typeDefinitions;
rules[ 16 ][ 2 ]=declarations;
rules[ 16 ][ 3 ]=otherStmts;
rules[ 16 ][ 4 ]=returnStmt
;
rules[ 17 ][ 0 ]=typeDefinitions;
rules[ 17 ][ 1 ]=typeDefinition;
rules[ 17 ][ 2 ]=typeDefinitions
;
rules[ 18 ][ 0 ]=typeDefinitions;
rules[ 18 ][ 1 ]=eps
;
rules[ 19 ][ 0 ]=typeDefinition;
rules[ 19 ][ 1 ]=TK_RECORD;
rules[ 19 ][ 2 ]=TK_RECORDID;
rules[ 19 ][ 3 ]=fieldDefinitions;
rules[ 19 ][ 4 ]=TK_ENDRECORD;
rules[ 19 ][ 5 ]=TK_SEM
;
rules[ 20 ][ 0 ]=fieldDefinitions;
rules[ 20 ][ 1 ]=fieldDefinition;
rules[ 20 ][ 2 ]=fieldDefinition;
rules[ 20 ][ 3 ]=moreFields
;
rules[ 21 ][ 0 ]=fieldDefinition;
rules[ 21 ][ 1 ]=TK_TYPE;
rules[ 21 ][ 2 ]=primitiveDatatype;
rules[ 21 ][ 3 ]=TK_COLON;
rules[ 21 ][ 4 ]=TK_FIELDID;
rules[ 21 ][ 5 ]=TK_SEM
;
rules[ 22 ][ 0 ]=moreFields;
rules[ 22 ][ 1 ]=fieldDefinition;
rules[ 22 ][ 2 ]=moreFields
;
rules[ 23 ][ 0 ]=moreFields;
rules[ 23 ][ 1 ]=eps
;
rules[ 24 ][ 0 ]=declarations;
rules[ 24 ][ 1 ]=declaration;
rules[ 24 ][ 2 ]=declarations
;
rules[ 25 ][ 0 ]=declarations;
rules[ 25 ][ 1 ]=eps
;
rules[ 26 ][ 0 ]=declaration;
rules[ 26 ][ 1 ]=TK_TYPE;
rules[ 26 ][ 2 ]=dataType;
rules[ 26 ][ 3 ]=TK_COLON;
rules[ 26 ][ 4 ]=TK_ID;
rules[ 26 ][ 5 ]=global_or_not;
rules[ 26 ][ 6 ]=TK_SEM
;
rules[ 27 ][ 0 ]=global_or_not;
rules[ 27 ][ 1 ]=TK_COLON;
rules[ 27 ][ 2 ]=TK_GLOBAL
;
rules[ 28 ][ 0 ]=global_or_not;
rules[ 28 ][ 1 ]=eps
;
rules[ 29 ][ 0 ]=otherStmts;
rules[ 29 ][ 1 ]=stmt;
rules[ 29 ][ 2 ]=otherStmts
;
rules[ 30 ][ 0 ]=otherStmts;
rules[ 30 ][ 1 ]=eps
;
rules[ 31 ][ 0 ]=stmt;
rules[ 31 ][ 1 ]=assignmentStmt
;
rules[ 32 ][ 0 ]=stmt;
rules[ 32 ][ 1 ]=iterativeStmt
;
rules[ 33 ][ 0 ]=stmt;
rules[ 33 ][ 1 ]=conditionalStmt
;
rules[ 34 ][ 0 ]=stmt;
rules[ 34 ][ 1 ]=ioStmt
;
rules[ 35 ][ 0 ]=stmt;
rules[ 35 ][ 1 ]=funCallStmt
;
rules[ 36 ][ 0 ]=assignmentStmt;
rules[ 36 ][ 1 ]=singleOrRecId;
rules[ 36 ][ 2 ]=TK_ASSIGNOP;
rules[ 36 ][ 3 ]=arithmeticExpression;
rules[ 36 ][ 4 ]=TK_SEM
;
rules[ 37 ][ 0 ]=singleOrRecId;
rules[ 37 ][ 1 ]=TK_ID;
rules[ 37 ][ 2 ]=new_24
;
rules[ 38 ][ 0 ]=new_24;
rules[ 38 ][ 1 ]=TK_DOT;
rules[ 38 ][ 2 ]=TK_FIELDID
;
rules[ 39 ][ 0 ]=new_24;
rules[ 39 ][ 1 ]=eps
;
rules[ 40 ][ 0 ]=funCallStmt;
rules[ 40 ][ 1 ]=outputParameters;
rules[ 40 ][ 2 ]=TK_CALL;
rules[ 40 ][ 3 ]=TK_FUNID;
rules[ 40 ][ 4 ]=TK_WITH;
rules[ 40 ][ 5 ]=TK_PARAMETERS;
rules[ 40 ][ 6 ]=inputParameters;
rules[ 40 ][ 7 ]=TK_SEM
;
rules[ 41 ][ 0 ]=outputParameters;
rules[ 41 ][ 1 ]=TK_SQL;
rules[ 41 ][ 2 ]=idList;
rules[ 41 ][ 3 ]=TK_SQR;
rules[ 41 ][ 4 ]=TK_ASSIGNOP
;
rules[ 42 ][ 0 ]=outputParameters;
rules[ 42 ][ 1 ]=eps
;
rules[ 43 ][ 0 ]=inputParameters;
rules[ 43 ][ 1 ]=TK_SQL;
rules[ 43 ][ 2 ]=idList;
rules[ 43 ][ 3 ]=TK_SQR
;
rules[ 44 ][ 0 ]=iterativeStmt;
rules[ 44 ][ 1 ]=TK_WHILE;
rules[ 44 ][ 2 ]=TK_OP;
rules[ 44 ][ 3 ]=booleanExpression;
rules[ 44 ][ 4 ]=TK_CL;
rules[ 44 ][ 5 ]=stmt;
rules[ 44 ][ 6 ]=otherStmts;
rules[ 44 ][ 7 ]=TK_ENDWHILE
;
rules[ 45 ][ 0 ]=conditionalStmt;
rules[ 45 ][ 1 ]=TK_IF;
rules[ 45 ][ 2 ]=TK_OP;
rules[ 45 ][ 3 ]=booleanExpression;
rules[ 45 ][ 4 ]=TK_CL;
rules[ 45 ][ 5 ]=TK_THEN;
rules[ 45 ][ 6 ]=stmt;
rules[ 45 ][ 7 ]=otherStmts;
rules[ 45 ][ 8 ]=elsePart
;
rules[ 46 ][ 0 ]=elsePart;
rules[ 46 ][ 1 ]=TK_ELSE;
rules[ 46 ][ 2 ]=stmt;
rules[ 46 ][ 3 ]=otherStmts;
rules[ 46 ][ 4 ]=TK_ENDIF
;
rules[ 47 ][ 0 ]=elsePart;
rules[ 47 ][ 1 ]=TK_ENDIF
;
rules[ 48 ][ 0 ]=ioStmt;
rules[ 48 ][ 1 ]=TK_READ;
rules[ 48 ][ 2 ]=TK_OP;
rules[ 48 ][ 3 ]=singleOrRecId;
rules[ 48 ][ 4 ]=TK_CL;
rules[ 48 ][ 5 ]=TK_SEM
;
rules[ 49 ][ 0 ]=ioStmt;
rules[ 49 ][ 1 ]=TK_WRITE;
rules[ 49 ][ 2 ]=TK_OP;
rules[ 49 ][ 3 ]=allVar;
rules[ 49 ][ 4 ]=TK_CL;
rules[ 49 ][ 5 ]=TK_SEM
;
rules[ 50 ][ 0 ]=allVar;
rules[ 50 ][ 1 ]=singleOrRecId
;

rules[ 51 ][ 0 ]=allVar;
rules[ 51 ][ 1 ]=TK_NUM;

rules[ 52 ][ 0 ]=arithmeticExpression;
rules[ 52 ][ 1 ]=term;
rules[ 52 ][ 2 ]=expPrime
;
rules[ 53 ][ 0 ]=expPrime;
rules[ 53 ][ 1 ]=lowPrecedenceOperators;
rules[ 53 ][ 2 ]=term;
rules[ 53 ][ 3 ]=expPrime
;
rules[ 54 ][ 0 ]=expPrime;
rules[ 54 ][ 1 ]=eps
;
rules[ 55 ][ 0 ]=term;
rules[ 55 ][ 1 ]=factor;
rules[ 55 ][ 2 ]=termPrime
;
rules[ 56 ][ 0 ]=termPrime;
rules[ 56 ][ 1 ]=highPrecedenceOperators;
rules[ 56 ][ 2 ]=factor;
rules[ 56 ][ 3 ]=termPrime
;
rules[ 57 ][ 0 ]=termPrime;
rules[ 57 ][ 1 ]=eps
;
rules[ 58 ][ 0 ]=factor;
rules[ 58 ][ 1 ]=TK_OP;
rules[ 58 ][ 2 ]=arithmeticExpression;
rules[ 58 ][ 3 ]=TK_CL
;
rules[ 59 ][ 0 ]=factor;
rules[ 59 ][ 1 ]=all
;
rules[ 60 ][ 0 ]=highPrecedenceOperators;
rules[ 60 ][ 1 ]=TK_MUL
;
rules[ 61 ][ 0 ]=highPrecedenceOperators;
rules[ 61 ][ 1 ]=TK_DIV
;
rules[ 62 ][ 0 ]=lowPrecedenceOperators;
rules[ 62 ][ 1 ]=TK_PLUS
;
rules[ 63 ][ 0 ]=lowPrecedenceOperators;
rules[ 63 ][ 1 ]=TK_MINUS
;

rules[ 64 ][ 0 ]=allVar;
rules[ 64 ][ 1 ]=TK_RNUM
;

rules[ 65 ][ 0 ]=all;
rules[ 65 ][ 1 ]=TK_NUM
;
rules[ 66 ][ 0 ]=all;
rules[ 66 ][ 1 ]=TK_RNUM
;
rules[ 67 ][ 0 ]=all;
rules[ 67 ][ 1 ]=TK_ID;
rules[ 67 ][ 2 ]=temp
;
rules[ 68 ][ 0 ]=temp;
rules[ 68 ][ 1 ]=eps
;
rules[ 69 ][ 0 ]=temp;
rules[ 69 ][ 1 ]=TK_DOT;
rules[ 69 ][ 2 ]=TK_FIELDID
;
rules[ 70 ][ 0 ]=booleanExpression;
rules[ 70 ][ 1 ]=TK_OP;
rules[ 70 ][ 2 ]=booleanExpression;
rules[ 70 ][ 3 ]=TK_CL;
rules[ 70 ][ 4 ]=logicalOp;
rules[ 70 ][ 5 ]=TK_OP;
rules[ 70 ][ 6 ]=booleanExpression;
rules[ 70 ][ 7 ]=TK_CL
;
rules[ 71 ][ 0 ]=booleanExpression;
rules[ 71 ][ 1 ]=var;
rules[ 71 ][ 2 ]=relationalOp;
rules[ 71 ][ 3 ]=var
;
rules[ 72 ][ 0 ]=booleanExpression;
rules[ 72 ][ 1 ]=TK_NOT;
rules[ 72 ][ 2 ]=TK_OP;
rules[ 72 ][ 3 ]=booleanExpression;
rules[ 72 ][ 4 ]=TK_CL
;
rules[ 73 ][ 0 ]=var;
rules[ 73 ][ 1 ]=TK_ID
;
rules[ 74 ][ 0 ]=var;
rules[ 74 ][ 1 ]=TK_NUM
;
rules[ 75 ][ 0 ]=var;
rules[ 75 ][ 1 ]=TK_RNUM
;
rules[ 76 ][ 0 ]=logicalOp;
rules[ 76 ][ 1 ]=TK_AND
;
rules[ 77 ][ 0 ]=logicalOp;
rules[ 77 ][ 1 ]=TK_OR
;
rules[ 78 ][ 0 ]=relationalOp;
rules[ 78 ][ 1 ]=TK_LT
;
rules[ 79 ][ 0 ]=relationalOp;
rules[ 79 ][ 1 ]=TK_LE
;
rules[ 80 ][ 0 ]=relationalOp;
rules[ 80 ][ 1 ]=TK_EQ
;
rules[ 81 ][ 0 ]=relationalOp;
rules[ 81 ][ 1 ]=TK_GT
;
rules[ 82 ][ 0 ]=relationalOp;
rules[ 82 ][ 1 ]=TK_GE
;
rules[ 83 ][ 0 ]=relationalOp;
rules[ 83 ][ 1 ]=TK_NE
;
rules[ 84 ][ 0 ]=returnStmt;
rules[ 84 ][ 1 ]=TK_RETURN;
rules[ 84 ][ 2 ]=optionalReturn;
rules[ 84 ][ 3 ]=TK_SEM
;
rules[ 85 ][ 0 ]=optionalReturn;
rules[ 85 ][ 1 ]=TK_SQL;
rules[ 85 ][ 2 ]=idList;
rules[ 85 ][ 3 ]=TK_SQR
;
rules[ 86 ][ 0 ]=optionalReturn;
rules[ 86 ][ 1 ]=eps
;
rules[ 87 ][ 0 ]=idList;
rules[ 87 ][ 1 ]=TK_ID;
rules[ 87 ][ 2 ]=more_ids
;
rules[ 88 ][ 0 ]=more_ids;
rules[ 88 ][ 1 ]=TK_COMMA;
rules[ 88 ][ 2 ]=idList
;
rules[ 89 ][ 0 ]=more_ids;
rules[ 89 ][ 1 ]=eps;

}
