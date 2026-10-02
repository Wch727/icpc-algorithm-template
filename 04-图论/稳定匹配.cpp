// Gale–Shapley：两侧各 n 人，严格、完整偏好列表，编号 0..n-1，越前越喜欢。
// 返回左侧的伴侣；O(n²)，提议的左侧在所有稳定匹配中最优。目标不是最大总权。
#include<bits/stdc++.h>
using namespace std;
vector<int> stable_matching(const vector<vector<int>> &a,const vector<vector<int>> &b)
{
    int n=a.size();assert((int)b.size()==n);
    vector<vector<int>> rank(n,vector<int>(n));
    for(int j=0;j<n;j++)for(int k=0;k<n;k++)rank[j][b[j][k]]=k;
    vector<int> next(n),left(n,-1),right(n,-1);queue<int> q;
    for(int i=0;i<n;i++)q.push(i);
    while(!q.empty())
    {
        int i=q.front();q.pop();int j=a[i][next[i]++],old=right[j];
        if(old<0||rank[j][i]<rank[j][old])
        {left[i]=j;right[j]=i;if(old>=0)left[old]=-1,q.push(old);}
        else q.push(i);
    }
    return left;
}
