// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include"ast.h"



void setSentinel(treeNode* root){
	
    traverseTree1(root);
	traverseTree2(root,NULL);
	traverseTree3(astroot,NULL);
    traverseTree4(astroot,NULL);
    calculateASTsize(astroot,NULL);
}


void assignType(astNode *p, astNode *q){

    if(q->enumVal == TK_INT ){
        p->type = integer;
        strcpy(p->recordType,"");
        return;
    }
    if(q->enumVal == TK_REAL){
        p->type = real;
        strcpy(p->recordType,"");
        return;
    }
    if(q->enumVal == TK_RECORDID){
        q->type= record;
        p->type = record;
        strcpy(p->recordType,q->lexemeCurrentNode);
        return;  
    }
    if(p->enumVal == TK_FUNID){
        p->type = func;
        strcpy(p->recordType,"");
        return;  
    }
    if(p->enumVal == TK_NUM){
        p->type = integer;
        strcpy(p->recordType,"");
        return;
    }

    if(p->enumVal == TK_RNUM){
        p->type = real;
        strcpy(p->recordType,"");
    }
    
}



void shrinkTree(astNode *p){
    astNode * temp = NULL;
    astNode *temp1 = NULL;
    temp = p->childast->siblingast;         // sibling of the first child of declarations i.e declarations
    
    if(p->enumVal == declarations|| p->enumVal == moreFields || p->enumVal == typeDefinitions){
        p->childast = p->childast->childast->siblingast;
        temp1 = p->childast;
    }
    else{
        p->childast = p->childast->childast;    // the first declaration will point to int
        temp1 = p->childast->siblingast;        // temp1  points to sibling of int    
    }
    while(temp1->siblingast != NULL){       // linking of int id to declarations    
        temp1 =  temp1->siblingast;
    }
    temp1->siblingast = temp;
}

