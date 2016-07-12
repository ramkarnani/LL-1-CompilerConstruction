// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P



# include "semanticAnalysis.h"



void getList(astNode*p, int count){

	if(p==NULL)
		return;

	if(p->enumVal == TK_ID){

		astNode* x = (astNode*)malloc(sizeof(astNode));
		strcpy(x->NodeSymbol,p->NodeSymbol);
		strcpy(x->lexemeCurrentNode,p->lexemeCurrentNode);
		x->enumVal = p->enumVal;
		x->lineno= p->lineno;
		x->siblingast=NULL;
		x->childast=NULL;
		x->parentast=NULL;
		
		if(whileArray[count]==NULL){
			whileArray[count]=x;
		}
		else{
			astNode* y = whileArray[count];
			whileArray[count]=x;
			x->siblingast=y;
		}
		return;
	}
	getList(p->childast,count);
	if(p->childast != NULL)
		getList(p->childast->siblingast,count);
	return;
}


void whileCheck(astNode* p,int count){

	if(p==NULL)
		return;

	if(p->enumVal== iterativeStmt){

		count++;
		addrArray[count]=p;
		astNode* temp = p->childast->siblingast;         //points to 1st operator of boolean expression
		getList(p,count);
		whileCheck(temp->siblingast,count);
		whileCheck(p->siblingast,count);
		return;

	}

	whileCheck(p->childast,count);
	whileCheck(p->siblingast,count);
	return;

}


void checkExistence(astNode* p,int count){

	if(p==NULL || present==1)
		return;

	if(p->enumVal == assignmentStmt){

		astNode* temp = p->childast->childast;          // pointing to TK_ID
		if(temp->siblingast == NULL && temp->enumVal == TK_ID && (temp->type ==0 || temp->type==1)){

			// x is an element of a linked list of boolean variables
			astNode* x = whileArray[count];
			while(x != NULL){

				if(strcmp(x->lexemeCurrentNode,temp->lexemeCurrentNode)==0){
					present = 1;
					return;
				}	
				x = x->siblingast;
			}
		}
		//if(p->siblingast !=)
		checkExistence(p->siblingast,count);
		return;
	}
	if(p->enumVal == ioStmt){
		astNode *temp = p->childast;
		if(temp->enumVal == TK_READ){
			temp = temp->siblingast->childast;	// temp points to TK_ID
			if(temp->siblingast == NULL && temp->enumVal == TK_ID && (temp->type ==0 || temp->type==1)){

				astNode* x = whileArray[count];	// x is an element of a linked list of boolean variables
				while(x != NULL){
					if(strcmp(x->lexemeCurrentNode,temp->lexemeCurrentNode)==0){
						present = 1;
						return;
					}	
					x = x->siblingast;
				}
			}
		}
		checkExistence(p->siblingast,count);
		return;
	}

	if(p->enumVal ==funCallStmt){
		astNode *temp = p->childast;
		if(temp->enumVal == outputParameters){
			temp  = temp->childast->childast;
			while(temp!= NULL){
				if(temp->enumVal == TK_ID && (temp->type ==0 || temp->type==1)){
					astNode* x = whileArray[count];	// x is an element of a linked list of boolean variables
					while(x != NULL){
						if(strcmp(x->lexemeCurrentNode,temp->lexemeCurrentNode)==0){
							present = 1;
							return;
						}	
						x = x->siblingast;
					}
				}
				temp = temp->siblingast;
			}
		}
		checkExistence(p->siblingast,count);
		return;
	}

	checkExistence(p->childast,count);
	checkExistence(p->siblingast,count);
	return;
}


void semanticErr(){

	whileCheck(astroot,-1);

	int i=0;
	for(i=0;i<30;i++){
		present=0;
		if(addrArray[i]==NULL)
			break;
		checkExistence(addrArray[i]->childast,i);
		if(present==0){
			semanticError=1;
			printf("lineno %d: ERROR variable in while loop not being updated\n",addrArray[i]->lineno);
		}
	}
}

void getfunList(astNode*p){

	
	astNode* temp = p->siblingast;

	if(temp->siblingast->enumVal == parameter_list && temp->enumVal == parameter_list){
		temp = temp->siblingast->childast;             //points to TK_ID of output parameters
		
		while(temp!=NULL){
			
			if(temp->enumVal==TK_ID){
				astNode* x = (astNode*)malloc(sizeof(astNode));
				strcpy(x->NodeSymbol,temp->NodeSymbol);
				strcpy(x->lexemeCurrentNode,temp->lexemeCurrentNode);
				x->enumVal = temp->enumVal;
				x->lineno= temp->lineno;
				x->siblingast=NULL;
				x->childast=NULL;
				x->parentast=NULL;
				
				if(funHead==NULL){
					funHead=x;
				}
				else{
					astNode* y = funHead;
					funHead=x;
					x->siblingast=y;
				}
			}
			temp = temp->siblingast;
		}
	return;
	}
}


