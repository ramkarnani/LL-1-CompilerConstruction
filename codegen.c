// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P

#include "codegen.h"



void declarationCode(astNode* p){
	
	astNode* temp = p;
	int width=0;
	fprintf(asmFile, "	section .bss\n\n");

	while(temp!= NULL){
		if(temp->enumVal ==TK_ID){
			
			if(temp->type ==0){
				fprintf(asmFile, "	%s:\tresb\t8\n",temp->lexemeCurrentNode);
			}
			if(temp->type==2){
				
				int index = hash(temp->recordType);
				if(recST[index].occupied){
					
					if(strcmp(temp->recordType,recST[index].name)==0)
						width = recST[index].width;

					else{
						entry *temp1 = recST[index].head;
						while((temp1->name,temp->recordType)!=0)
							temp1= temp1->head;
						width = temp1->width;
					}
					
				}
				fprintf(asmFile, "	%s:\tresb\t%d\n",temp->lexemeCurrentNode,(width/2)*8);						
			}
		}
		temp = temp->siblingast;
	}


}


void arithmeticCode(astNode* p){

	int width;
	if(p->enumVal == TK_NUM){
		int num = atoi(p->valueIfNumber);
		fprintf(asmFile, "	mov	rax,%d\n",num);
		return;
	}
	if(p->enumVal == TK_ID){

		
		if(p->type==0){
			fprintf(asmFile, "	mov	rax,[%s]\n",p->lexemeCurrentNode);
		}
		if(p->type==2){
			int index = hash(p->lexemeCurrentNode);
			if(strcmp(p->lexemeCurrentNode,symbolTable[index].name)==0)
				width = symbolTable[index].width;
			else{
				entry *temp1 = symbolTable[index].head;
				while((temp1->name,p->lexemeCurrentNode)!=0)
					temp1= temp1->head;
				width = temp1->width;
			}
			width = width /2;
			
			int i=0;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rax,[%s+%d]\n",p->lexemeCurrentNode,i*8);
				fprintf(asmFile, "	mov	[temp+%d],rax\n",i*8);
			}
		}
		return;

	}

	if(p->enumVal == TK_FIELDID){

		
		int offset =0;
		entry* st;

		char str[40];
		strcpy(str,p->lexemeCurrentNode);
		char* str1 = strtok(str,".");
		
		char rec[30];
		char field[30];
		char recName [30];
		strcpy(rec,str1);
		str1 = strtok(NULL,".");
		strcpy(field,str1);
				
		int index = hash(rec);
		if(strcmp(rec,symbolTable[index].name)==0)
			strcpy(recName,symbolTable[index].recordType);
		else{
			entry *temp1 = symbolTable[index].head;
			while((temp1->name,rec)!=0)
				temp1= temp1->head;
			strcpy(recName,temp1->recordType);
		}
		
		index = hash(recName);
		if(strcmp(recName,recST[index].name)==0)
			st = recST[index].next;
		else{
			entry *temp1 = recST[index].head;
			while((temp1->name,recName)!=0)
				temp1= temp1->head;
			st = temp1->next;
		}
		
		index = hash(field);
		if(strcmp(field,st[index].name)==0)
			offset = st[index].offset;
		else{
			entry *temp1 = st[index].head;
			while((temp1->name,field)!=0)
				temp1= temp1->head;
			offset = temp1->offset;
		}
		
		offset = offset /2;

		fprintf(asmFile, "	mov	rax,[%s+%d]\n",rec,offset*8 );
		return;
	}

	arithmeticCode(p->childast);

		
	if(p->childast->type ==0)
		fprintf(asmFile, "	push	rax\n");
	else{

		int index = hash(p->childast->lexemeCurrentNode);
		if(strcmp(p->childast->lexemeCurrentNode,symbolTable[index].name)==0)
			width = symbolTable[index].width;
		else{
			entry *temp1 = symbolTable[index].head;
			while((temp1->name,p->childast->lexemeCurrentNode)!=0)
				temp1= temp1->head;
			width = temp1->width;
		}
		width = width /2;

		int i=0;
		for(i=0;i<width;i++){
			fprintf(asmFile, "	push	qword[temp+%d]\n",i*8);
		}
		fprintf(asmFile, "\n");

	}

	//if left is record, value in temp.. and else in temp1
	arithmeticCode(p->childast->siblingast);

	if(p->childast->siblingast->type==0)
		fprintf(asmFile, "	mov	rbx,rax\n");
	else{

		int index = hash(p->childast->siblingast->lexemeCurrentNode);
		if(strcmp(p->childast->siblingast->lexemeCurrentNode,symbolTable[index].name)==0)
			width = symbolTable[index].width;
		else{
			entry *temp1 = symbolTable[index].head;
			while((temp1->name,p->childast->siblingast->lexemeCurrentNode)!=0)
				temp1= temp1->head;
			width = temp1->width;
		}
		width = width /2;

		int i;
		for(i=0;i<width;i++){
			fprintf(asmFile, "	mov	rax,[temp+%d]\n",i*8);
			fprintf(asmFile, "	mov	[temp1+%d],rax\n",i*8);	
		}
	}

	
	if(p->childast->type==0)
		fprintf(asmFile, "	pop		rax\n");
	else{

		int i=0;
		for(i=0;i<width;i++){
			fprintf(asmFile, "	pop		qword[temp+%d]\n",(width-i-1)*8);
		}
		fprintf(asmFile, "\n");

	}

	if(p->enumVal==TK_PLUS){
		if(p->childast->type==0)
			fprintf(asmFile, "	add	rax,rbx\n");
		else{

			int i=0;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rax,[temp+%d]\n", i*8);
				fprintf(asmFile, "	add	rax,[temp1+%d]\n", i*8);
				fprintf(asmFile, "	mov	[temp+%d],rax\n", i*8);
			}

		}


		return;
	}

	if(p->enumVal==TK_MINUS){
		if(p->childast->type==0)
			fprintf(asmFile, "	sub	rax,rbx\n");
		else{

			int i=0;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rax,[temp+%d]\n", i*8);
				fprintf(asmFile, "	sub	rax,[temp1+%d]\n", i*8);
				fprintf(asmFile, "	mov	[temp+%d],rax\n", i*8);
			}

		}


		return;
	}

	if(p->enumVal==TK_MUL){

		if(p->childast->type==0 && p->childast->siblingast->type==0)
			fprintf(asmFile, "	mul	rbx\n");
		else if(p->childast->type==0 && p->childast->siblingast->type==2){
			int i;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rbx,[temp1+%d]\n",i*8);
				fprintf(asmFile, "	mul	rbx\n");
				fprintf(asmFile, "	mov	[temp+%d],rax\n",i*8);
			}
		}
		else{
			int i;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rax,[temp+%d]\n",i*8);
				fprintf(asmFile, "	mul	rbx\n");
				fprintf(asmFile, "	mov	[temp+%d],rax\n",i*8);
			}
		}

		return;
	}
	
	if(p->enumVal==TK_DIV){

		if(p->childast->type==0 && p->childast->siblingast->type==0){
			fprintf(asmFile, "	mov	rdx,0\n");
			fprintf(asmFile, "	div	rbx\n");
		}
		else if(p->childast->type==0 && p->childast->siblingast->type==2){
			int i;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rbx,[temp1+%d]\n",i*8);
				fprintf(asmFile, "	mov	rdx,0\n");
				fprintf(asmFile, "	div	rbx\n");
				fprintf(asmFile, "	mov	[temp+%d],rax\n",i*8);
			}
		}
		else{
			int i;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rax,[temp+%d]\n",i*8);
				fprintf(asmFile, "	mov	rdx,0\n");
				fprintf(asmFile, "	div	rbx\n");
				fprintf(asmFile, "	mov	[temp+%d],rax\n",i*8);
			}
		}

		return;
	}


}