void traverseTree4(astNode* p,astNode * p1){

    if(p==NULL) return;
    int i;
    astNode *temp = NULL;
    astNode *temp1 = NULL;
    astNode *temp2 = NULL;
    
    if(p->enumVal == declarations || p->enumVal == otherFunctions 
        || p->enumVal == typeDefinitions|| p->enumVal==moreFields )
        shrinkTree(p);

    if(p->enumVal == otherFunctions && p1->enumVal == TK_END){      // removing other functions coming in btw by making TK_end of prev fun pt to next functions ID
        temp = p->childast;
        p1->siblingast = temp;
    }
    if(p->enumVal == parameter_list)
        p->childast = p->childast->siblingast;
    
    if(p->enumVal == parameter_list && p1->enumVal == TK_ID){
        temp = p->childast;
        p1->siblingast = temp;
    }

    if(p->enumVal == typeDefinitions && p1->enumVal == TK_ENDRECORD){
        temp = p->childast;
        p1->siblingast = temp;
    }
    if(p->enumVal == declarations && p1->enumVal == TK_ID || 
        p->enumVal == declarations && p1->enumVal == TK_GLOBAL|| p->enumVal == moreFields && p1->enumVal == TK_FIELDID){
        temp = p->childast;
        p1->siblingast = temp;
    }

    if(p->enumVal == otherStmts && p->parentast->enumVal == otherStmts){
        temp = p->parentast->childast;
  	    temp->siblingast = p->childast;
	}

    if(p->enumVal == fieldDefinitions){
        temp = p->childast;                 //toward first fieldDef
        temp1 = p->childast->siblingast;    // towards second fieldDef
        temp2 = temp1->siblingast;           // towrds morefields

        temp1->childast->siblingast->siblingast = temp2;    // the sibling of 2nd field ID pointing to morefields
        temp->childast->siblingast->siblingast = temp1->childast->siblingast;   // Linking of ID's of 2 fields
        p->childast = temp->childast->siblingast;       // towrds first fields ID
        
    }

    if(p->enumVal ==assignmentStmt){    // to create the tree of Arithmetic Expression
        //printf("466\n");
        astNode * temp = p->childast;   // single or rec id

        //printf("\n%s\n",temp->childast->lexemeCurrentNode);
        while(temp->enumVal!= arithmeticExpression){
            temp = temp->siblingast;
            //printf("%s\n",temp->NodeSymbol);
        }
        astNode * t= createTree(temp->childast);           // call to term 
        p->childast->siblingast->siblingast =t;
    }
    if(p->enumVal == conditionalStmt || p->enumVal == iterativeStmt){   // to create tree of Boolean Expression 
        astNode *temp = p->childast;
        //printf("%s\n", temp->siblingast->NodeSymbol);
        astNode* temp4 = temp->siblingast->siblingast;
        astNode *t = createBoolTree(temp->siblingast->childast);

        p->childast->siblingast = t;
        t->siblingast = temp4;
    }
    //printf("%s %s\n",p->NodeSymbol,p->lexemeCurrentNode);
    traverseTree4(p->childast,p);
    traverseTree4(p->siblingast,p);
}
astNode* createBoolTree(astNode *p){    // p points to boolean EXP child

    //astNode * temp = p->childast;

    if(p->enumVal == TK_ID||p->enumVal == TK_NUM||p->enumVal == TK_RNUM){
            
            astNode *a = (astNode *) malloc(sizeof(astNode));
            astNode *y = (astNode *) malloc(sizeof(astNode));
            astNode *x = (astNode *) malloc(sizeof(astNode));


            strcpy(x->lexemeCurrentNode,p->lexemeCurrentNode);  
            x->type = p->type;
            x->childast = NULL;
            x->siblingast = y;
            x->isLeafNode = 1;
            x->parentast = a;
            strcpy(x->NodeSymbol,p->NodeSymbol);
            strcpy(x->recordType,p->recordType);
            x->enumVal = p->enumVal;
            x->lineno = p->lineno;
            strcpy(x->valueIfNumber,p->valueIfNumber);

            strcpy(y->lexemeCurrentNode,p->siblingast->siblingast->lexemeCurrentNode);  
            y->type = p->siblingast->siblingast->type;
            y->childast = NULL;
            y->siblingast = NULL;
            y->isLeafNode = 1;
            y->parentast = a;
            y->lineno = p->siblingast->siblingast->lineno;
            strcpy(y->NodeSymbol,p->siblingast->siblingast->NodeSymbol);
            strcpy(y->recordType,p->siblingast->siblingast->recordType);
            y->enumVal = p->siblingast->siblingast->enumVal;
            strcpy(y->valueIfNumber,p->siblingast->siblingast->valueIfNumber);

            strcpy(a->lexemeCurrentNode,p->siblingast->lexemeCurrentNode);  
            a->type = p->siblingast->type;
            a->childast = x;
            a->siblingast = NULL;
            a->isLeafNode = 0;
            a->lineno = p->siblingast->lineno;
            strcpy(a->NodeSymbol,p->siblingast->NodeSymbol);
            strcpy(a->recordType,p->siblingast->recordType);
            a->enumVal = p->siblingast->enumVal;
            strcpy(a->valueIfNumber,p->siblingast->valueIfNumber);
            
            return a;
    }

    if(p->enumVal == booleanExpression){        // if expprime not NULL 
        astNode *a = (astNode *) malloc(sizeof(astNode));
        strcpy(a->lexemeCurrentNode,p->siblingast->lexemeCurrentNode);  
        a->type = p->siblingast->type;
        a->childast = createBoolTree(p->childast);
        a->childast->siblingast = createBoolTree(p->siblingast->siblingast->childast);
        a->siblingast =NULL;
        a->isLeafNode = 0;
        a->lineno = p->siblingast->lineno;
        strcpy(a->NodeSymbol,p->siblingast->NodeSymbol);
        strcpy(a->recordType,p->siblingast->recordType);
        a->enumVal = p->siblingast->enumVal;
        strcpy(a->valueIfNumber,p->siblingast->valueIfNumber);
        return a;
    }
    if(p->enumVal==TK_NOT){
        astNode *a = (astNode *) malloc(sizeof(astNode));
        strcpy(a->lexemeCurrentNode,p->lexemeCurrentNode);  
        a->type = p->type;
        a->childast = createBoolTree(p->siblingast->childast);
        a->childast->siblingast = NULL;
        a->siblingast =NULL;
        a->isLeafNode = 0;
        a->lineno = p->lineno;
        strcpy(a->NodeSymbol,p->NodeSymbol);
        strcpy(a->recordType,p->recordType);
        a->enumVal = p->enumVal;
        strcpy(a->valueIfNumber,p->valueIfNumber);
        return a;
    }
}
astNode* createTree(astNode *p){    // p points to term

    //printf("%s\n",p->NodeSymbol );
    if(p->enumVal == TK_ID||p->enumVal == TK_NUM||p->enumVal == TK_RNUM){
        if(p->siblingast == NULL){  // for normal identifiers
            astNode *x = (astNode *) malloc(sizeof(astNode));
            strcpy(x->lexemeCurrentNode,p->lexemeCurrentNode);  
            x->type = p->type;
            x->childast = NULL;
            strcpy(x->NodeSymbol,p->NodeSymbol);
            x->siblingast = NULL;
            x->isLeafNode = 1;
            x->lineno = p->lineno;
            strcpy(x->recordType,p->recordType);
            x->enumVal = p->enumVal;
            strcpy(x->valueIfNumber,p->valueIfNumber);
            return x;
        }
        else{   // for the field ID's
            astNode *x = (astNode *) malloc(sizeof(astNode));
            astNode *y = p;
            //printf("514\n");
            char fieldName[40] ;
            strcpy(fieldName,y->lexemeCurrentNode);
            y = y->siblingast;
            int t = 0;
            while(y!= NULL){
                strcat(fieldName,y->lexemeCurrentNode);
                t = y->type;
                x->enumVal = y->enumVal;
                y = y->siblingast;
            } 
            //printf("%s\n",alphabet[x->enumVal]);
            strcpy(x->lexemeCurrentNode,fieldName); // x points to operator
            x->isLeafNode = 1;
            x->childast = NULL;
            strcpy(x->NodeSymbol,p->NodeSymbol);
            x->siblingast = NULL;
            x->lineno = p->lineno;

            //strcpy(x->recordType,p->recordType);
            //printf("317%s\n",x->recordType);
            strcpy(x->valueIfNumber,p->valueIfNumber);
            //printf("%s\n",x->valueIfNumber );
            return x;   
        }
    }
    if(p->siblingast != NULL){      // if expprime not NULL 

        astNode *x = (astNode *) malloc(sizeof(astNode));
        strcpy(x->lexemeCurrentNode,p->siblingast->childast->lexemeCurrentNode);    // x points to operator
        x->childast = createTree(p->childast);  //  first child to term child
        x->childast->siblingast = createTree(p->siblingast->childast->siblingast); // second child to expprime
        x->isLeafNode = 0;
        x->siblingast = NULL;
        x->lineno = p->siblingast->childast->lineno;
        strcpy(x->NodeSymbol,p->siblingast->childast->NodeSymbol);

        x->enumVal = p->siblingast->childast->enumVal;
        return x;
    }

    else{
        return(createTree(p->childast));
    }
}



