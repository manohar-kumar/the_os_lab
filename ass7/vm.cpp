#include <iostream>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <string>
#include <list>
#include <map>
using namespace std;
struct pageTableEntry {
	bool valid;
	int frameNo;
//	char Prot;
	int ref; 
	bool modified;
	pageTableEntry(){
			valid=false;
			modified=false;
	}
	
};
struct proc{
	int pid;
	int processSize;
	int M;
	int N;
	int *frames;
	int *LRU;
	pageTableEntry *pageTable;
	proc(int pid1,int processSize1,int M1,int N1){
		pid=pid1;
		processSize=processSize1;
		M=M1;
		N=N1;
		pageTable=new pageTableEntry[processSize];
		frames=new int[N-M+1];
		LRU=new int[N-M+1];
		for(int i=0;i<N-M+1;i++){
			frames[i]=-1;
			LRU[i]=-1;
		}
	}
	proc(){
		
	}
};
void shift(int A[],int i,int pageNo,int size){
	for(int j=i;j<size;j++){
		if(A[j+1]==-1) {
			A[j]=pageNo;
			break;
		}
		A[j]=A[j+1];
		
	}
	return;
}

map<int,proc> proclist;

int page_fault_handler(int pid,int pageNo){
	proc* temp= new proc();
	temp= &proclist[pid];
	for(int i=0;i<temp->N-temp->M+1;i++){
		
		if(temp->frames[i]==-1) {
			//cout<<"entered"<<endl;
			temp->frames[i]=pageNo;
			temp->pageTable[pageNo].valid=true;
			temp->pageTable[pageNo].frameNo=i;
			for(int j=0;j<temp->N-temp->M+1;j++){
				//cout<<"fzfzg"<<endl;
				if(temp->LRU[j]== -1) {
					shift(temp->LRU,j,pageNo,temp->N-temp->M+1);
					break;
				}
			}
			
			cout << "Loaded page "<<pageNo<<" into frame "<<i<< endl;
			return i;
		}
	}
	int k=temp->LRU[0];
	temp->pageTable[k].valid=false;
	temp->frames[temp->pageTable[k].frameNo]=pageNo;
	temp->pageTable[pageNo].valid=true;
	temp->pageTable[pageNo].frameNo=temp->pageTable[k].frameNo;
	cout<<"Removed page "<<k<<" from frame"<<temp->pageTable[k].frameNo<<endl;
	cout << "Loaded page "<<pageNo<<" into frame "<<temp->pageTable[k].frameNo<< endl;
	shift(temp->LRU,0,pageNo,temp->N-temp->M+1);
	
}

int mmu(int pid, int pageNo){
	proc* temp=new proc();
	temp= &proclist[pid];
	
	if(pageNo>=temp->processSize){
		return 0;
	}
	if(!temp->pageTable[pageNo].valid) {
		return -1;
	}
	else {
		for(int i=0;i<temp->N-temp->M+1;i++){
			if(temp->LRU[i]==pageNo) {
				shift(temp->LRU,i,pageNo,temp->N-temp->M+1);
				break;
			}
		}
		return 1;
	}
}

void process(int pid,int processSize,int M,int N){
	string s="s";
	string line;
	//cout<<"adsff"<<endl;
	s=s + to_string(pid);
	ifstream spec (s);
	int access=0,modify=0,pageFault=0;
	
	if (!spec.is_open()) {
		cout <<"Error opening file";
		return;
		}
		
	else{
		while ( getline (spec,line) ){
			string command,end;
			end=line.substr(0,3);
			if(end=="end"){
				cout<<"Number of access operations : "<<access<<endl;
				cout<<"Number of modify operations : "<<modify<<endl;
				cout<<"Number of pafeFault operations : "<<pageFault<<endl;
				return;
			}
			command=line.substr(0,6);
			if(command=="access") {
				int pageNo,wordNo;
				pageNo= stoi(line.substr(7,3));
				wordNo= stoi(line.substr(11,4));
				int status=mmu(pid,pageNo);
				cout <<"process number: "<<pid <<" attempted to access "<<pageNo<<" "<<wordNo<<endl;
				if(status==1){
					access++;
				}
				if(status==0){
					cout<<"Reported a memory protection violation"<<endl;
				}
				if(status==-1){
					pageFault++;
					
					cout<<"Reported a page fault"<<endl;
					page_fault_handler(pid,pageNo);
				}
				//cout<<"Accessed page frame number "<<endl;
			}
			else if(command=="modify") {
				int pageNo,wordNo;
				pageNo= stoi(line.substr(7,3));
				wordNo= stoi(line.substr(11,4));
				int status=mmu(pid,pageNo);
				cout <<"process number: "<<pid <<" attempted to modify "<<pageNo<<" "<<wordNo<<endl;
				if(status==1){
					modify++;
				}
				if(status==0){
					cout<<"Reported a memory protection violation"<<endl;
				}
				if(status==-1){
					pageFault++;
					
					cout<<"Reported a page fault"<<endl;
					page_fault_handler(pid,pageNo);
				}
				cout<<"Accessed page frame number ";
			}
			else{
				cout<<"command not found";
				return;
			}
			proc* temp=new proc();
			temp= &proclist[pid];
			for(int i=0;i<temp->N-temp->M+1;i++){
			cout<<temp->LRU[i];
			}
			cout<<endl;
		}
		
	}	
}

int main() {
	string line;
	ifstream init ("init");
	if (!init.is_open()) {
		cout <<"Error opening file";
		return -1;
		}
		
	else {
		
		while ( getline (init,line) ){
			string command;
			
		//	cout<<line.substr(0,6)<<endl;
			command=line.substr(0,6);
			
			if(command=="create") {
				
				int pid,processSize,M,N;
				pid= stoi(line.substr(7,2));
				processSize= stoi(line.substr(10,3));
				M= stoi(line.substr(14,3));
				N= stoi(line.substr(18,3));
				proc p(pid,processSize,M,N);
				proclist[pid]=p;
		
				thread Process(process,pid,processSize,M,N);
				Process.join();
			}
			else{
				cout<<"command not found";
				return -1;
			}
		}
		
	}
}