void assignmentCode(astNode* p){

	//p is pointing to singleOrRecId
	char assignedVar[30];

	arithmeticCode(p->siblingast->siblingast);


	if(p->childast->siblingast==NULL){

		strcpy(assignedVar,p->childast->NodeSymbol);

		if(p->childast->type==0){

			fprintf(asmFile, "	mov	[%s],rax\n",p->childast->lexemeCurrentNode);
		
		}
		if(p->childast->type==2){
			
			int width;
			int index = hash(p->childast->lexemeCurrentNode);
			if(strcmp(p->childast->lexemeCurrentNode,symbolTable[index].name)==0)
				width = symbolTable[index].width;
			else{
				entry *temp1 = symbolTable[index].head;
				while((temp1->name,p->childast->lexemeCurrentNode)!=0)
					temp1= temp1->head;
				width = temp1->width;
			}
			width = width /2;
			//printf("67%d\n",width);
			int i=0;
			for(i=0;i<width;i++){
				fprintf(asmFile, "	mov	rax,[temp+%d]\n",i*8);
				fprintf(asmFile, "	mov	[%s+%d],rax\n",p->childast->lexemeCurrentNode,i*8);
			}
			fprintf(asmFile, "\n");
		}
	}
	else{

		//printf("93\n");
		int offset =0;
		entry* st;
		int index = hash(p->childast->recordType);
		if(strcmp(p->childast->recordType,recST[index].name)==0)
			st = recST[index].next;
		else{
			entry *temp1 = recST[index].head;
			while((temp1->name,p->childast->recordType)!=0)
				temp1= temp1->head;
			st = temp1->next;
		}
		//printf("%d\n",width);
		index = hash(p->childast->siblingast->siblingast->lexemeCurrentNode);
		if(strcmp(p->childast->siblingast->siblingast->lexemeCurrentNode,st[index].name)==0)
			offset = st[index].offset;
		else{
			entry *temp1 = st[index].head;
			while((temp1->name,p->childast->siblingast->siblingast->lexemeCurrentNode)!=0)
				temp1= temp1->head;
			offset = temp1->offset;
		}
		//printf("%d\n",offset);
		offset = offset /2;
		fprintf(asmFile, "	mov	[%s+%d],rax\n",p->childast->lexemeCurrentNode,offset*8);
	}


}