void checkFunUpdate(astNode* p){

	if(p==NULL || funHead==NULL || p->enumVal==TK_END)
		return;

	if(p->enumVal == assignmentStmt){

		astNode* temp = p->childast->childast;          // pointing to TK_ID
		if(temp->enumVal == TK_ID ){

			// x is an element of a linked list of boolean variables
			astNode* x = funHead;
			astNode* xprev = NULL;

			while(x != NULL){

				if(strcmp(x->lexemeCurrentNode,temp->lexemeCurrentNode)==0){
					
					if(xprev==NULL){
						funHead= x->siblingast;
					}
					else{
						xprev->siblingast = x->siblingast;
					}
				}
				else{
					xprev = x;
				}	
				x = x->siblingast;
			}
		}
		checkFunUpdate(p->siblingast);
		return;
	}

	if(p->enumVal == ioStmt){
		astNode *temp = p->childast;
		if(temp->enumVal == TK_READ){
			temp = temp->siblingast->childast;	// temp points to TK_ID
			
			if(temp->enumVal == TK_ID ){

				// x is an element of a linked list of boolean variables
				astNode* x = funHead;
				astNode* xprev = NULL;

				while(x != NULL){

					if(strcmp(x->lexemeCurrentNode,temp->lexemeCurrentNode)==0){
						
						if(xprev==NULL){
							funHead= x->siblingast;
						}
						else{
							xprev->siblingast = x->siblingast;
						}
					}
					else{
						xprev = x;
					}	
					x = x->siblingast;
				}
			}
		}
		checkFunUpdate(p->siblingast);
		return;
	}

	if(p->enumVal ==funCallStmt){
		astNode *temp = p->childast;
		if(temp->enumVal == outputParameters){
			temp  = temp->childast->childast;
			while(temp!= NULL){
				
				if(temp->enumVal == TK_ID ){

					// x is an element of a linked list of boolean variables
					astNode* x = funHead;
					astNode* xprev = NULL;

					while(x != NULL){

						if(strcmp(x->lexemeCurrentNode,temp->lexemeCurrentNode)==0){
							
							if(xprev==NULL){
								funHead= x->siblingast;
							}
							else{
								xprev->siblingast = x->siblingast;
							}
						}
						else{
							xprev = x;
						}	
						x = x->siblingast;
					}
				}
				temp = temp->siblingast;
			}
		}
		checkFunUpdate(p->siblingast);
		return;
	}

	checkFunUpdate(p->childast);
	checkFunUpdate(p->siblingast);
	return;
}


void funCheck(astNode* p){

	if(p==NULL)
		return;

	if((p->enumVal == TK_FUNID && p->parentast->enumVal != funCallStmt)){

		getfunList(p);
		if(funHead != NULL){                                              //to check for variables update
			checkFunUpdate(p->siblingast->siblingast->siblingast);
		}
		if(funHead !=NULL){                                               //when list is updated by functions i.e. there are some errors then list won't be empty

			while(funHead != NULL){
				semanticError=1;
				printf("lineno %d: ERROR Output variable %s of function is not getting updated\n",p->lineno,funHead->lexemeCurrentNode);
				funHead = funHead->siblingast;	
			}			
			funHead=NULL;
		}
		funCheck(p->siblingast);
		return;
	}
	funCheck(p->childast);
	funCheck(p->siblingast);
	return;
}

void funErr(){

	funCheck(astroot);

	int i=0;
	astNode* x = funHead;
}

int typeAssignment(astNode *p,entry st[]){

	int index = hash(p->lexemeCurrentNode);
	char  s[20] ;

	strcpy(s,p->lexemeCurrentNode);
	//printf("%s\n",s );

	if(st[index].occupied){
		if(strcmp(st[index].name,s)==0){
			//printf("\n%d\n",st[index].type);
			strcpy(p->recordType,st[index].recordType);
			return st[index].type;
		}		
		entry* temp = st[index].head;		
		
		while(temp != NULL){
			if(strcmp(temp->name,s)==0){
				strcpy(p->recordType,temp->recordType);
				return temp->type;
			}
			temp = temp->head;
		}
		return -1;
	}
	else
		return -1;
}

