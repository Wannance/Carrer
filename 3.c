/***************************************
求矩阵中最大的子矩阵
***************************************/
#include<bits/stdc++.h>
#include<iostream>

using namespace std;
int main()
{
    int n,a[101][101],i,j,r,sum[101],s=-99999999,t1;
    cin>>n;
    for(i=1;i<=n;i++)
      for(j=1;j<=n;j++)
      {
         cin>>a[i][j];
         a[i][j]+=a[i-1][j];
      }
      for(i=1;i<=n;i++)
      {
        for(j=i;j<=n;j++)
        {
            int t=-99999999;
            for(r=1;r<=n;r++)
            {
                 t1=a[j][r]-a[i-1][r];
                 sum[r]=max(sum[r-1]+t1,t1);
                 t=max(sum[r],t);
              }
               s=max(s,t);
          }
      }
      cout<<s;
      return 0;

}