void writeCode(astNode* p){

	astNode* temp = p->siblingast->childast;   //points to TK_ID
	if(temp->siblingast ==NULL){

		if(temp->type==0){

			fprintf(asmFile, "	mov rdi,fmt\n" );
			fprintf(asmFile, "	mov	rsi,[%s]\n",temp->lexemeCurrentNode);
			fprintf(asmFile, "	mov rax,0\n" );
			fprintf(asmFile, "  call printf\n" );

		}
		else{

			int width;
			int index = hash(temp->lexemeCurrentNode);
			if(strcmp(temp->lexemeCurrentNode,symbolTable[index].name)==0)
				width = symbolTable[index].width;
			else{
				entry *temp1 = symbolTable[index].head;
				while((temp1->name,temp->lexemeCurrentNode)!=0)
					temp1= temp1->head;
				width = temp1->width;
			}
			width = width /2;

			int i=0;

			for(i=0;i<width;i++){

				fprintf(asmFile, "	mov rdi,fmt\n");
				fprintf(asmFile, "	mov	rsi,[%s+%d]\n",temp->lexemeCurrentNode,i*8);
				fprintf(asmFile, "	mov rax,0\n");
				fprintf(asmFile, "  call printf\n");
				
			}

		}

	}
	else{

		entry* st;
		int index = hash(temp->recordType);
		if(recST[index].occupied){
			//printf("%s\n", recST[index].name);
			if(strcmp(temp->recordType,recST[index].name)==0)
				st = recST[index].next;
				else{
				entry *temp1 = recST[index].head;
				while((temp1->name,temp->recordType)!=0)
					temp1= temp1->head;
				st = temp1->next;
			}
			//printf("%d\n",width);
		}
		int offset;
		index = hash(temp->siblingast->siblingast->lexemeCurrentNode);
		if(strcmp(temp->siblingast->siblingast->lexemeCurrentNode,st[index].name)==0)
			offset = st[index].offset;
		else{
			entry *temp1 = st[index].head;
			while((temp1->name,temp->siblingast->siblingast->lexemeCurrentNode)!=0)
				temp1= temp1->head;
			offset = temp1->offset;
		}
		//printf("%d\n",offset);
		offset = offset /2;

		fprintf(asmFile, "	mov rdi,fmt\n");
		fprintf(asmFile, "	mov	rsi,[%s+%d]\n",temp->lexemeCurrentNode,offset*8);
		fprintf(asmFile, "	mov rax,0\n");
		fprintf(asmFile, "  call printf\n");

	}
}