entry* search (entry st[],int offset){			// to compute the index given an offset
	int i;
	for(i=0;i<size;i++){
		if(st[i].occupied){
			if(st[i].offset == offset)
				return &(st[i]);
			else{
				entry *temp = st[i].head;
				while(temp != NULL){
					if(temp->offset==offset)
						return temp;
					temp = temp->head;
				}
			}
		}
	}
}

int numPar(entry st[],int x){				// to compute the no of input and out par
	int i,total =0;
	for(i=0;i<size;i++){
		if(st[i].occupied){
			if(st[i].iopar == x)
				total++;
			else{
				entry *y = st[i].head;
				while(y!=NULL){
					if(y->iopar==x)
						total++;
					y = y->head;
				}
			}
		}
	}
	return total;
}

int numParMain(astNode *p){					// to compute the number of para for fun call in main
	int total=0;
	//printf("%s\n",p->NodeSymbol);
	astNode *temp = p->childast;
	while(temp != NULL){
		//printf("%s\n",temp->NodeSymbol);
		total++;
		temp = temp->siblingast;
	}
	return total;
}

void typeCheck(entry st[], astNode *inp, astNode *out){			// inp points to tdlist of input ,out to outputs
	int offset = 0;
	int i;
	int outMain=0;
	int inpMain =0;
	astNode *temp1 = NULL;
	astNode *temp2 = NULL;
	entry *x;
	int numInp =0;
	int numOut = 0;
	numInp =  numPar(st,1);			// actual no of input parameters in function call
	//printf("%d\t", numInp);
		
	numOut = numPar(st,2);			// actual no of output parameters
	//printf("%d\n", numOut);
		
	if(out!= NULL && out->parentast->enumVal != returnStmt){
		
		inpMain = numParMain(inp);		// number of output para in function call in main
		temp1 = inp->childast;			// points to TK_id of inp
		
		outMain = numParMain(out);		// number of output para in function call in main
		temp2 = out->childast;			// points to TK_id of out
		
		//printf("%d\n", outMain);
		
		int min1 = numInp > inpMain ? inpMain:numInp;
		int min2 = numOut > outMain ? outMain:numOut;
		
		if(numInp != inpMain){
			semanticError =1;
			printf("lineno %d: ERROR: The number of input parameters in the call are not matching the actual format\n",inp->lineno);
		}
		
		if(numOut != outMain){
			semanticError =1;
			printf("lineno %d: ERROR: The number of input parameters in the call are not matching the actual format\n",out->lineno);
		}
		
		for(i=0;i<min1;i++){
			x = search(st,offset);
			//printf("%d %d\n",x->type,temp1->type);
			if(temp1->type != x->type){
				semanticError =1;
				printf("lineno %d: ERROR: The type of the input parameter %s does not match\n",temp1->lineno,temp1->lexemeCurrentNode);
			}
			offset = offset + x->width;
			temp1 = temp1->siblingast;
		}
		for(i=0;i<min2;i++){
			x = search(st,offset);
			if(temp2->type != x->type){
				semanticError =1;
				printf("lineno %d: ERROR: The type of the input parameter %s does not match\n",temp2->lineno,temp2->lexemeCurrentNode);
			}
			
			offset = offset + x->width;
			temp2 = temp2->siblingast;
		}
	}
	else{									// for return stmt
		
		if(out == NULL){
			if(inp!= NULL){						// chechking for input parameters when no output par in call
				inpMain = numParMain(inp);		// number of output para in function call in main
				temp1 = inp->childast;			// points to TK_id of inp
				int min1 = numInp > inpMain ? inpMain:numInp;
				//int min2 = numOut > outMain ? outMain:numOut;
				
				if(numInp != inpMain){
					semanticError =1;
					printf("lineno %d: ERROR: The number of input parameters in the call are not matching the actual format\n",temp1->lineno);
				}
				
				if(numOut != outMain){
					semanticError =1;
					printf("lineno %d: ERROR: The number of output parameters in the call are not matching the actual format\n",temp1->lineno);
				}
				
				for(i=0;i<min1;i++){
					x = search(st,offset);
					//printf("%d %d\n",x->type,temp1->type);
					if(temp1->type != x->type){
						semanticError = 1;
						printf("lineno %d: ERROR: The type of the input parameter %s does not match\n",temp1->lineno,temp1->lexemeCurrentNode);
					}
					offset = offset + x->width;
					temp1 = temp1->siblingast;
				}
			}
			else{
				if(numOut != outMain){			// checking with return stmt 
					semanticError =1;
					printf("lineno %d: ERROR: The number of parameters in the returnStmt are not matching the actual format\n",temp1->lineno);
				}
			}

		}
		
		else{								// for return stmt
				
			outMain = numParMain(out);		// number of output para in return stmt in function call
			temp2 = out->childast;			// points to TK_id of out
			//printf("%s\n", temp2->lexemeCurrentNode);
			
			int min2 = (numOut > outMain) ? outMain:numOut;
			
			//printf("%d\n",min2);
			if(numOut != outMain){
				semanticError =1;
				printf("lineno %d: ERROR: The number of output parameters in the returnStmt are not matching the actual format\n",temp2->lineno);
			}
				for(i=0;i<min2;i++){


					//printf("533\n");
					char s1[20] = "";
					strcpy(s1,temp2->lexemeCurrentNode);
					//int ioVal = temp2->iopar;
					int j = hash(s1);
					if(st[j].occupied){
						if(strcmp(st[j].name,s1)==0){
							//printf("538\n");
							if( (st[j].type != temp2->type) || (st[j].iopar != 2)){			// checking if name and type of parameter same
								//printf("%d %d %d\n",temp2->type,st[j].type,st[j].iopar);
								semanticError =1;
								printf("lineno %d: ERROR : The type of the returning variable %s doesn't match the actual type.\n",temp2->lineno,s1);
							}
						}
						else{
							entry *next = st[j].head;
							while(next!=NULL){
								if( strcmp(next->name,s1)==0){
									if((next->type != temp2->type) || (next->iopar != 2)){
										printf("549\n");
										semanticError =1;
										printf("lineno %d: ERROR : The type of the returning variable %s doesn't match the actual type.\n",temp2->lineno,s1);
									}
									break;
								}
								next = next->head;
							}
						}
					}
					else{
						//printf("560\n");
						semanticError = 1;
						printf("lineno %d: ERROR : The type of the returning variable %s doesn't match the actual type.\n",temp2->lineno,s1);
					}
					
					temp2 = temp2->siblingast;
				}
			}
		}

}
// This function traverses the AST and assigns the types to the variables i.e. TK_ID in the code
void traverseAst1(astNode *p, astNode *q, char scope[]){
	
	if(p==NULL) 
		return;
	// if(q != NULL)
	 //printf("%s\n", p->NodeSymbol);
	astNode * temp;
	astNode * prev;
	char s[30];
	int index,type;

	// This condtition is to byPass the declarations and typedefinitions. 
	if((p->enumVal == TK_FUNID && p->parentast->enumVal != funCallStmt)|| p->enumVal == TK_MAIN){
		//printf("%s\n",p->parentast->NodeSymbol);
		strcpy(s,p->lexemeCurrentNode);
		prev = p;
		temp = p->siblingast;
		while(temp->enumVal != stmts){	// to come directly to stmts
			prev = temp;
			temp = temp->siblingast;
		}
		
		p = temp;					// temp and p are pointing to stmts
		temp = temp ->childast;		// temp points to either type definitions or declarations
		
		while(1)		// to by pass the declarations and typedef
		{
			
			if(temp->enumVal == otherStmts)
			{
				//temp = temp->childast;
				break;
			}

			prev = temp;
			//printf("%s %s\n",temp->NodeSymbol,temp->parentast->NodeSymbol);
			temp = temp->siblingast;
		}
		//printf("%s %s\n",temp->NodeSymbol,temp->parentast->siblingast->NodeSymbol);

		traverseAst1(temp,prev,s);		// the traversal of a function ends.
		//traverseAst1(temp->siblingast,temp,s);
		return;
	}

	if(p->enumVal == TK_ID){	// this is to assign type to variables and checking for undeclared ones
		
		if(strcmp(scope,"_main")!=0){			// to assign type to variables in functions
			//printf("\n %s \n\n",scope );
			index = hash(scope);
			entry * next ;				// points to symol table of the function
			
			if(funST[index].occupied){
				if(strcmp(scope,funST[index].name)==0)
					next = funST[index].next;
				else{
					entry * temp1 = funST[index].head;
					while(strcmp(scope,temp1->name) !=0)
						temp1 = temp1->head;
					next = temp1->next;
				}
			}

			//printf("%s %s\t",p->lexemeCurrentNode,scope);
			type = typeAssignment(p,next);
			//printf("%d\n", type);
			if(type ==-1){			// if variable not in the functions symboltable then check in global
				
				type = typeAssignment(p,globalST);
				
				if(type == -1){		// if not even in GlobalST then ERROR
					printf("lineno %d: ERROR: In func %s The variable %s is an undeclared variable \n",p->lineno,scope,p->lexemeCurrentNode);
					semanticError = 1;
					p->type =-1;
				}
				else
					p->type= type;
			}
			else
				p->type = type;
		}
		
		else{			// if assigning type for variable in main
			//printf("%s\t",p->lexemeCurrentNode);
			type = typeAssignment(p,symbolTable);
			//printf("%d\n",type);
			if(type ==-1){
				//printf("%s\n",p->lexemeCurrentNode);
				type = typeAssignment(p,globalST);
				
				if(type == -1){
					semanticError =1;
					printf("lineno %d : ERROR: In func %s The variable %s is an undeclared variable \n",p->lineno,scope,p->lexemeCurrentNode);
				}
				else
					p->type= type;
				}
			else
				p->type = type;	
		}
	}
	if(p->enumVal == TK_DOT){
		//printf("%s %s\n",q->lexemeCurrentNode,p->siblingast->lexemeCurrentNode);
		if(q->type == 2){
			index = hash(scope);
			entry * next ;				// points to symol table of the function
			entry * next1;
			char rec[30] ;				// to store the record type
			if(strcmp(scope,"_main")!=0){
				
				int i = 0;
				strcpy(rec,q->recordType);
				//printf("%s\n",rec);
				int j = hash(rec);		//now we got the record

				if(recST[j].occupied){
						if(strcmp(rec,recST[j].name)==0)
							next = recST[j].next;
						else{
							entry * temp1 = recST[j].head;
							while(strcmp(rec,temp1->name) !=0)
								temp1 = temp1->head;
							next = temp1->next;		// points to records symbol table
						}
				}
				i = hash(p->siblingast->lexemeCurrentNode);
				if(next[i].occupied){		// looking for field id in recods st
						if(strcmp(p->siblingast->lexemeCurrentNode,next[i].name)==0)
							p->siblingast->type = next[i].type;
						else{
							//printf("%s\n",next[index].name);
							entry * temp1 = next[i].head;
							while(strcmp(p->siblingast->lexemeCurrentNode,temp1->name) !=0)
									temp1 = temp1->head;
							p->siblingast->type = temp1->type;
						}
				}
			}
			else{
				strcpy(rec,q->recordType);
				//printf("%s\n",rec);
				int j = hash(rec);

				if(recST[j].occupied){
						if(strcmp(rec,recST[j].name)==0)
							next = recST[j].next;
						else{
							entry * temp1 = recST[j].head;
							while(strcmp(rec,temp1->name) !=0)
								temp1 = temp1->head;
							next = temp1->next;		// points to records symbol table
						}
				}

				int i = hash(p->siblingast->lexemeCurrentNode);
				//printf("%s\n",next[i].name);
				//printf("%s\n",p->siblingast->lexemeCurrentNode);
				if(next[i].occupied){		// looking for tk id in functions st
						if(strcmp(p->siblingast->lexemeCurrentNode,next[i].name)==0)
							p->siblingast->type = next[i].type;
						else{
							entry * temp1 = next[i].head;
							//printf("%s\n",temp1->name);
							while(strcmp(p->siblingast->lexemeCurrentNode,temp1->name) !=0)
									temp1 = temp1->head;
							p->siblingast->type = temp1->type;
						}
				}
			}
		}
		else{
			printf("lineno %d :ERROR: In func %s The variable %s is not a record type\n",q->lineno,scope,q->lexemeCurrentNode);
		}
	}

	traverseAst1(p->childast,p,scope);
	traverseAst1(p->siblingast,p,scope);
}

