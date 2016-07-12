// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P
#include "symbolTable.h"


int hash(char a[]){
	 int sum=0;
	 int i=0;
	 for(i=0;i<strlen(a);i++){
	 	sum += a[i];
	 }
	 return sum%size;
}


entry* createNewNode(astNode* p){		// to fill the entry corresponding to chained entry and returns the entry

	entry* x = (entry*)malloc(sizeof(entry));
	strcpy(x->name,p->lexemeCurrentNode);
	x->type = p->type;
	x->offset = 0;
	x->occupied = 1;
	x->iopar =0;
	strcpy(x->recordType,p->recordType);

	if(x->type == integer)
		x->width = 2;
	else if(x->type == real)
		x->width = 4;
	else if(x->type == record){
		
		int i = hash(x->recordType);
		
		if(recST[i].occupied){
			if(strcmp(recST[i].name,x->recordType)==0)
				x->width = recST[i].width;
			else{
				entry * next=recST[i].head;
				while(strcmp(x->recordType,next->name)!=0)
					next = next->head;
				if(next!=NULL)
					x->width = next->width;
				else{
					printf("lineno %d: ERROR: The record %s is undefined. \n",p->lineno,x->recordType);
					semanticError =1;
					x->width = 0;
					x->occupied=0;	
				}
			}
		}
		else{
			printf("lineno %d: ERROR: The record %s is undefined. \n",p->lineno,x->recordType);
			semanticError =1;
			x->width = 0;
			x->occupied=0;
		}	
	}
	else
		x->width = 0;

	return x;
}

void fillEntry(astNode *p,entry a[],int index){		// if the table entry for a corresponding to index is not occupied

	strcpy(a[index].name,p->lexemeCurrentNode);
	//printf("%s %s\n",p->lexemeCurrentNode,p->recordType);
	a[index].type = p->type;
	a[index].occupied = 1;
	strcpy(a[index].recordType,p->recordType);
	a[index].iopar = 0;

	if(a[index].type == integer)
		a[index].width = 2;
	else if(a[index].type == real)
		a[index].width = 4;
	else if(a[index].type == record){
		int i = hash(a[index].recordType);
		if(recST[i].occupied){
			if(strcmp(recST[i].name,a[index].recordType)==0)
				a[index].width = recST[i].width;
			else{
				entry * next=recST[i].head;
				while(strcmp(a[index].recordType,next->name)!=0)
					next = next->head;
				a[index].width = next->width;
			}
		}
		else{
			printf("lineno %d: ERROR: The record %s is undefined. \n",p->lineno,a[index].recordType);
			semanticError =1;
			a[index].width = 0;
			a[index].occupied=0;
		}
	}
	else
		a[index].width =0;
}

int computeWidth(entry a[]){
	int i;
	int sum =0;
	for(i=0;i<size;i++){
		if(a[i].occupied){
			if(a[i].head == NULL)
				sum = sum + a[i].width;
			else{
					sum = sum + a[i].width;
					entry * x = a[i].head;
					while (x!= NULL){
						sum += x->width;
						x = x->head;
					}
			}
		}
	}
	return sum;
}