void readCode(astNode* p){

	astNode* temp = p->siblingast->childast;   //points to TK_ID
	if(temp->siblingast ==NULL){

		if(temp->type==0){

			fprintf(asmFile, "	sub rbp,8\n");
			fprintf(asmFile, "	mov rdi,scanfmt\n" );
			fprintf(asmFile, "	mov	rsi,%s\n",temp->lexemeCurrentNode);
			fprintf(asmFile, "  call scanf\n");
			fprintf(asmFile, "	add rbp,8\n");

		}
		else{

			int width;
			int index = hash(temp->lexemeCurrentNode);
			if(strcmp(temp->lexemeCurrentNode,symbolTable[index].name)==0)
				width = symbolTable[index].width;
			else{
				entry *temp1 = symbolTable[index].head;
				while((temp1->name,temp->lexemeCurrentNode)!=0)
					temp1= temp1->head;
				width = temp1->width;
			}
			width = width /2;

			int i=0;

			for(i=0;i<width;i++){

				fprintf(asmFile, "	sub rbp,8\n");
				fprintf(asmFile, "	mov rdi,scanfmt\n" );
				fprintf(asmFile, "	mov	rsi,%s+%d\n",temp->lexemeCurrentNode,i*8);
				fprintf(asmFile, "  call scanf\n");
				fprintf(asmFile, "	add rbp,8\n");
				
			}

		}

	}
	else{

		entry* st;
		int index = hash(temp->recordType);
		if(recST[index].occupied){
			//printf("%s\n", recST[index].name);
			if(strcmp(temp->recordType,recST[index].name)==0)
				st = recST[index].next;
				else{
				entry *temp1 = recST[index].head;
				while((temp1->name,temp->recordType)!=0)
					temp1= temp1->head;
				st = temp1->next;
			}
			//printf("%d\n",width);
		}
		int offset;
		index = hash(temp->siblingast->siblingast->lexemeCurrentNode);
		if(strcmp(temp->siblingast->siblingast->lexemeCurrentNode,st[index].name)==0)
			offset = st[index].offset;
		else{
			entry *temp1 = st[index].head;
			while((temp1->name,temp->siblingast->siblingast->lexemeCurrentNode)!=0)
				temp1= temp1->head;
			offset = temp1->offset;
		}
		//printf("%d\n",offset);
		offset = offset /2;

		fprintf(asmFile, "	sub rbp,8\n");
		fprintf(asmFile, "	mov rdi,scanfmt\n" );
		fprintf(asmFile, "	mov	rsi,%s+%d\n",temp->lexemeCurrentNode,offset*8);
		fprintf(asmFile, "  call scanf\n");
		fprintf(asmFile, "	add rbp,8\n");

	}

}