// This function does the type checking for the functions
void traverseAst2(astNode *p, astNode *q, char scope[]){
	

	if(p==NULL) 
		return;
	//printf("%s\n",p->NodeSymbol );
	astNode * temp;
	astNode * prev;
	char s[30];
	int index,type;

	if((p->enumVal == TK_FUNID && p->parentast->enumVal != funCallStmt)|| p->enumVal == TK_MAIN){
		
		strcpy(s,p->lexemeCurrentNode);
		prev = p;
		temp = p->siblingast;
		while(temp->enumVal != stmts){	// to come directly to stmts
			prev = temp;
			temp = temp->siblingast;
		}
		
		p = temp;		// p is pointing to stmts
		temp = temp ->childast;
		
		while(1)		// to by pass the declarations and typedef
		{
			if(temp->enumVal == otherStmts )
			{
				astNode* temp1 = temp->childast;											// temp now points to child of stmt
				// if(temp->enumVal == returnStmt && temp->childast->siblingast != NULL){
				// 	semanticError =1;
				// 	printf("lineno %d: ERROR: In function %s() The return parameters are not updated\n",temp->lineno,scope);
				// }
				break;
			}

			//prev = temp;
			//printf("%s %s\n",temp->NodeSymbol,temp->parentast->NodeSymbol);
			temp = temp->siblingast;
		}
		//printf("%s\n",temp->siblingast ->NodeSymbol);
		traverseAst2(temp,prev,s);		// the traversal of a function ends.
		//traverseAst2(temp->siblingast,temp,s);
		return;
	}

	if(p->enumVal == funCallStmt){
		//printf("%s\n", scope);
		if(strcmp(scope,"_main")!= 0){	// type checking for functions in other functions
			if(p->type!=5){
				
				int exist,i;				
				char b[30];						 
				astNode * temp1;
				astNode * temp2;
				temp  = p->childast;	// points to output parameters or Fun iD 
				temp1 = temp;			// points to output parameters or Fun iD
				
				
				while(temp->enumVal != TK_FUNID)
					temp = temp->siblingast;			//temp now points to FUN_ID

				if(temp1->enumVal==outputParameters){
						strcpy(b,temp->lexemeCurrentNode);				// b stores the name of function
						//printf("%s\n", b);
						temp2 = temp1->siblingast->siblingast;			// temp2 points to id list of input par
						//printf("%s\n",temp2->NodeSymbol);
						temp1 = temp1->childast;						// temp1 points to id list of out par
						//printf("%s\n",temp1->NodeSymbol);
						entry *table;
						i = hash(b);
						//printf("%s %s\n",b,funST[i].name);

						if(strcmp(funST[i].name,b)==0)
							typeCheck(funST[i].next,temp2,temp1);		// checks the parameter types of input and output parameters in FunST
						else{
							table = funST[i].head;
							while(!(strcmp(table->name,b)==0))
								table = table->head;
							typeCheck(table->next,temp2,temp1);
						}
				}
				else{
						strcpy(b,temp->lexemeCurrentNode);					
						temp2 = temp1 -> siblingast;					// temp2 points to idList of input parameters
						//printf("%s\n",temp2->NodeSymbol);
						entry *table;
						i = hash(b);
						if(strcmp(funST[i].name,b)==0)
							typeCheck(funST[i].next,temp2,NULL);
						else{
							table = funST[i].head;
							while(!(strcmp(table->name,b)==0))
								table = table->head;
							typeCheck(table->next,temp2,NULL);
						}

				}
			}
		}

		else{						// type checking for functions in _main 
			astNode * temp1;
			astNode * temp2;
			temp  = p->childast;	// points to output parameters or Fun iD 
			temp1 = temp;			// points to output parameters or Fun iD
			//printf("%s\n",temp1->NodeSymbol);
			int exist,i;				
			char b[30];
			
			while(temp->enumVal != TK_FUNID)
				temp = temp->siblingast;

			//printf("%s\n",temp->lexemeCurrentNode);
			exist = typeAssignment(temp,funST);
			//printf("%d\n", exist);
			if(exist == -1){			// if the function is not defined earlier
				semanticError =1;
				printf("lineno %d: ERROR : The function %s() is undefined.\n",p->lineno,temp->lexemeCurrentNode);
				p = p->siblingast;		// moving on to NULL
			}

			else{						// if the function is defined earlier
				if(temp1->enumVal==outputParameters){
					strcpy(b,temp->lexemeCurrentNode);	// b stores the name of function
					//printf("%s\n", b);
					temp2 = temp1->siblingast->siblingast;			// temp2 points to id list of input par
					//printf("%s\n",temp2->NodeSymbol);
					temp1 = temp1->childast;						// temp1 points to id list of out par
					//printf("%s\n",temp1->NodeSymbol);
					entry *table;
					i = hash(b);
					//printf("%s %s\n",b,funST[i].name);

					if(strcmp(funST[i].name,b)==0)
						typeCheck(funST[i].next,temp2,temp1);
					else{
						table = funST[i].head;
						while(!(strcmp(table->name,b)==0))
							table = table->head;
						typeCheck(table->next,temp2,temp1);
					}
				}
				else{
					strcpy(b,temp->lexemeCurrentNode);					
					temp2 = temp1 -> siblingast;
					//printf("%s\n",temp2->NodeSymbol);
					entry *table;
					i = hash(b);
					if(strcmp(funST[i].name,b)==0)
						typeCheck(funST[i].next,temp2,NULL);
					else{
						table = funST[i].head;
						while(!(strcmp(table->name,b)==0))
							table = table->head;
						typeCheck(table->next,temp2,NULL);
					}

				}
			}
		}		
	}

	if( (p->enumVal == returnStmt) && (strcmp(scope,"_main") !=0) ){
		entry *table;
		int index = hash(scope);
		if(strcmp(funST[index].name,scope)==0){			// if we directly find the function 
			
			if(p->childast->siblingast != NULL)			// if there are return parameters.
				typeCheck(funST[index].next,NULL,p->childast->siblingast);
			else
				typeCheck(funST[index].next,NULL,NULL);
		}
			
		else{											// if we have to go for chaining
			table = funST[index].head;
			while(!(strcmp(table->name,scope)==0))
				table = table->head;
			if(p->childast->siblingast != NULL)
				typeCheck(table->next,NULL,p->childast->siblingast);
			else
				typeCheck(table->next,NULL,NULL);
		}
	}

	if( (p->enumVal == returnStmt) && (strcmp(scope,"_main") ==0) ){
		if(p->childast->siblingast != NULL){
			semanticError = 1;
			printf("lineno %d: ERROR: The main function should not return anything.\n",p->lineno);
		}
	}

	if(p->enumVal == conditionalStmt || p->enumVal == iterativeStmt){
		astNode * temp = p->childast->siblingast;
		//printf("%s\n",temp->NodeSymbol );
		extractBoolType(temp);
		//printf("%d\n",temp->type);
		if(temp->type==-1){
			semanticError = 1 ;
			printf("lineno %d: ERROR: in %s, type mismatch in Boolean Expression.\n",temp->lineno,scope);
		}
	}
	
	if(p->enumVal ==assignmentStmt){
		//printf("466\n");
		astNode * temp = p->childast;		//temp points to single or rec id

		//printf("\n%s\n",temp->childast->lexemeCurrentNode);
		if(temp->childast->siblingast ==NULL){
			if(temp->childast->type == -1){
				traverseAst2(p->siblingast,p,scope);
				return;
			}
		}
		else{
			if(temp->childast->type == -1){
				traverseAst2(p->siblingast,p,scope);
				return;
			}	
			if(temp->childast->siblingast->siblingast->type == -1){
				traverseAst2(p->siblingast,p,scope);
				return;
			}
		}

		temp = p->childast; 		// pointd to single or rec id on left
		astNode* t = p->childast->siblingast->siblingast;	// t points to the root operator
		extractType(t);
		if(temp->childast->siblingast ==NULL){
			// printf("%d\n", temp->childast->type);
			// printf("977\n%d\n",t->type );
			if(temp->childast->type != t->type){
				semanticError =1;
				printf("lineno %d: ERROR: in %s, type mismatch in arithmeticExpression.\n",temp->lineno,scope);
			}
			else if(temp->childast->type ==2 && t->type==2){
				if(strcmp(temp->childast->recordType,t->recordType)!=0){
					int i,j;
					i = hash(temp->childast->recordType);
					j = hash(t->recordType);
					entry *a;
					entry *b;
					int width1 = recST[i].width;
					int width2 = recST[i].width;
					int offset =0;
					if(width1 == width2){
						if(strcmp(temp->childast->recordType,recST[i].name)==0)
							a = recST[i].next;
						else{
							a = recST[i].head;
							while(strcmp(temp->childast->recordType,a->name)!=0)
								a = a->head;
							a = a->next;
						}

						if(strcmp(t->recordType,recST[j].name)==0)
							b = recST[j].next;
						else{
							b = recST[j].head;
							while(strcmp(t->recordType,b->name)!=0)
								b = b->head;
							b = b->next;
						}
						while(offset<width1){
	 						entry *x = search(a,offset);
	 						entry *y = search(b,offset);
		 					if(x->type==y->type){
		 						offset = offset + x->width;
		 					}
		 					else{
		 						printf("lineno %d: ERROR: in %s, type mismatch in arithmeticExpression.\n",temp->lineno,scope);
		 						break;
		 					}
	 					}
					}
				}
			}
			else{
					if(temp->childast->type != t->type)
						printf("lineno %d: ERROR: in %s, type mismatch in arithmeticExpression.\n",temp->lineno,scope);
				}
		}

		else{
			if(temp->childast->siblingast->siblingast->type != t->type){	//no need to check for record type here
				semanticError =1;
				printf("lineno %d: ERROR: in %s, type mismatch in arithmeticExpression.\n",temp->lineno,scope);	
			}
		}
		//printf("%d\n",t );
	}

	traverseAst2(p->childast,p,scope);
	traverseAst2(p->siblingast,p,scope);
}


