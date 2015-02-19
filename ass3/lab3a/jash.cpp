#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <list>
#include <vector>
#include <map>
#include <string>

using namespace std;

#define MAXLINE 1000
#define DEBUG 0

/* Function declarations and globals */
int curr_id;
int parent_pid ;
list<int> child_ids;
int child_id;
char ** tokenize(char*) ;
int execute_command(char** tokens) ;

void SIGINT_handler(int sig){
	for (list<int> ::iterator it=child_ids.begin();it!=child_ids.end();it++){
		kill(*it, SIGTERM);
			}
	}

int main(int argc, char** argv){
	parent_pid = getpid() ;
	
	struct sigaction sa;
	memset(&sa,0,sizeof(sa));
	sa.sa_handler=&SIGINT_handler;
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);
	/* Set (and define) appropriate signal handlers */
	/* Exact signals and handlers will depend on your implementation */
	// signal(SIGINT, quit);
	// signal(SIGTERM, quit);

	char input[MAXLINE];
	char** tokens;
	
	while(1) { 
		char pwd[1024];
		getcwd(pwd, sizeof(pwd));
		string s(pwd);
		cout << s<<"$ "; // The prompt
		fflush(stdin);

		char *in = fgets(input, MAXLINE, stdin); // Taking input one line at a time
		//Checking for EOF
		if (in == NULL){
			if (DEBUG) printf("jash: EOF found. Program will exit.\n");
			break ;
		}

		// Calling the tokenizer function on the input line    
		tokens = tokenize(input);	
		// Executing the command parsed by the tokenizer
		execute_command(tokens) ; 
		
		// Freeing the allocated memory	
		int i ;
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
	}
	
	/* Kill all Children Processes and Quit Parent Process */
	return 0 ;
}

/*The tokenizer function takes a string of chars and forms tokens out of it*/
char ** tokenize(char* input){
	int i, doubleQuotes = 0, tokenIndex = 0, tokenNo = 0 ;
	char *token = (char *)malloc(MAXLINE*sizeof(char));
	char **tokens;

	tokens = (char **) malloc(MAXLINE*sizeof(char*));

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
		} else if (doubleQuotes == 1){
			token[tokenIndex++] = readChar;
		} else if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0){
				tokens[tokenNo] = (char*)malloc(MAXLINE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		} else {
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

int execute_command(char** tokens) {
	curr_id=getpid();
	cout<<curr_id<<endl;
	char *temp=(char *)malloc(MAXLINE*sizeof(char));
	/* 
	Takes the tokens from the parser and based on the type of command 
	and proceeds to perform the necessary actions. 
	Returns 0 on success, -1 on failure. 
	*/
	if (tokens == NULL) {
		return -1 ; 				// Null Command
	} else if (tokens[0] == NULL) {
		return 0 ;					// Empty Command
	} else if (!strcmp(tokens[0],"exit")) {
		kill(curr_id,SIGTERM);
		/* Quit the running process */
		return 0 ;
	} 
	else if (!strcmp(tokens[0],"cd")) {
		
		if (tokens[1]==NULL){
			strcpy(temp,"pwd");}
		else {
			strcpy(temp,tokens[1]);
		int ret=chdir(temp);
			if (ret!=0){
		perror("Error");}
	}
		//system("pwd");
			
		/* Change Directory, or print error on failure */
		return 0 ;
	} else if (!strcmp(tokens[0],"parallel")) {
		int i=1;
		char** tok;
			while(tokens[i]!= NULL){
				
				
				tok = (char**)malloc(MAXLINE*sizeof(char));
					for(int j=0;tokens[i]!= NULL;j++,i++){
						if(strcmp(tokens[i],":::")==0){
							int pid=fork();
							if (pid==0){
							execute_command(tok);
							exit(0);
						}
							else {			
								child_id=pid;
								int status=0;
								//waitpid(pid,&status,0);
									if (status==1){
										perror("child process terminated with error");
									}
						
								}
							break;
						}
						
			else{
					tok[j] = (char*)malloc(MAXLINE*sizeof(char));
					tok[j]=tokens[i];

							}
			
				}
			
				i++;
				
		}
		if (tokens[i]==NULL){
			int pid=fork();
			if (pid==0){
				execute_command(tok);
				exit(0);
			}
			else {
			child_id=pid;
			int status=0;
		//	waitpid(pid,&status,0);
				if (status==1){
				perror("child process terminated with error");
			}
			/* Parent Process */
			/* Wait for child process to complete */
			}
		}
		
		cout<<"end"<<endl;
		
		/* Analyze the command to get the jobs */
		/* Run jobs in parallel, or print error on failure */
		return 0 ;
	}
	 else if (!strcmp(tokens[0],"sequential")) {
		/* Analyze the command to get the jobs */
		/* Run jobs sequentially, print error on failure */
		/* Stop on failure or if all jobs are completed */
		return 0 ;					// Return value accordingly
	} else {
		/* Either file is to be executed or batch file to be run */
		/* Child process creation (needed for both cases) */
		int pid = fork() ;
		
		if (pid == 0) {
			if (!strcmp(tokens[0],"run")) {
				char * cstr = new char [100];
			 FILE *myfile;
			 myfile=fopen(tokens[1],"r");
			if (myfile == NULL) perror ("Error opening file");
			else {
				while( fgets (cstr, 100 , myfile) != NULL ){
				char** tok;
				tok = tokenize(cstr);
							execute_command(tok);
			}
				fclose (myfile);
				}
			
				/* Locate file and run commands */
				/* May require recursive call to execute_command */
				/* Exit child with or without error */
				exit (0) ;
			}
			else {
				
				int res_file;
				res_file=execvp(tokens[0],tokens);
				if (res_file==-1){
					perror("file execution problem");
				}
				/* File Execution */
				/* Print error on failure, exit with error*/
				exit(0) ;
			}
		}
		else {
			
			child_id=pid;
			int status;
			waitpid(pid,&status,0);
			if (status==1){
				perror("child process terminated with error");
			}
			/* Parent Process */
			/* Wait for child process to complete */
		}
	}
	return 1 ;
}
