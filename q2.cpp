#include <iostream>
#include <vector>
using namespace std;

struct nutrient{
	int pro;
	int carbo;
	int fat;
	nutrient(int a,int b,int c){
		pro=a;
		carbo=b;
		fat=c;
	}
};

bool func(vector<nutrient> &v,int index,nutrient desired){
	
	if (index > v.size()) return false;
	if (desired.pro==0&&desired.carbo==0&&desired.fat==0) return true;
	if (desired.pro<0||desired.carbo<0||desired.fat<0) return false;
	
	int a=v[index].pro;
	int b=v[index].carbo;
	int c=v[index].fat;
	nutrient new_desired=nutrient(desired.pro-a,desired.carbo-b,desired.fat-c);
	return func(v,index+1,desired)||func(v,index+1,new_desired);
}
int main(){
	int t;
	cin>>t;
	for (int i=1;i<=t;i++){
		int a,b,c;
		cin>>a>>b>>c;
		nutrient ans=nutrient(a,b,c);
		vector<nutrient> v;
		int n;
		cin>>n;
		for (int j=0;j<n;j++){
			cin>>a>>b>>c;
			nutrient temp=nutrient(a,b,c);
			v.push_back(temp);
		}
		bool result=func(v,0,ans);
		if (result)
		cout<<"Case #"<<i<<": yes"<<endl;
		else 
		cout<<"Case #"<<i<<": no"<<endl;
		
	}

}
