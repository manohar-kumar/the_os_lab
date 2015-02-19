#include <iostream>
#include <vector>
using namespace std;
int main(){
	int t;
	cin>>t;
	for (int j=1;j<=t;j++){
		vector<int> ans;
		int n,minloc,maxloc,min=10,max=0,count=1;
		cin>>n;
		while(true){
			if (n==0) break;
			int temp=n%10;
			if (temp<min&&temp!=0) {min=temp;
				minloc=count;}
			if (temp>max) {max=temp;
				maxloc=count;}
			ans.push_back(temp);
			n=n/10;
			count++;
			}
			if (count==1){
				cout<<"Case #"<<j<<": "<<0<<" "<<0<<endl;
				
			}
			else{
		int temp=ans[minloc-1];
		ans[minloc-1]=ans[ans.size()-1];
		ans[ans.size()-1]=temp;
		min=0;
		for (int i=ans.size()-1;i>=0;i--){
			min=min*10+ans[i];
		}
		temp=ans[minloc-1];
		ans[minloc-1]=ans[ans.size()-1];
		ans[ans.size()-1]=temp;
		
		//for maximum
		temp=ans[maxloc-1];
		ans[maxloc-1]=ans[ans.size()-1];
		ans[ans.size()-1]=temp;
		max=0;
		for (int i=ans.size()-1;i>=0;i--){
			max=max*10+ans[i];
		}
		cout<<"Case #"<<j<<": "<<min<<" "<<max<<endl;
	}
}
		}
