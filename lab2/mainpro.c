#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

#include <map>
#include <string>

using namespace std;

struct childtype{
	string domain;
	string email;
};
void Child(struct childtype* tokens)
{

		cout<<tokens->domain<<" eefd "<<tokens->email;
	  
}
int main(int argc, char** argv){
cout<<"dfsdfds";
childtype* tokens;
int i =6;
int ShmID;

pid_t pid;
ShmID = shmget(IPC_PRIVATE,sizeof(childtype),IPC_CREAT|0666);

map <string,int> domains;
tokens = (childtype *) shmat(ShmID, NULL, 0);
string input;
 cin>>input;
 int u=input.find(" ");

if (input.substr(0,u).compare("add_email")==0){
	string a=input.substr(u+1,input.size()-1);
	cout<<a<<endl;
	int t=a.find("@");

	tokens->domain= a.substr(t+1,a.size()-1);
	tokens->email= a.substr(0,t);

	pid=fork();
	if (pid== 0) {
	Child(tokens);
	exit(0);
}

}
else {
	printf("%s","wrong command");
}

shmdt((void *) tokens); shmctl(ShmID, IPC_RMID, NULL);
exit(0);
}

void add_email(char **input){

}