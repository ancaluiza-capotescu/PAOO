#include <bits/stdc++.h>
using namespace std;

int main()
{
	int a[]={23,4,1,-3,10};
	int i,j;
	for(i=0;i<5;i++)
		for(j=i+1;j<5;j++)
			if(a[i]>a[j])
				swap(a[i],a[j]);
	for(i=0;i<5;i++)
		cout<<a[i]<<" ";
	cout<<endl;
	return 0;
}