void extractBoolType(astNode *p){
	
	if(p->isLeafNode)
		return;
	
	astNode* x1 = p->childast;
	//printf("%s\n",x1->NodeSymbol);
	extractBoolType(x1);
	int t1 = x1->type;
	if(p->childast->siblingast == NULL){
		//printf("649\n");
		p->type = t1;
		return;
	}
	astNode* x2 = p->childast->siblingast;
	extractBoolType(x2);
	int t2 = x2->type;

	if(t1==-1|| t2==-1){
		p->type = -1;
		return ;
	}
	if(t1==t2 && (t1 == integer ||t1==real) ){
		p->type = t1;
		return;
	}
	else{
		p->type=-1;
		return;
	}
	
}


void extractType(astNode *p){
	if(p->isLeafNode)
		return ;
	astNode* x1 = p->childast;
	astNode* x2 = p->childast->siblingast;
	extractType(x1);
	int t1 = x1->type;
	extractType(x2);
	int t2 = x2->type;
	// printf("%s %s\n",x1->valueIfNumber,x2->valueIfNumber );
	// printf("%d %d\n",t1,t2);
	if(t1==-1|| t2==-1){
		p->type = -1;
		return ;
	}
	if(p->enumVal == TK_PLUS||p->enumVal == TK_MINUS){
		if(t1 == t2 && (t1 == integer||t1 == real) ){
			p->type = t1;
			return ;
		}
		else if( t1== record && t2==record && (strcmp(p->childast->recordType,p->childast->siblingast->recordType)==0)){
			p->type = record;
			strcpy(p->recordType,p->childast->recordType);
			return ;
		}
		else if(t1== record && t2==record && (strcmp(p->childast->recordType,p->childast->siblingast->recordType)!=0)){
			int i,j;
			i = hash(p->childast->recordType);
			j = hash(p->childast->siblingast->recordType);
			entry *a;
			entry *b;
			int width1 = recST[i].width;
			int width2 = recST[i].width;
			int offset;
			if(width1 == width2){
				if(strcmp(p->childast->recordType,recST[i].name)==0)
					a = recST[i].next;
				else{
					a = recST[i].head;
					while(strcmp(p->childast->recordType,a->name)!=0)
						a = a->head;
					a = a->next;
				}

				if(strcmp(p->childast->siblingast->recordType,recST[j].name)==0)
					b = recST[j].next;
				else{
					b = recST[j].head;
					while(strcmp(p->childast->siblingast->recordType,b->name)!=0)
						b = b->head;
					b = b->next;
				}
				i = 0;
 				while(offset<width1){
 					entry *x = search(a,offset);
 					entry *y = search(b,offset);
 					if(x->type==y->type){
 						offset = offset + x->width;
 					}
 					else{
 						p->type = -1;
 						return;
 					}
 				}
 				p->type = record;
 				strcpy(p->recordType,p->childast->recordType);
 				return;
			}
			else{
				p->type = -1;
				return;
			}
		}
		else{
			//printf("%s %s\n", p->childast->recordType,p->childast->siblingast->recordType);
			p->type = -1;
			return ;
		}
	}
	if(p->enumVal == TK_MUL||p->enumVal == TK_DIV){
		if(t1 == integer && t2 == integer || t1 == real && t2 == real){
			p->type = t1;
			//printf("%d\n", p->type);
			return ;
		}
		if(t1== record && (t2==integer||t2==real)){
			p->type = record;
			strcpy(p->recordType,p->childast->recordType);
			return ;
		}
		if(t2== record && (t1==integer||t1==real)){
			p->type = record;
			strcpy(p->recordType,p->childast->siblingast->recordType);
			return ;
		}
		if(t1==record && t2==record){
			p->type=-1;
			return;
		}
	}
}



void printAst(astNode* p,astNode* p1){

    if(p==NULL) return;
	    
    int i;
    printf("%s %s %s %s %d\n",p->NodeSymbol,p->lexemeCurrentNode,p->valueIfNumber,p->recordType,p->type);
    printAst(p->childast,p);
    //printf("754\n");
    printAst(p->siblingast,p);
    //printf("756\n");
}

void semantics(){

	traverseAst1(astroot,NULL,"");
	traverseAst2(astroot,NULL,"");
	//printAst(astroot,NULL);
}