void traverseTree3(astNode* p, astNode* p1){    //to remove nodes pointing to eps

	if(p==NULL) return;

    if(p->enumVal == TK_ID || p->enumVal == TK_FUNID || p->enumVal == TK_FIELDID|| p->enumVal == TK_NUM|| p->enumVal == TK_RNUM)
        assignType(p,p1);

	if(p->enumVal==eps){
		while(p->parentast->numChild==1 ){
			p = p->parentast;
	}
		int s = p->enumVal;
		astNode* next = p->parentast->childast;
		astNode* prev = next;


		if(next->enumVal==s){
		next->parentast->childast=next->siblingast; 
			return;
		}
		else{
			while(next->enumVal != s) {
				prev = next; 
				next = next->siblingast;
			}
			prev->siblingast = next->siblingast;
			return;
		}	
	}	

	traverseTree3(p->childast,p);
    traverseTree3(p->siblingast,p);
}

astNode* createNode (astNode* parent, treeNode * current){      // to create a new Node

    astNode* temp = (astNode*)malloc(sizeof(astNode));
    temp->parentast = parent;
    strcpy(temp->NodeSymbol,current->NodeSymbol); 
    temp->enumVal = current->enumVal;
    temp->childast=NULL;
    temp->siblingast = NULL;
    temp->type = -1;
    temp->lineno = current->lineno;
    strcpy(temp->lexemeCurrentNode,current->lexemeCurrentNode);
    strcpy(temp->valueIfNumber,current->valueIfNumber);
    return temp;
}

