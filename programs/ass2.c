#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
// #include <malloc.h>
#define MAXLINE 1000

//declarations
char ** tokenize(char*);
char ** get_input() ;

/*int main(int argc, char** argv){
	char ** tokens = get_input() ;
	int i ;

	// Freeing the allocated memory	
	for(i=0;tokens[i]!=NULL;i++){
		free(tokens[i]);
	}
	free(tokens);
	return 0;
}
*/


/*the tokenizer function takes a string of chars and forms tokens out of it*/
char ** tokenize(char* input){
	int i;
	int doubleQuotes = 0;
	
	char *token = (char *)malloc(1000*sizeof(char));
	int tokenIndex = 0;

	char **tokens;
	tokens = (char **) malloc(MAXLINE*sizeof(char*));

	int tokenNo = 0;
	
	for(i =0; i < strlen(input); i++){
		char readChar = input[i];
		if (readChar == '"'){
			doubleQuotes = (doubleQuotes + 1) % 2;
			if (doubleQuotes == 0){
				token[tokenIndex] = '\0';
				if (tokenIndex != 0){
					tokens[tokenNo] = (char*)malloc(MAXLINE*sizeof(char));
					strcpy(tokens[tokenNo++], token);
					tokenIndex = 0; 
				}
			}
		}
		else if (doubleQuotes == 1){
			token[tokenIndex++] = readChar;
		}
		else if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0){
				tokens[tokenNo] = (char*)malloc(MAXLINE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else{
			token[tokenIndex++] = readChar;
		}
	}
	
	if (doubleQuotes == 1){
		token[tokenIndex] = '\0';
		if (tokenIndex != 0){
			tokens[tokenNo] = (char*)malloc(MAXLINE*sizeof(char));
			strcpy(tokens[tokenNo++], token);
		}
	}
	
	tokens[tokenNo] = NULL ;
	return tokens;
}

char ** get_input() {
	FILE* stream = stdin;
	char input[MAXLINE];
	fflush(stdin) ;

	char *in = fgets(input, MAXLINE, stream); // taking input ;
	if (in == NULL){
		return NULL ;
	}
	// Calling the tokenizer function on the input line    
	char **tokens = tokenize(input);	
	
	// Comment to NOT print tokens
	int i ;
	/*for(i=0;tokens[i]!=NULL;i++){
		printf("%s\n", tokens[i]);
	}   */
	return tokens ;
}


