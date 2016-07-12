// Batch -3
// Ram Karnani 2012B3A7750P
// Akath Singh Dua 2012B4A7333P


//############################# CODE FOR LEXER #######################################


// ################ FUNCTION TO CHECK WHETHER A CHARACTER IS A TERMINAL OR NOT ######################## 

int isTerminal(char c){
	if((c>=65 && c<=90) || (c>=97 && c<=122) || (c>=48 && c<=57) || (c==' ') || (c=='\t') || (c=='\n')
	 || c=='<' || c=='-' || c=='=' || c=='%' || c=='.' || c=='_' || c=='#' || c=='[' || c==']' || c==';' 
	 || c==':' || c=='(' || c==')' || c=='+' ||c=='*' || c=='/' || c=='&' || c=='@' || c=='~' || c=='>' || c=='!' || c==',' || c==0 )
	return 1;
	else
	return 0;
}


//##############################################################################################

// FUNCTION TO RETURN THE INDEX OF KEYWORD FROM THE LEXEMES ARRAY USING BINARY SEARCH  

int getTok(char temp[]){
	int i=0;
	int low=0,high=22;
	int mid;
	
	while(low<=high){
		mid = (low+high)/2;		
		if(strcmp(temp,lexemes[mid])==0)
			return mid;
		else if(strcmp(temp,lexemes[mid])< 0) high=mid-1;
		else{
			low=mid+1;
		}
	}
	return -1;
}


//##############################################################################################


//### FUNCTIO TO LOAD THE BUFFERS FROM THE INPUT FILE POINTED TO BY FILE POINTER FP ############
FILE *getStream(FILE *fp, buffer B, buffersize k)
{

		if(feof(fp)){
			return fp;
		}
		else{
			int s = fread(B,sizeof(char),k-2,fp); // k-1 so that it gets appended by eof.
			//printf("%s\n",B );
			B[s]=EOF; // putting the sentinel value after the characters read.
			return fp;
		}
}



//##############################################################################################


void append(char* str,char c){
	int len = strlen(str);
	str[len]=c;
	str[len+1] ='\0';
}


//##############################################################################################


