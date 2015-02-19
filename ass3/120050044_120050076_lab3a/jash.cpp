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
#include <vector>
#include <map>
#include <string>
#include <list>

using namespace std;

#define MAXLINE 1000
#define DEBUG 0

/* Function declarations and globals */
int curr_id;
int parent_pid ;
list<int> child_id;
list<int> back_id;
char ** tokenize(char*) ;
int execute_command(char** tokens) ;


void handle_sigchld(int sig) {

	 pid_t pid;
    int   status;
    while ((pid = waitpid(-1, &status, WNOHANG)) != -1)
    {
    	for (list<int> ::iterator it=back_id.begin();it!=back_id.end();it++){
    		if (*it==pid){
        cout<<pid<<" exited"<<endl; 
        back_id.erase(it); // Or whatever you need to do with the PID
    }
}
}
}
void SIGINT_handler(int sig){
for (list<int> ::iterator it=child_id.begin();it!=child_id.end();it++){
kill(*it, SIGTERM);
}

}

int main(int argc, char** argv){
	parent_pid = getpid() ;
	struct sigaction sb;
	struct sigaction sa;
	memset(&sa,0,sizeof(sa));
	sb.sa_handler=&handle_sigchld;
	sa.sa_handler=&SIGINT_handler;
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);
	sigaction(SIGCHLD,&sb,NULL);

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
	FILE *in,*out;
	curr_id=getpid();
	char *temp=(char *)malloc(MAXLINE*sizeof(char));
	/* 
	Takes the tokens from the parser and based on the type of command 
	and proceeds to perform the necessary actions. 
	Returns 0 on success, -1 on failure. 
	*/