void traverseAST(astNode* p,astNode* q,entry* symbolT,int offset)	// to enter records entry and global variables
{
	int width=0;
	if(p==NULL) return;
	int index;
	entry * next;
	entry *prev;
	astNode* temp1=NULL;
	
	if(p->enumVal == declarations){
		return;
	}

	if(p->enumVal == TK_RECORDID && p->parentast->enumVal != parameter_list){
		//printf("Entereing here\n");
		//printf("114 %s %s\n",p->lexemeCurrentNode,q->NodeSymbol);
		//printf("115 %s %s\n",p->lexemeCurrentNode,p->parentast->NodeSymbol);

		entry* recSym = (entry*) malloc(sizeof(entry)*size);		// separate table for record
		
		index = (int)hash(p->lexemeCurrentNode);		// finding index for the record name


		if(recST[index].occupied==1){
			entry* next;
			entry* prev;

			if(strcmp(p->lexemeCurrentNode,recST[index].name)==0){		// checking if Record already Declared
				printf("lineno %d: ERROR Redeclaration of Record %s\n",p->lineno,recST[index].name);
				semanticError = 1;
			}

			else{				// if recST[index] is occupied then chain
				next = recST[index].head;
				prev = &(recST[index]);
				while(next != NULL){			//looping through the chain
					if(strcmp(p->lexemeCurrentNode,next->name)==0){		// checking if record already present in chain
						printf("lineno %d: ERROR Redeclaration of record %s\n",p->lineno,next->name);
						semanticError;
						break;
					}
					prev = next;
					next = next->head;
				}

				if(next == NULL){
					entry* temporary = next;
					entry* newNode = createNewNode(p);			// creates a new entry node
					newNode->type = -1;							//setting type of RecordId as -1
					newNode->next = recSym;						// points to records symbol table
					newNode->head = temporary;					// making head pointer NULL	
					newNode->offset =0;
					traverseAST(p->siblingast,p,recSym,0);		// to fill the table of record
					newNode->width= computeWidth(recSym);
					//globalOffset = globalOffset + newNode->width;
					prev->head = newNode;
				}
			}
		}
		else{			// if entry in symbol table not occupied
				fillEntry(p,recST,index);
				recST[index].next = recSym;
				recST[index].type = -1;
				recST[index].offset = 0;
				traverseAST(p->siblingast,p,recSym,0);
				recST[index].width = computeWidth(recSym);
			}
			//printf("leaving\n");
	
		while(p->enumVal !=TK_ENDRECORD)		//to move onto next record 
			p = p->siblingast;
	}


	if(p->enumVal == fieldDefinitions){
		int width =0;
		astNode* temp2 = p->childast;		// points to FieldID's
		while(temp2 != NULL){
				index = (int)hash(temp2->lexemeCurrentNode);
				if(symbolT[index].occupied==1){
						prev = &(symbolT[index]);
						next = symbolT[index].head;
						while(next != NULL){			//looping through the chain
							if(strcmp(temp2->lexemeCurrentNode,next->name)==0){		// checking if field already present in chain
								printf("lineno %d: ERROR Redeclaration of Field %s\n",p->lineno,next->name);
								semanticError =1;
								break;
							}
							prev = next;
							next = next->head;
						}
						if(next == NULL){
							entry* newNode = createNewNode(temp2);
							newNode->head = next;
							newNode->offset = offset;
							width = newNode->width;
							prev->head = newNode;
						}
				}
				else{
					fillEntry(temp2,symbolT,index);
					symbolT[index].offset = offset;
					width = symbolT[index].width;
				}
				offset = offset + width;
				temp2 = temp2->siblingast;
		}
		return;
	}

	traverseAST(p->childast,p,symbolT,offset);
	traverseAST(p->siblingast,p,symbolT,offset);
}