// THE FUNCTION GENERATES THE TOKEN AND SENDS IT TO THE PARSER #################################
tokenInfo getNextToken(FILE* fp){

	if(*forward==EOF && feof(fp)) {
		lexflag=0;
		return;
	}
	int state =0;				// VARIABLE TO STORE THE CURRENT STATE OF DFA BEING ACCESSED
	tokenInfo tok;				// VARIABLE TO STORE THE INFORMATION REGARDING TOKENS

	strcpy(tok.lexemeName,"");

	tok.tokenName = 105;
	char checker[100];			// ARRAY TO CHECK THE LENGTH CONSTRAINTS OF LEXEMES OF IDENTIFIERS
	memset(checker,0,sizeof(char));
	
	int x;
	int loopVar=1;				//  VARIABLE TO KEEP ON LOOPING UNTIL A TOKN IS GENERATED
	int ftp=0;
while(loopVar){					//LOOP TO GENERATE THE TOKEN
	
	char ch;
	ch= *forward;

	// CONDITION TO REFILL THE BUFFERS ACCORDINGLY IF ANY OF THE BUFFERS IS CONSUMED FULLY//

	if(ch==EOF){

		//FILL THE SECOND BUFFER IF FIRST IS CONSUMED 
		if(ch == secondB[0] && !feof(fp)){
			fp= getStream(fp,secondB,bsize);
			//printf("SECOND\n");
			memset(firstB,0,sizeof(char));
			firstB[0]=EOF;
			forward = secondB;
			ch=*forward;
			}

		//FILL THE SECOND BUFFER IF FIRST IS CONSUMED
		else if(ch == firstB[0] && !feof(fp)){
			fp= getStream(fp,firstB,bsize);
			forward = firstB;
			//printf("FIRST\n");
			memset(secondB,0,sizeof(char));
			secondB[0]=EOF;
			ch=*forward;
			}
		// TERMINATING CONDITIONS TO END THE LEXICL PHASE
		else{

			loopVar=0;
			lexflag=0;
		}
	}
	
	// SWITCH CASE STATEMENTS TO GENERATE THE TOKENS USING DFA 	
	switch(state){

		case 0 :

			if(ch=='~'){
				tok.tokenName = TK_NOT;
				strcpy(tok.lexemeName,"~");
				tok.lineno = lineNo;
				forward+=1;
				state = 19;
				return tok;
				}

			else if(ch=='%'){
				tok.tokenName = TK_COMMENT;
				strcpy(tok.lexemeName,"%");
				tok.lineno = lineNo;
				while(*forward !='\n'){
					ch = *forward;
					if(ch==EOF){
						if(ch == secondB[0] && !feof(fp)){
							fp= getStream(fp,secondB,bsize);
							memset(firstB,0,sizeof(char));
							firstB[0]=EOF;
							forward = secondB;
							ch=*forward;
							}
						else if(ch == firstB[0] && !feof(fp)){
							fp= getStream(fp,firstB,bsize);
							forward = firstB;
							memset(secondB,0,sizeof(char));
							secondB[0]=EOF;
							ch=*forward;
							}
					}


					forward += 1;
				}
				forward+=1;
				lineNo +=1;
				return tok;
				}

			else if(ch=='['){
				tok.tokenName = TK_SQL;
				strcpy(tok.lexemeName,"[");
				tok.lineno = lineNo;
				forward+=1;
				state = 21;
				return tok;
				}

			else if(ch==']'){
				tok.tokenName = TK_SQR;
				strcpy(tok.lexemeName,"]");
				tok.lineno = lineNo;
				forward+=1;
				state = 22;
				return tok;
				}

			else if(ch==';'){
				tok.tokenName = TK_SEM;
				strcpy(tok.lexemeName,";");
				tok.lineno = lineNo;
				forward+=1;
				state =23;
				return tok;
				}

			else if(ch==':'){
				tok.tokenName = TK_COLON;
				strcpy(tok.lexemeName,":");
				tok.lineno = lineNo;
				forward+=1;	
				state =24;
				return tok;
				}

			else if(ch=='.'){
				tok.tokenName = TK_DOT;
				strcpy(tok.lexemeName,".");
				tok.lineno = lineNo;
				forward+=1;
				state=25;
				return tok;
			}

			else if(ch=='('){
				tok.tokenName = TK_OP;
				strcpy(tok.lexemeName,"(");
				tok.lineno = lineNo;
				forward+=1;
				state=26;
				return tok;
				}

			else if(ch==')'){
				tok.tokenName = TK_CL;
				strcpy(tok.lexemeName,")");
				tok.lineno = lineNo;
				forward+=1;
				state=27;
				return tok;
				}

			else if(ch=='+'){
				tok.tokenName = TK_PLUS;
				strcpy(tok.lexemeName,"+");
				tok.lineno = lineNo;
				forward+=1;
				state=28;
				return tok;
				}

			else if(ch=='-'){
				tok.tokenName = TK_MINUS;
				strcpy(tok.lexemeName,"-");
				tok.lineno = lineNo;
				forward+=1;
				state=29;
				return tok;
				}

			else if(ch=='*'){
				tok.tokenName = TK_MUL;
				strcpy(tok.lexemeName,"*");
				tok.lineno = lineNo;
				forward+=1;
				state=30;
				return tok;
				}

			else if(ch=='/'){
				tok.tokenName = TK_DIV;
				strcpy(tok.lexemeName,"/");
				tok.lineno = lineNo;
				forward+=1;
				state=31;
				return tok;
				}

			else if(ch==','){
				tok.tokenName = TK_COMMA;
				strcpy(tok.lexemeName,",");
				tok.lineno = lineNo;
				forward+=1;
				state=51;
				return tok;
				}

			else if(ch=='&'){
				forward+=1;
				state=32;
				}

			else if(ch=='@'){
				forward +=1;
				state =35;
				}

			else if(ch=='='){
				forward+=1;
				state =49;
				}

			else if(ch=='>'){
				forward+=1;
				state = 44;
				}

			else if(ch=='<'){
				forward+=1;
				state = 38;
				}

			else if(ch=='!'){
				forward+=1;
				state = 47;
				}

			else if(ch=='#'){
				forward+=1;
				state = 16;
				append(tok.lexemeName,ch);
				tok.lineno = lineNo;
				}
			
			else if(ch=='_'){
				forward+=1;
				append(tok.lexemeName,ch);
				state = 7;
				tok.lineno = lineNo;
				}
			
			else if(ch >='0' && ch <= '9'){
				forward+=1;
				append(tok.lexemeName,ch);
				tok.lineno = lineNo;
				state = 11;
				}

			else if(ch >='b' && ch <='d'){
				forward+=1;
				append(tok.lexemeName,ch);
				tok.lineno = lineNo;
				state = 1;
				}

			else if((ch>= 'a' && ch<='z') && ch!='b'&& ch!='c' && ch!='d'){
				forward+=1;
				append(tok.lexemeName,ch);
				state = 5;
				tok.lineno = lineNo;
			}

			else if(ch=='\n'){
				forward+=1;
				lineNo+=1;
				}

			else if(ch==' ' || ch=='\t'){
				forward+=1;
				}

			else if(ch=='\r'){
				forward+=1;
				}

			else if(ch==EOF){
				return tok;
			}

			else if (!isTerminal(ch)){
				tok.tokenName = TK_LEXERROR;
				append(tok.lexemeName,ch);
				tok.lineno=lineNo;
				lexError =1;
				printf("ERROR_2: Unknown Symbol  <%c> at line <%d>\n", ch,lineNo);
				forward+=1;
				return tok;
			}
			else loopVar=0;
			break;

//##############################################################################################

		case 32:
			
			if(ch==EOF && !feof(fp))
				continue;

			if (ch=='&'){
				forward+=1;
				state = 33;
			}

			else{
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"&");
				tok.lineno = lineNo;	
				lexError =1;				
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}

			break;

//##############################################################################################

		case 33:
			if(ch==EOF && !feof(fp))
				continue;
			
			if (ch=='&'){
				tok.tokenName = TK_AND;
				strcpy(tok.lexemeName,"&&&");
				tok.lineno = lineNo;
				forward+=1;
				return tok;
			}

			else{
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"&&");
				tok.lineno = lineNo;		
				lexError =1;			
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}

			break;

//##############################################################################################
		
		case 35:

			if(ch==EOF && !feof(fp))
				continue;

			if (ch=='@'){
			
				forward+=1;
				state = 36;
			}

			else{
			
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"@");
				tok.lineno = lineNo;		
				lexError =1;			
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 36:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if (ch=='@'){

				tok.tokenName = TK_OR;
				strcpy(tok.lexemeName,"@@@");
				tok.lineno = lineNo;
				forward+=1;
				return tok;

			}

			else{

				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"@@");
				tok.lineno = lineNo;	
				lexError =1;				
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			
			break;

//##############################################################################################

		case 38:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch=='='){
			
				tok.tokenName = TK_LE;
				strcpy(tok.lexemeName,"<=");
				tok.lineno = lineNo;
				forward +=1;
				return tok;
			}
			
			else if(ch=='-'){
			
				state=40;
				forward +=1;
			}
			
			else{
			
				tok.tokenName = TK_LT;
				strcpy(tok.lexemeName,"<");
				tok.lineno = lineNo;
				return tok;
			}
			break;