void traverseTree2(treeNode* p,astNode* presentPtr){        // to remove the nodes marked as sentinel

    int count=0;
    int i;
    for(i=0;i<p->numChild;i++){
        if(p->child[i]->enumVal ==-1 && p->child[i]->isLeafNode==1)
	      	count++;
    }
    

    if(p->enumVal==program){
        astroot = presentPtr = createNode(NULL,p);
    }

    count = p->numChild - count;

    presentPtr->childast = NULL;
    presentPtr->numChild=count;

    i=0;
    int j=0;                                //j is for count of children in AST 
    while(j < count && i< p->numChild){

    	if(p->child[i]->enumVal !=-1) {								////// node is a valid node

            astNode* temp = createNode(presentPtr,p->child[i]);
  
        		if(presentPtr->childast== NULL)	
        			presentPtr->childast = temp;
        		
                else{
        			astNode* y = presentPtr->childast;
        			while(y->siblingast != NULL)
        				y = y->siblingast;
        			y->siblingast = temp;
        		}

        		if(p->child[i]->isLeafNode){
        			 temp->isLeafNode=1;
        			}

        		else{
        			temp->isLeafNode=0;
        			traverseTree2(p->child[i],temp);
        		}

        		j++;
    	}

    	else{														////// invalid nodes
            
            //printf("%s\n",p->child[i]->NodeSymbol);
    		
            if(p->child[i]->isLeafNode==0){
                
                treeNode* x = p->child[i];
                astNode* temp1 = NULL;
                int total = x->numChild;
                int k = 0;
                
                while(k<total){

                    if(x->child[k]->enumVal != -1){
                            
                            temp1 = createNode(presentPtr,x->child[k]);
                            
                            if(presentPtr->childast== NULL) 
                                presentPtr->childast = temp1;
                    
                            else{
                                astNode* y = presentPtr->childast;
                                while(y->siblingast != NULL)
                                    y = y->siblingast;
                                y->siblingast = temp1;
                            }

                            if(x->child[k]->isLeafNode) 
                                temp1->isLeafNode=1;
                            
                            else{
                                temp1->isLeafNode=0;
                                traverseTree2(x->child[k],temp1);
                            }
                            k++;
                    }
                    
                    else if(k == total-1 && x->child[k]->isLeafNode==0){
                        //printf("92\n");
                        x = x->child[k];
                        total = x->numChild;
                        k = 0;
                    }
                    
                    else
                        k++;
                }

            j++;
        }
    }

    i++;
    	
    }

}

void traverseTree1(treeNode* p){            // marking the nodes that are not required

    int i;
    
    for(i=0;i<p->numChild;i++){
        traverseTree1(p->child[i]);
    }
    if(p->isLeafNode==1){
    	if(binarySearchT(p->enumVal)!=-1) {
    		p->enumVal=-1;	
    		return;
    	}
    }
   	
    if(binarySearchNT(p->enumVal)!=-1) p->enumVal=-1;	
    //if(strcmp(p->NodeSymbol,"eps") ==0) p->enumVal=-1;
}

void printAST(astNode* p,astNode* p1){

    if(p==NULL) return;

    printf("%-10d %-20s %-20s \n",p->lineno,p->NodeSymbol,p->lexemeCurrentNode);
    
    //printf("%s %s \n", p->NodeSymbol,p->lexemeCurrentNode);
    printAST(p->childast,p);
    printAST(p->siblingast,p);
}
void calculateASTsize(astNode* p,astNode* p1){

    if(p==NULL) return;
    int i;

    if(p->enumVal == idList && p1->enumVal == TK_ID){
        
        astNode *temp;
        temp = p->childast;
        p1->siblingast = temp;
    }
    if(p->enumVal == new_24 && p1->enumVal == TK_ID){
        
        astNode *temp;
        temp = p->childast;
        p1->siblingast = temp;
    }



    else{
        astSize += sizeof(p);
        astNodes +=1;
        //printf("%-10d %-20s %-20s \n",p->lineno,p->NodeSymbol,p->lexemeCurrentNode);
    }
    //printf("%s %s \n", p->NodeSymbol,p->lexemeCurrentNode);
    calculateASTsize(p->childast,p);
    calculateASTsize(p->siblingast,p);
}

void printRatios(){

    printf("Parse Tree:\t Number of Nodes=%d\t Allocated Memory=%d Bytes\n",parseNodes,parseTreeSize );
    printf("Abstract Stntax Tree:\t Number of Nodes=%d\t Allocated Memory=%d Bytes\n",astNodes,astSize );

    float ratio = (((float)parseTreeSize-astSize)/parseTreeSize)*100;
    printf("Compression Percentage = %f%%\n",ratio);

}

int binarySearchT(int key){
	int i=0;
	int low=0,high=15;
	int mid;
	

	while(low<=high){
		mid = (low+high)/2;		
		if(key == termNT[mid])
			return mid;
		else if(key < termNT[mid]) high=mid-1;
		else{
			low=mid+1;
		}
	}
	return -1;
}

int binarySearchNT(int key){
	int i=0;
	int low=0,high=20;
	int mid;
	

	while(low<=high){
		mid = (low+high)/2;		
		if(key == ntermNT[mid])
			return mid;
		else if(key < ntermNT[mid]) high=mid-1;
		else{
			low=mid+1;
		}
	}
	return -1;
}