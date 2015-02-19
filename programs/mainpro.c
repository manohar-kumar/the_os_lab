#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <vector>
#include <algorithm> 
#include <signal.h>
#include <sys/types.h>
#include <unordered_map>
#include <string.h>

#include <map>
#include <string>

using namespace std;
struct childtype{
	char* operation;
	char* email;
	char* domain;
	char* message;
	childtype(){

	}
};

int ShmID = shmget(IPC_PRIVATE,sizeof(childtype),IPC_CREAT|0666);
childtype *tokens=new childtype();
void SIGUSR1_handler(int sig){
cout<<tokens->operation<<" "<<tokens->email<<" "<<tokens->message<<endl;
}

void SIGUSR2_handler(int sig){

	cout<<"handler"<<endl;
	static vector<char *> email_list;
	cout<<"checking"<<tokens->operation<<endl;
	if (strcmp(tokens->operation,"delete_email")==0){
			vector<char *> :: iterator it=email_list.begin();
			int i=0,found=0;
			string email(tokens->email);
			string domain(tokens->domain);
			for(it;it!=email_list.end();it++,i++) {
				if(strcmp(tokens->email,*it)==0){
					email_list.erase(it);
					found=1;
					string message= "Child process - child "+domain+ " deleted "+email+" from position"+(char)i;
					tokens->message= new char[message.length() + 1];
					strcpy(tokens->message, message.c_str());
				}
			}
			if(found==0) {
				string message= "Parent process - child "+domain+ " could not find email address "+email;
					tokens->message= new char[message.length() + 1];
					strcpy(tokens->message, message.c_str());
				}
	}
	if (strcmp(tokens->operation,"search_email")==0){
			vector<char *> :: iterator it=email_list.begin();
			int i=0,found=0;
			string email(tokens->email);
			string domain(tokens->domain);
			for(it;it!=email_list.end();it++,i++) {
				if(strcmp(tokens->email,*it)==0) {
					found=1;
					string message= "Parent process found the emain adress "+email+" from position"+(char)i;
					tokens->message= new char[message.length() + 1];
					strcpy(tokens->message, message.c_str());
				}
			}
			if(found==0) {
				string message= "Parent process could not find the emain adress "+email;
					tokens->message= new char[message.length() + 1];
					strcpy(tokens->message, message.c_str());
				}
	}

	if (strcmp(tokens->operation,"add_email")==0){
		cout<<"here"<<endl;
		vector<char *>::iterator it;
		cout<<tokens->email<<endl;
			it=find(email_list.begin(),email_list.end(),tokens->email);
			if (it!=email_list.end()){
				cout<<"email already there"<<endl;
			}
			else {
				cout<<"email added"<<endl;
				email_list.push_back(tokens->email);
				
			}

	}
	else {
		cout<<"some command"<<endl;
	}
	kill(getppid(),SIGUSR1);

}


void Child(childtype* tokens)
{
while(1){

	sleep(10);
}
}
int main(int argc, char** argv){

struct sigaction sa;
memset(&sa,0,sizeof(sa));
sa.sa_handler=&SIGUSR2_handler;
sigaction(SIGUSR2,&sa,NULL);
struct sigaction sa1;
memset(&sa1,0,sizeof(sa1));
sa1.sa_handler=&SIGUSR1_handler;
sigaction(SIGUSR1,&sa1,NULL);


pid_t pid;


unordered_map <string,int> domains;
domains.insert(make_pair("a.com",4567));
tokens = (struct childtype *) shmat(ShmID, NULL, 0);  //attaching to shared memory
string input;	
while (true){		cout<<"first"<<endl;				//main loop for taking input
 getline(cin,input);
 int u=input.find(" "); 
 	string op=input.substr(0,u);
	string a=input.substr(u+1,input.size());
	cout<<a<<endl;
	int t=a.find("@");
	string dom=a.substr(t+1,a.size());
	unordered_map<string,int>::const_iterator got = domains.find (dom); //searching for domains in map
	if (op=="delete_domain"){
		if (got==domains.end()){
			cout<<"Parent process - Domain does not exist."<<endl;
		}
		else {
			domains.erase(dom);
			cout<<"yolo";
			
			continue;
		}
	}
	 if (got == domains.end()){
	 	if (op=="add_email"){}
	 	else{
	 		string email(tokens->email);
			string domain(tokens->domain);
			string message= "Parent process-Domain does not exist";
			tokens->message= new char[message.length() + 1];
			strcpy(tokens->message, message.c_str());
			//kill(getpid(),SIGUSR1);
	 	}
	 	char* s2 = (char *)alloca(a.substr(t+1,a.size()).size() + 1);
    	memcpy(s2, a.substr(t+1,a.size()).c_str(), a.substr(t+1,a.size()).size() + 1);
    	char* s3=(char *)alloca(a.substr(0,t).size()+1);
    	memcpy(s3,a.substr(0,t).c_str(),a.substr(0,t).size()+1);
		(*tokens).domain= s2;

		(*tokens).email= s3;

		cout<<"email->"<<(*tokens).email<<endl;
		char * cstr = new char [op.length()+1];
		strcpy (cstr, op.c_str());
		(*tokens).operation=cstr;
		pid=fork();
		if (pid>0){
			cout<<"inserting into map"<<endl;
			domains.insert(make_pair(a.substr(t+1,a.size()),pid));
			kill(pid,SIGUSR2);
		}	
		else if (pid== 0) {
			Child(tokens);
			exit(0);
		}
	}
	else {
		char* s2 = (char *)alloca(a.substr(t+1,a.size()).size() + 1);
    	memcpy(s2, a.substr(t+1,a.size()).c_str(), a.substr(t+1,a.size()).size() + 1);
    	char* s3=(char *)alloca(a.substr(0,t).size()+1);
    	memcpy(s3,a.substr(0,t).c_str(),a.substr(0,t).size()+1);
		(*tokens).domain= s2;

		(*tokens).email= s3;
		cout<<"email->"<<(*tokens).email<<endl;
		char * cstr = new char [op.length()+1];
		strcpy (cstr, op.c_str());
		cout<<"now"<<cstr<<endl;
		tokens->operation=cstr;
	 	cout<<"process_id for domain name "<<got->first<<" is "<<got->second<<endl;
	 	kill(got->second,SIGUSR2);
	 }
	sleep(10);
}
shmdt((void *) tokens); shmctl(ShmID, IPC_RMID, NULL);
exit(0);
}