//##############################################################################################

		case 40:

			if(ch==EOF && !feof(fp))
				continue;
			if(ch=='-'){
				
				state=41;
				forward +=1;
			}
			
			else{
			
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"<-");
				tok.lineno = lineNo;		
				lexError =1;			
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 41:

			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch=='-'){

				tok.tokenName = TK_ASSIGNOP;
				strcpy(tok.lexemeName,"<---");
				tok.lineno = lineNo;
				forward +=1;
				return tok;
			}

			else{
			
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"<--");
				tok.lineno = lineNo;	
				lexError =1;				
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 44:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch=='='){
				
				tok.tokenName =TK_GE;
				strcpy(tok.lexemeName,">=");
				tok.lineno = lineNo;
				forward +=1;
				return tok;
			}

			else{
				
				tok.tokenName = TK_GT;
				strcpy(tok.lexemeName,">");
				tok.lineno = lineNo;
				return tok;
			}
			break;

//##############################################################################################

		case 47:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch=='='){
				tok.tokenName = TK_NE;
				strcpy(tok.lexemeName,"!=");
				tok.lineno = lineNo;
				forward +=1;
				return tok;
			}
			
			else{
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"!");
				tok.lineno = lineNo;
				lexError =1;					
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			
			break;

//##############################################################################################		

		case 49:

			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch=='='){
			
				tok.tokenName = TK_EQ;
				strcpy(tok.lexemeName,"==");
				tok.lineno = lineNo;
				forward +=1;
				return tok;
			}
			else{
			
				tok.tokenName = TK_PATERROR;
				strcat(tok.lexemeName,"=");
				tok.lineno = lineNo;			
				lexError =1;		
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 16:

			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch >= 'a' && ch <='z'){
			
				state =17;
				append(tok.lexemeName,ch);
				forward +=1;
			}
			
			else{
			
				tok.tokenName = TK_PATERROR;
				lexError =1;
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 17:
			while(ch >= 'a' && ch <='z'){
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(strlen(checker)>20){
				
					tok.tokenName = TK_LENGTH;
					lexError =1;
					printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 20 characters.\n",tok.lineno);
					return tok;
				}
			
			strcat(tok.lexemeName,checker);

			tok.tokenName = TK_RECORDID;
			return tok;
			break;

//##############################################################################################

		case 11:
			while(ch >= '0' && ch <='9'){
			
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}	
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(strlen(checker)>20){
				
					tok.tokenName = TK_LENGTH;
					lexError =1;
					printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 20 characters.\n", tok.lineno);
					return tok;
				}
			
			strcat(tok.lexemeName,checker);

			if(ch=='.'){

				append(tok.lexemeName,ch);
				state =12;
				forward +=1;
			}

			else{

				tok.tokenName = TK_NUM;
				return tok;
			}
			break;

//##############################################################################################

		case 12:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch >= '0' && ch <='9'){
				
				append(tok.lexemeName,ch);
				forward +=1;
				state =13;
			}
			
			else{
			
				tok.tokenName = TK_PATERROR;
				lexError =1;
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 13:

			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch >= '0' && ch <='9'){
			
				append(tok.lexemeName,ch);
				forward +=1;
				ch =*forward;
				tok.tokenName = TK_RNUM;
				return tok;
			}
			
			else{
			
				tok.tokenName = TK_PATERROR;
				lexError =1;
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 1:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(ch >= '2' && ch <='7'){
				append(tok.lexemeName,ch);
				forward +=1;
				state =2;
			}

			else if(ch >= 'a' && ch <='z'){
				append(tok.lexemeName,ch);
				forward +=1;
				state =5;
			}

			else{
				tok.tokenName = TK_PATERROR;
				lexError =1;
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			break;

//##############################################################################################

		case 2:
			while(ch >= 'b' && ch <='d'){
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}
			
			if(ch==EOF && !feof(fp))
				continue;

			if(ch >= '2' && ch <='7'){
				append(checker,ch);
				forward +=1;
				state =3;
			}
			
			else{
				
				if(strlen(checker)>20){
					tok.tokenName = TK_LENGTH;
					lexError =1;
					printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 20 characters.\n",tok.lineno);
					return tok;
				}
				
				else{
					strcat(tok.lexemeName,checker);
					tok.tokenName = TK_ID;
					return tok;
				}
			}
			break;

//##############################################################################################

		case 3:
			while(ch >= '2' && ch <='7'){
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}
			if(ch==EOF && !feof(fp))
				continue;
			
			if(strlen(checker)>20){
					tok.tokenName = TK_LENGTH;
					lexError =1;
					printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 20 characters.\n",tok.lineno);
					return tok;
				}
			
			strcat(tok.lexemeName,checker);
			tok.tokenName = TK_ID;
			return tok;
			break;

//##############################################################################################

		case 5:
			while(ch >= 'a' && ch <='z'){
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if(strlen(checker)>20){
					tok.tokenName = TK_LENGTH;
					lexError =1;
					printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 20 characters.\n", tok.lineno);
					return tok;
				}
			
			strcat(tok.lexemeName,checker);
			int index = getTok(tok.lexemeName);
			
			if(index==-1) 
				tok.tokenName = TK_FIELDID;
			
			else
				tok.tokenName = tokens[index];
			
			return tok;
			
			break;

//##############################################################################################
		
		case 7:
			
			if(ch==EOF && !feof(fp))
				continue;
			
			if((ch >= 'a' && ch <='z') || (ch >= 'A' && ch <='Z')){
				append(tok.lexemeName,ch);
				forward +=1;
				state =8;
			}
			
			else{
				tok.tokenName = TK_PATERROR;
				lexError =1;
				printf("ERROR_3: Unknown pattern <%s> at line number <%d>\n", tok.lexemeName,tok.lineno);
				return tok;
			}
			
			break;

//##############################################################################################

		case 8:

			while((ch >= 'a' && ch <='z') || (ch >= 'A' && ch <='Z')){
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}

			if(ch==EOF && !feof(fp))
				continue;
			
			if(strlen(checker)>30){
					tok.tokenName = TK_LENGTH;
					printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 30 characters.\n",tok.lineno);
					return tok;
				}

			strcat(tok.lexemeName,checker);
			
			if(ch >= '0' && ch <='9'){
				append(tok.lexemeName,ch);
				forward +=1;
				state =9;
			}
			
			else{
				if(strcmp(tok.lexemeName,"_main")==0){
					tok.tokenName = TK_MAIN;
					return tok;
				}
				else{
					tok.tokenName = TK_FUNID;
					return tok;
				}
			}
			break;

//##############################################################################################

		case 9:
			while(ch >= '0' && ch <='9'){
				append(checker,ch);
				forward +=1;
				ch =*forward;
			}

			if(ch==EOF && !feof(fp))
				continue;
			
			if(strlen(checker)>30){
				tok.tokenName = TK_LENGTH;
				lexError =1;
				printf("ERROR_1: Identifier at line<%d> is longer than the prescribed length of 30 characters.\n",tok.lineno);
				return tok;
			}

			strcat(tok.lexemeName,checker);
			tok.tokenName = TK_FUNID;
			return tok;
			break;

//##############################################################################################

		default : 
				loopVar=0;
	}
}
 return tok;
}
