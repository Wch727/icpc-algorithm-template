#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=505;
const ll INF=1e18;
int n,m;
ll d[N][N];// 邻接矩阵，无边为 INF，d[i][i]=0
bool reach[N][N];// 传递闭包：i 能否到 j

void floyd()// 全源最短路 O(n^3)，允许负权边，不能有负环
{
    for(int k=1;k<=n;k++)
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(d[i][k]+d[k][j]<d[i][j])d[i][j]=d[i][k]+d[k][j];
}

void closure()// 传递闭包 O(n^3)
{
    for(int k=1;k<=n;k++)
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(reach[i][k]&&reach[k][j])reach[i][j]=1;
}