bool and_term=false;
//&&&&&
	char **temp_tok=(char **)malloc(MAXLINE*sizeof(char));
	for (int i=0;tokens[i]!=NULL;i++){

		if (!strcmp(tokens[i],"&")){
			and_term=true;1

			for (int k=0;k<i;k++){
				temp_tok[k]=tokens[k];
			}
			cout<<temp_tok[0]<<endl;
			 pid_t piid=fork();
			 if (piid==0){
			 	execute_command(temp_tok);

			 	exit(0);
			 }
			 else {
			 	signal(SIGCHLD,handler4);
			 	back_id.push_back(piid);
			 }


	}
		}




	if (tokens == NULL) {
		return -1 ; 				// Null Command
	} 
	else if (tokens[0] == NULL) {
		return 0 ;					// Empty Command
	} 
	else if (and_term){
		return 0;
	}
	
	
	else if (!strcmp(tokens[0],"exit")) {

		for (list<int>::iterator it=back_id.begin();it!=back_id.end();it++){
			kill(*it,SIGTERM);
		}
		kill(curr_id,SIGTERM);
		/* Quit the running process */
		return 0 ;
	} 
	
	
	else if (!strcmp(tokens[0],"cd")) {
		
		//cout<<tokens[1]<<endl;
		if (tokens[1]==NULL){
			strcpy(temp,"pwd");}
		else {
			strcpy(temp,tokens[1]);
			//cout<<temp<<endl;
		int ret=chdir(temp);
		if (ret!=0){
			perror("Error");
			return -1;
		}
	}
		//system("pwd");
			
		/* Change Directory, or print error on failure */
		return 0 ;
	}
	
	
	 else if (!strcmp(tokens[0],"parallel")) {
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
								child_id.push_back(pid);
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
			child_id.push_back(pid);
			int status=0;
		//	waitpid(pid,&status,0);
				if (status==1){
				perror("child process terminated with error");
			}
			/* Parent Process */
			/* Wait for child process to complete */
			}
		}
		for (list<int> ::iterator it=child_id.begin();it!=child_id.end();it++){
		waitpid(*it,NULL,0);
		}
		
		//cout << "end";
		/* Analyze the command to get the jobs */
		/* Run jobs in parallel, or print error on failure */
		return 0 ;
	}
	
	
	 else if (!strcmp(tokens[0],"sequential")) {
		/* Analyze the command to get the jobs */
		/* Run jobs sequentially, print error on failure */
		/* Stop on failure or if all jobs are completed */
		int i=1;
		int error=0;
		while(tokens[i]!= NULL){
			char** tok;
			tok = (char**)malloc(MAXLINE*sizeof(char));
			for(int j=0;tokens[i]!= NULL;j++,i++){
				if(strcmp(tokens[i],":::")==0){
					
				error=execute_command(tok);
				break;
				}	
				else{
					tok[j] = (char*)malloc(MAXLINE*sizeof(char));
					tok[j]=tokens[i];
					//cout << tok[j]<< "pdfp ";
					
				}
			}
			if(tokens[i]== NULL) execute_command(tok);
			i++;
			if(error<0) break;	
		}
		//cout <<"done"<<endl;
		
		return 0 ;					// Return value accordingly
	}
	
	else if (!strcmp(tokens[0],"sequential_or")) {
		/* Analyze the command to get the jobs */
		/* Run jobs sequentially, print error on failure */
		/* Stop on failure or if all jobs are completed */
		int i=1;
		int error=-1;
		while(tokens[i]!= NULL){
			char** tok;
			tok = (char**)malloc(MAXLINE*sizeof(char));
			for(int j=0;tokens[i]!= NULL;j++,i++){
				if(strcmp(tokens[i],":::")==0){
					
				error=execute_command(tok);
				break;
				}	
				else{
					tok[j] = (char*)malloc(MAXLINE*sizeof(char));
					tok[j]=tokens[i];
					//cout << tok[j]<< "pdfp ";
					
				}
			}
			if(error!= -1) break;
			if(tokens[i]== NULL) execute_command(tok);
			i++;
				
		}
		//cout <<"done"<<endl;
		
		return 0 ;					// Return value accordingly
	}
	
	
	
	 else {
		/* Either file is to be executed or batch file to be run */
		/* Child process creation (needed for both cases) */
		int pid = fork() ;
		
		if (pid == 0) {
			if (!strcmp(tokens[0],"run")) {
				sleep(5);
				char * cstr = new char [100];
			 FILE *myfile;
			 myfile=fopen(tokens[1],"r");
			if (myfile == NULL) {
				perror ("Error opening file");
				return -1;
			}
			else {
				while( fgets (cstr, 100 , myfile) != NULL ){
				//puts (cstr);
				char** tok;
				tok = tokenize(cstr);
							//cout<<tok[0]<<" "<<tok[1]<<endl;
							execute_command(tok);
							char pwd[1024];
							getcwd(pwd, sizeof(pwd));
							string s(pwd);
							//cout <<"present:"<< s<< endl;// The prompt
			}
				fclose (myfile);
				}
			
				/* Locate file and run commands */
				/* May require recursive call to execute_command */
				/* Exit child with or without error */
				exit (0) ;
			}
			else {
				char **temp_tokens=(char **)malloc(MAXLINE*sizeof(char));
				int less=0,great=0,append=0,pos=0,pos_l=0,pos_g=0,pos_a=0;
				bool pipeline=false;
				for (int i=0;tokens[i]!=NULL;i++){
					if(strcmp(tokens[i],"|")==0){
						pipeline=true;
						pos=i;
					}
					if(strcmp(tokens[i],"<")==0){
						less++;
						pos_l=i;
					}
					if(strcmp(tokens[i],">")==0){
						great++;
						pos_g=i;
					}
					if(strcmp(tokens[i],">>")==0){
						append++;
						pos_a=i;
					}
				}
				if((!pipeline)&&(less==0)&&(great==0)&&(append==0)) {
					int res_file;
					res_file=execvp(tokens[0],tokens);
					if (res_file == -1){
						perror("file execution problem");
						return -1;
					}
					/* File Execution */
					/* Print error on failure, exit with error*/
					exit(0) ;
				}
				else if (!pipeline){
					for(int i=0;tokens[i]!= NULL;i++){
						if((!strcmp(tokens[i],"<"))||(!strcmp(tokens[i],">"))||(!strcmp(tokens[i],">>"))){
							for(int j=0;j<i;j++){
								temp_tokens[j] = (char*)malloc(MAXLINE*sizeof(char));
								temp_tokens[j]=tokens[j];
							}
						}
					}
					if (less>1){
						perror("too many < arguments");
						return -1;
					}
					if (great+append > 1){
						perror("too many > or >> arguments");
						return -1;
					}
					if (less>0) {
						in = fopen(tokens[pos_l+1], "ra");
						dup2(fileno(in),STDIN_FILENO);
						fclose(in);
					}
					if (great>0) {
						//cout << "text"<< endl;
						out = fopen(tokens[pos_g+1], "w+");
						dup2(fileno(out), STDOUT_FILENO);
						fclose(out);
					}
					if (append>0) {
						out = fopen(tokens[pos_a+1], "a");
						dup2(fileno(out), STDOUT_FILENO);
						fclose(out);
					}
					//cout << "reached"<<endl;
					int res_file;
					res_file=execvp(temp_tokens[0],temp_tokens);
					if (res_file == -1){
						perror("file execution problem");
						return -1;
					}
					/* File Execution */
					/* Print error on failure, exit with error*/
					exit(0) ;
				
				}
				else if (pipeline){
					int pipefd[2];
					pipe(pipefd);
					int i=0;
					char** tok1;
					char** tok2;
					tok1 = (char**)malloc(MAXLINE*sizeof(char));
					tok2 = (char**)malloc(MAXLINE*sizeof(char));
					int count1=0,count2=0;
					for(int j=0;tokens[i]!= NULL;j++,i++){
						if(j<pos){
							tok1[j] = (char*)malloc(MAXLINE*sizeof(char));
							tok1[j]=tokens[i];
							//cout << tok1[j]<<endl;
							count1++;
						}
						else if(j>pos){
							tok2[j-pos-1] = (char*)malloc(MAXLINE*sizeof(char));
							tok2[j-pos-1] =tokens[i];
							//cout << tok2[j-pos-1]<<endl;
							count2++;
						}
					}
					tok1[count1+1]=NULL;
					tok2[count2+1]=NULL;
					int pid=fork();
					if (pid==0){
						close(pipefd[0]);
						//cout << "done1"<< endl;
						dup2(pipefd[1], STDOUT_FILENO);
						// close unused hald of pipe
						
						execute_command(tok1);
						
					}
					else {	
						close(pipefd[1]);
						//cout << "done1"<< endl;
						dup2(pipefd[0],STDIN_FILENO);
						// close unused hald of pipe
						
						execute_command(tok2);
						
						waitpid(-1,NULL,0);
					}		
					exit(0);		
				}//cout << "done"<< endl;
							
			}
		}
		else {
			child_id.push_back(pid);
			int status;
			waitpid(pid,&status,0);
			if (status==1){
				perror("child process terminated with error");
			}
			/* Parent Process */
			/* Wait for child process to complete */
		}
	}
	return 0 ;
}
