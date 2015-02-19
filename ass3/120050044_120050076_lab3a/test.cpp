#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
int main(){
	
	/*char *grep_args[] = {"ls", NULL};
	FILE* in = fopen("text", "ra");
	FILE* out = fopen("output", "w+");
	dup2(fileno(in), fileno(stdin));
	dup2(fileno(out), fileno(stdout));
	fclose(in);
  fclose(out);
  execvp(grep_args[0], grep_args);
  */
  int b;
  cin>>b;
  cout<<"b="<<b<<endl;
}