void boolExpressionCode(astNode* p){

	if(p->enumVal == TK_ID){
		fprintf(asmFile, "	mov	rax,[%s]\n",p->lexemeCurrentNode);
		return;
	}
	if(p->enumVal == TK_NUM){
		int num = atoi(p->valueIfNumber);
		fprintf(asmFile, "	mov rax,%d\n",num);
		return;
	}


	boolExpressionCode(p->childast);

	fprintf(asmFile, "	push	rax\n");


	if(p->childast->siblingast != NULL){

		boolExpressionCode(p->childast->siblingast);
		fprintf(asmFile, "	mov	rbx,rax\n");

	}
		
	fprintf(asmFile, "	pop	rax\n");

	if(p->enumVal == TK_AND){

		fprintf(asmFile, "	and rax,rbx\n");
		return;
	}

	if(p->enumVal== TK_OR){

		fprintf(asmFile, "	or rax,rbx\n");
		return;

	}

	if(p->enumVal == TK_NOT){

		fprintf(asmFile, "	xor rax,1\n");
		return;

	}

	if(p->enumVal == TK_LT){

		relCount++;
		fprintf(asmFile, "	cmp	rax,rbx\n");
		fprintf(asmFile, "	jl	label%d\n",relCount);

		relCount++;
		fprintf(asmFile, "	mov	rax,0\n");
		fprintf(asmFile, "	jmp	label%d\n\n",relCount);

		fprintf(asmFile, "label%d:\n",relCount-1);
		fprintf(asmFile, "	mov	rax,1\n\n");
		fprintf(asmFile, "label%d:\n\n",relCount);
		return;

	}

	if(p->enumVal == TK_LE){

		relCount++;
		fprintf(asmFile, "	cmp	rax,rbx\n");
		fprintf(asmFile, "	jle	label%d\n",relCount);

		relCount++;
		fprintf(asmFile, "	mov	rax,0\n");
		fprintf(asmFile, "	jmp	label%d\n\n",relCount);

		fprintf(asmFile, "label%d:\n",relCount-1);
		fprintf(asmFile, "	mov	rax,1\n\n");
		fprintf(asmFile, "label%d:\n\n",relCount);
		return;
	}

	if(p->enumVal == TK_GT){

		relCount++;
		fprintf(asmFile, "	cmp	rax,rbx\n");
		fprintf(asmFile, "	jg	label%d\n",relCount);

		relCount++;
		fprintf(asmFile, "	mov	rax,0\n");
		fprintf(asmFile, "	jmp	label%d\n\n",relCount);

		fprintf(asmFile, "label%d:\n",relCount-1);
		fprintf(asmFile, "	mov	rax,1\n\n");
		fprintf(asmFile, "label%d:\n\n",relCount);
		return;
	}

	if(p->enumVal == TK_GE){

		relCount++;
		fprintf(asmFile, "	cmp	rax,rbx\n");
		fprintf(asmFile, "	jge	label%d\n",relCount);

		relCount++;
		fprintf(asmFile, "	mov	rax,0\n");
		fprintf(asmFile, "	jmp	label%d\n\n",relCount);

		fprintf(asmFile, "label%d:\n",relCount-1);
		fprintf(asmFile, "	mov	rax,1\n\n");
		fprintf(asmFile, "label%d:\n\n",relCount);
		return;
	}

	if(p->enumVal == TK_EQ){

		relCount++;
		fprintf(asmFile, "	cmp	rax,rbx\n");
		fprintf(asmFile, "	je	label%d\n",relCount);

		relCount++;
		fprintf(asmFile, "	mov	rax,0\n");
		fprintf(asmFile, "	jmp	label%d\n\n",relCount);

		fprintf(asmFile, "label%d:\n",relCount-1);
		fprintf(asmFile, "	mov	rax,1\n\n");
		fprintf(asmFile, "label%d:\n\n",relCount);
		return;
	}

	if(p->enumVal == TK_NE){

		relCount++;
		fprintf(asmFile, "	cmp	rax,rbx\n");
		fprintf(asmFile, "	je	label%d\n",relCount);

		relCount++;
		fprintf(asmFile, "	mov	rax,1\n");
		fprintf(asmFile, "	jmp	label%d\n\n",relCount);

		fprintf(asmFile, "label%d:\n",relCount-1);
		fprintf(asmFile, "	mov	rax,0\n\n");
		fprintf(asmFile, "label%d:\n\n",relCount);
		return;
	}

}