void traverseAST2(astNode* p,astNode *q, entry *symbolT,int offset,char scope[30]){

	if(p==NULL) return;
	//printf("%s\n",p->NodeSymbol);
	int index;
	entry * next;
	entry *prev;
	astNode* temp1=NULL;
	
	if(p->enumVal == declarations){			// now we are inserting variables in the Symbol Table
		//printf("Entering\n");
		//printf("215 %s\n",p->siblingast->NodeSymbol);
		temp1 = p->childast;			// temp1 points to the declarations i.e Tk_Id

		int width;

		while(temp1 != NULL){
			//printf("221 %s\n",temp1->lexemeCurrentNode);
			if(temp1->enumVal == TK_ID){
				//printf("221 %s\n",p->lexemeCurrentNode);
				//printf("%s\n",temp1->lexemeCurrentNode );
				index = (int)hash(temp1->lexemeCurrentNode);
			if(temp1->siblingast !=NULL && temp1->siblingast->enumVal == TK_GLOBAL){		//checking for global variable
					
					//printf("%s %s\n",temp1->NodeSymbol,temp1->lexemeCurrentNode);

					if(globalST[index].occupied==1){				//if entry is already occupied then chaining
						
						
						if(strcmp(temp1->lexemeCurrentNode,globalST[index].name)==0){
							printf("lineno %d: ERROR Redeclaration of global variable %s\n",temp1->lineno,globalST[index].name);
							semanticError =1;
						}

						else{					// chaining of Nodes
							next = globalST[index].head;
							prev = &(globalST[index]);	
							while(next != NULL){
								if(strcmp(temp1->lexemeCurrentNode,next->name)==0){
									printf("lineno %d: ERROR Redeclaration of global varaible %s\n",temp1->lineno,globalST[index].name);
									break;
								}
								prev = next;
								next = next->head;
							}

							if(next != NULL){	// if variable redeclared ignore it and move onto next one
								temp1 = temp1->siblingast;
								continue;
							}

							entry* newNode = createNewNode(temp1);
							width = newNode->width;
							newNode->offset =  globalOffset;
							globalOffset = globalOffset + width; 
							newNode->head = next;
							prev->head = newNode;
						}

					}

					else{										// if entry not occupied then directly entering the result
						fillEntry(temp1,globalST,index);
						width = globalST[index].width;
						globalST[index].offset =  globalOffset;
						globalOffset = globalOffset + width; 
					}	
				}
			
			else{			// if not global then make entry
					if(symbolT[index].occupied==1){

						//printf("%s\n",temp1->lexemeCurrentNode );
						
						if(strcmp(temp1->lexemeCurrentNode,symbolT[index].name)==0){
							printf("lineno %d: ERROR Redeclaration of variable %s\n",temp1->lineno,symbolT[index].name);
							semanticError = 1;
						}
						else{
							next = symbolT[index].head;
							prev = &(symbolT[index]);				
							while(next != NULL){
								if(strcmp(temp1->lexemeCurrentNode,next->name)==0){
									printf("lineno %d: ERROR Redeclaration of varaible %s\n",temp1->lineno,next->name);
									break;
								}
								prev = next;
								next = next->head;
							}

							if(next != NULL){
								temp1 = temp1->siblingast;
								continue;
							}

							
							entry* newNode = createNewNode(temp1);			// new node created
							width = newNode->width;
							newNode->offset =  offset;
							offset = offset + width;
							newNode->head = next;						// new nodes headptr points to NULL
							prev->head = newNode;
						}
					
					}

					else{			// directly make an entry for the variable in the Symbol Table
						//printf("%s\n", temp1->lexemeCurrentNode);
						fillEntry(temp1,symbolT,index);
						symbolT[index].type = temp1->type;
						width = symbolT[index].width;
						symbolT[index].offset =  offset;
						offset = offset + width; 
					}
				}
			}
			temp1 = temp1->siblingast;
		}

		//return;
	}

	if(p->enumVal == parameter_list){		// insering parameterList into functions ST
		int width;
		temp1 = p->childast;
		while(temp1 != NULL){
				
			if(temp1 ->enumVal == TK_ID){
				//printf("\n%s\n",temp1->lexemeCurrentNode);

				index = (int)hash(temp1->lexemeCurrentNode);
				//printf("%s\n",temp1->lexemeCurrentNode);

				if(symbolT[index].occupied==1){			//checking for chaining
						
					next = symbolT[index].head;
					prev = &(symbolT[index]);

					while(next != NULL){
						prev = next;
						next = next->head;
					}
					
					entry* newNode = createNewNode(temp1);			// new node created
					width = newNode->width;
					newNode->offset =  offset;
					offset = offset + width;
					newNode->head = next;						// new nodes headptr points to NULL
					prev->head = newNode;

					if(q->enumVal == TK_FUNID) 
						newNode->iopar = input;	
					if(q->enumVal == parameter_list)
						newNode->iopar = output;
						
					symbolT[index].head = newNode;
				}

				else{
						
					fillEntry(temp1,symbolT,index);
					width = symbolT[index].width;
					symbolT[index].offset =  offset;
					offset = offset + width; 
					if(q->enumVal == TK_FUNID)
						symbolT[index].iopar = input;	
					if(q->enumVal == parameter_list)
						symbolT[index].iopar = output;
						
				}
			}
			
			temp1 = temp1->siblingast;
		}
	}

	if(p->enumVal == TK_FUNID && p->parentast->enumVal != funCallStmt || p->enumVal == TK_MAIN){

		// printf("Entering\n");
		// printf("%s\n",p->lexemeCurrentNode);
		if(p->enumVal != TK_MAIN)
		{
		entry* funSym = (entry*) malloc(sizeof(entry)*size);		// to create a separate table for function

		index = (int)hash(p->lexemeCurrentNode);

		if(funST[index].occupied==1){			// if sybol table enry for func occupied
			if(strcmp(p->lexemeCurrentNode,funST[index].name)==0){
				semanticError =1;
				printf("lineno %d: ERROR Redefinition of Function %s() is not allowed\n",p->lineno,p->lexemeCurrentNode);
			}

			else{

				next = funST[index].head;
				prev = &(funST[index]);
				while(next != NULL){
					if(strcmp(p->lexemeCurrentNode,next->name)==0){
						semanticError =1;
						printf("lineno %d: ERROR Redefinition of Function %s() is not allowed\n",p->lineno,next->name);
						break;
					}
					prev = next;
					next = next->head;
				}
				if(next == NULL){
					entry* newNode = createNewNode(p);
					newNode->type = -1;
					newNode->next = funSym;
					newNode->head = next;
					newNode->offset =0;
					traverseAST2(p->siblingast,p,funSym,0,p->lexemeCurrentNode);
					newNode->width= computeWidth(funSym);
					//globalOffset = globalOffset + newNode->width;
					prev->head = newNode;
				}
			}
		}
		else{			// if symbol table entry is vacant
			fillEntry(p,funST,index);
			funST[index].next = funSym;
			funST[index].offset = offset;
			traverseAST2(p->siblingast,p,funSym,0,p->lexemeCurrentNode);
			funST[index].width = computeWidth(funSym);
		}
		//printf("leaving\n" );
		return;

		}
		else{
			traverseAST2(p->childast,p,symbolTable,0,"_main");
			traverseAST2(p->siblingast,p,symbolTable,0,"_main");
			return;
		}
	}

	if(p->enumVal == funCallStmt && strcmp(scope,"_main")!=0 && strcmp(scope,"")!=0){
		
		//printf("%s\n",scope);
		astNode* temp  = p->childast;	// points to output par if present.
		while(temp->enumVal != TK_FUNID)
			temp = temp->siblingast;
		
		//printf("%s\n",temp->lexemeCurrentNode );

		int i = hash(temp->lexemeCurrentNode);	// temp points to TK_FUNID
		
		if(funST[i].occupied){					// checking for functions ENTRY IN FunST.
			
			if(strcmp(funST[i].name,temp->lexemeCurrentNode)!=0){
				next = funST[i].head;
				while(next!=NULL && strcmp(next->name,temp->lexemeCurrentNode)!=0 )
					next = next->head;
				if(next ==NULL){
					semanticError =1;
					printf("lineno %d: ERROR: The function %s() is undefined.\n",temp->lineno,temp->lexemeCurrentNode);
					p->type = 5;
				}
			}
			else{
				if(strcmp(scope,temp->lexemeCurrentNode)==0){
					p->type = 5;
					semanticError =1;
					printf("lineno %d: ERROR:In %s() Recursive call to a function not allowed.\n",temp->lineno,temp->lexemeCurrentNode);
				}
			}
		}
		
		else{	
				semanticError =1;
				printf("lineno %d: ERROR: The function %s() is undefined.\n",temp->lineno,temp->lexemeCurrentNode);
			}
	}
	
	if(p->enumVal == funCallStmt && strcmp(scope,"_main")==0){
		//printf("%s\n",scope);
		astNode* temp  = p->childast;	// points to output par if present.
		while(temp->enumVal != TK_FUNID)
			temp = temp->siblingast;
		int i = hash(temp->lexemeCurrentNode);	// temp points to TK_FUNID
		
		if(funST[i].occupied){					// checking for functions ENTRY IN FunST.
			if(strcmp(funST[i].name,temp->lexemeCurrentNode)!=0){
				next = funST[i].head;
				while(next!=NULL && strcmp(next->name,temp->lexemeCurrentNode)!=0 )
					next = next->head;
				if(next ==NULL){
					semanticError =1;
					printf("lineno %d: ERROR: The function %s() is undefined.\n",temp->lineno,temp->lexemeCurrentNode);
					p->type = 5;
				}
			}
		}
		else{
				semanticError =1;
				printf("lineno %d: ERROR: The function %s() is undefined.\n",temp->lineno,temp->lexemeCurrentNode);
			}
	}

	if(p->enumVal == TK_END){
		if(p->siblingast!=NULL)
			strcpy(scope,p->siblingast->lexemeCurrentNode);
	}

	traverseAST2(p->childast,p,symbolT,offset,scope);
	traverseAST2(p->siblingast,p,symbolT,offset,scope);
}

