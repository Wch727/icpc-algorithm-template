// 静态区间第 k 小与 <=x 计数，0-indexed 半开区间 [l,r)，k 从 0 开始。
// 离散化后稳定分流，构建 O(n log σ)，查询 O(log σ)，空间 O(n log σ)。不支持点修改。
#include<bits/stdc++.h>
using namespace std;
struct WaveletTree
{
    struct Node{int lo,hi,left=-1,right=-1;vector<int> pref;};
    vector<Node> t;vector<long long> value;int root=-1,n;
    int build(vector<int> a,int lo,int hi)
    {
        int p=t.size();t.push_back({lo,hi,-1,-1,{0}});if(lo==hi)return p;
        vector<int> l,r;int m=(lo+hi)/2;
        for(int x:a){if(x<=m)l.push_back(x);else r.push_back(x);t[p].pref.push_back(l.size());}
        if(!l.empty()){int c=build(l,lo,m);t[p].left=c;}
        if(!r.empty()){int c=build(r,m+1,hi);t[p].right=c;}
        return p;
    }
    WaveletTree(const vector<long long> &a):value(a),n(a.size())
    {
        sort(value.begin(),value.end());value.erase(unique(value.begin(),value.end()),value.end());
        vector<int> id;for(auto x:a)id.push_back(lower_bound(value.begin(),value.end(),x)-value.begin());
        if(n)root=build(id,0,value.size()-1);
    }
    long long kth(int l,int r,int k)const
    {
        assert(0<=l&&l<=r&&r<=n&&0<=k&&k<r-l);int p=root;
        while(t[p].lo<t[p].hi){int a=t[p].pref[l],b=t[p].pref[r];if(k<b-a)p=t[p].left,l=a,r=b;else k-=b-a,p=t[p].right,l-=a,r-=b;}
        return value[t[p].lo];
    }
    int count(int p,int l,int r,int x)const
    {if(p<0||l==r||x<t[p].lo)return 0;if(t[p].hi<=x)return r-l;int a=t[p].pref[l],b=t[p].pref[r];return count(t[p].left,a,b,x)+count(t[p].right,l-a,r-b,x);}
    int count_le(int l,int r,long long x)const{assert(0<=l&&l<=r&&r<=n);int id=upper_bound(value.begin(),value.end(),x)-value.begin()-1;return count(root,l,r,id);}
};