void conditionalCode(astNode* p){

	boolExpressionCode(p->siblingast);

	fprintf(asmFile, "\n cmp rax,1\n");

	int localCount = condCount;

	condCount += 2;
	localCount++;
	fprintf(asmFile, "	jnz	COND_L%d\n",localCount);     //label for elsepart
	
	generateCode(p->siblingast->siblingast);           //executing if statements

	//printf("684\n");
	localCount++;
	fprintf(asmFile, "	jmp	COND_L%d\n",localCount);  //label to exit

	fprintf(asmFile, "COND_L%d:\n\n",localCount-1);

	astNode* temp = p->siblingast->siblingast->siblingast;
	if(temp->siblingast == NULL){
		if(temp->childast->siblingast != NULL){
			generateCode(temp->childast->siblingast);		
		}
	}
	else{
		if(temp->siblingast->childast->siblingast != NULL){
			generateCode(temp->siblingast->childast->siblingast);		
		}	
	}
	
	fprintf(asmFile, "COND_L%d:\n\n",localCount);

}

void iterativeCode(astNode* p){

	int localCount = condCount;

	condCount += 2;
	localCount++;
	

	fprintf(asmFile, "COND_L%d:\n\n",localCount);

	boolExpressionCode(p->siblingast);

	fprintf(asmFile, "\n cmp rax,1\n");
	
	localCount++;
	fprintf(asmFile, "	jnz	COND_L%d\n",localCount);     //label for elsepart
	
	generateCode(p->siblingast->siblingast);           //executing if statements

	fprintf(asmFile, "	jmp	COND_L%d\n",localCount-1);

	fprintf(asmFile, "COND_L%d:\n\n",localCount);

}

void generateCode(astNode* p){

	if(p == NULL)
		return;

	if(p->enumVal==declarations){
		declarationCode(p->childast);                //child would be first declared variable


		fprintf(asmFile, "	temp:	resb	80\n");
		fprintf(asmFile, "	temp1:	resb	80\n");
		fprintf(asmFile, "\n\n");


		fprintf(asmFile, "	section	.data\n\n" );
		fprintf(asmFile, "fmt:	db	\"a=%%ld\",10,0\n" );
		fprintf(asmFile, "scanfmt:	db	\"%%ld\",0\n\n");
		//fmt1: db "%d",0
		fprintf(asmFile, "	section	.text\n");
		fprintf(asmFile, "	global	main\n\n");

		fprintf(asmFile, "main:		\n");
		fprintf(asmFile, "	push rbp\n\n");

		generateCode(p->siblingast);
		return;
	}

	if(p->enumVal==assignmentStmt){
		
		assignmentCode(p->childast);                 // child would be singleOrRecId

		fprintf(asmFile, "\n\n");
		generateCode(p->siblingast);
		return;
	}


	if(p->enumVal == ioStmt){

		if(p->childast->enumVal == TK_WRITE){
			writeCode(p->childast);
		}		
		else{
			readCode(p->childast);
		}
		generateCode(p->siblingast);
		return;
	}

	if(p->enumVal== conditionalStmt){


		fprintf(asmFile, ";IF-ELSE-THEN BEGINS\n\n");
		conditionalCode(p->childast);          // it points to TK_IF

		fprintf(asmFile, "\n;IF-ELSE-THEN ENDS\n\n");
		
		generateCode(p->siblingast);
		return;

	}


	if(p->enumVal == iterativeStmt){

		fprintf(asmFile, ";WHILE LOOP BEGINS\n\n");
		iterativeCode(p->childast);
		fprintf(asmFile, ";WHILE LOOP ENDS\n\n");

		generateCode(p->siblingast);
		return;
	}


	if(p->enumVal == elsePart)
		return;


	generateCode(p->childast);
	generateCode(p->siblingast);
}

void codegeneration(char filename[]){

	asmFile = fopen(filename,"w");
	fprintf(asmFile, "\n	extern printf\n");
	fprintf(asmFile, "	extern scanf\n\n");

	generateCode(astroot);


	fprintf(asmFile, "	pop rbp\n");
	fprintf(asmFile, "	mov rax,0\n\n");
	fprintf(asmFile, "	ret\n");

	fclose(asmFile);
}