char * typeGen (entry st[]){

	int i=0;
	char  t[30] = "";
	char * y;
	while(i< size){

		if(st[i].occupied){
			char a[5];
			//printf("%s\n",st[i].name);
			if(st[i].type == integer)
				strcpy(a,"int");
			else 
				strcpy(a,"real");
								
			if(strcmp(t,"")==0)
				strcpy(t,a);
			else
				strcat(strcat(t,"x"),a);

			entry* temp = st[i].head;
			
			while(temp != NULL){
			
			if(temp->type == integer)
				strcpy(a,"int");
			else 
				strcpy(a,"real");
								
			if(strcmp(t,"")==0)
				strcpy(t,a);
			else
				strcat(strcat(t,"x"),a);
			
				temp = temp->head;
			}

			//printf("%s\n", t);			
		}

		i++;
	}
	y= t;
return y;
}

void printST(entry st[], char a[]){

	if(st==NULL)return;
	
	int i=0;
	
	while(i< size){

		if(st[i].occupied){
			//printf("%s\n",st[i].name);
				char*  t;
				char b[30];
				if(st[i].type == integer){
					strcpy(b,"int");
				}
				else if(st[i].type == real)
					strcpy(b,"real");
				else{
					
					if(st[i].type == record){
					int index = hash(st[i].recordType);
					t = typeGen(recST[index].next);
					strcpy(b,t);
					}
					
				}

				printf("name = %s 	type = %s 	scope = %s 	offset = %d\n", st[i].name,b,a,st[i].offset);
		
			entry* temp = st[i].head;
			
			while(temp != NULL){
				if(temp->type == integer)
					strcpy(b,"int");
				else if(temp->type == real)
					strcpy(b,"real");
				else{
					int index = hash(temp->recordType);
					t = typeGen(recST[index].next);
					strcpy(b,t); 
				}
				printf("name = %s 	type = %s 	scope = %s 	offset = %d\n", temp->name,b,a,temp->offset);
				temp = temp->head;
			}
						
		}
		i++;
	}
printf("\n");
}


void fillSymbolTable(){
	
	traverseAST(astroot,NULL,symbolTable,0);
	traverseAST2(astroot,NULL,symbolTable,0,"");
}

void printSymbolTable(){
	printf("\n");
	printST(globalST,"global");
	printST(symbolTable,"_main");
	
	int i;
	for(i=0;i<size;i++){
		if(funST[i].occupied){
			printST(funST[i].next,funST[i].name);
			entry * temp = funST[i].head;
			while(temp != NULL){
				printST(temp->next,temp->name);
				temp = temp->head;
			}
		}
	}
}
