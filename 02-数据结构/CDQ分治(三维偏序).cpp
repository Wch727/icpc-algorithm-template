// 若 q[j]=sum(k<=j)(j-k+1)*f[k]，则 f[j]=q[j]-2*q[j-1]+q[j-2]；二阶差分消前缀转移。
// 区间截断需补边界项；再判断剩余转移是否是卷积，才组合 CDQ+NTT。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 每个点统计其他满足 x<=x_i,y<=y_i,z<=z_i 的点，按原输入顺序返回。
// 相同三元组先合并，组内另加 w-1；不能直接拿重复点做 CDQ。
// x 排序后分治，跨左右统计 y，树状数组统计 z；O(n log²n)，空间 O(n)。
vector<array<int,3>> a;
vector<ll> dominance()
{
    struct Node{int x,y,z,w,id;ll ans;};
    int n=a.size();
    vector<int> order(n),group(n),zs;
    iota(order.begin(),order.end(),0);
    sort(order.begin(),order.end(),[&](int i,int j){return a[i]<a[j];});
    vector<Node> p;
    for(int i:order)
    {
        if(p.empty()||array<int,3>{p.back().x,p.back().y,p.back().z}!=a[i])
            p.push_back({a[i][0],a[i][1],a[i][2],0,(int)p.size(),0});
        ++p.back().w,group[i]=p.back().id;
    }
    for(auto v:p)zs.push_back(v.z);
    sort(zs.begin(),zs.end());
    zs.erase(unique(zs.begin(),zs.end()),zs.end());
    for(auto &v:p)v.z=lower_bound(zs.begin(),zs.end(),v.z)-zs.begin()+1;
    vector<int> bit(zs.size()+1);
    auto add=[&](int x,int v){for(;x<(int)bit.size();x+=x&-x)bit[x]+=v;};
    auto sum=[&](int x){int s=0;for(;x;x-=x&-x)s+=bit[x];return s;};
    auto cdq=[&](auto &&self,int l,int r)->void
    {
        if(r-l<=1)return;
        int m=(l+r)/2;
        self(self,l,m),self(self,m,r);
        int i=l;
        for(int j=m;j<r;j++)
        {
            while(i<m&&p[i].y<=p[j].y)add(p[i].z,p[i].w),++i;
            p[j].ans+=sum(p[j].z);
        }
        for(int j=l;j<i;j++)add(p[j].z,-p[j].w);
        inplace_merge(p.begin()+l,p.begin()+m,p.begin()+r,
            [](Node a,Node b){return a.y<b.y;});
    };
    cdq(cdq,0,p.size());
    vector<ll> count(p.size()),ans(n);
    for(auto v:p)count[v.id]=v.ans+v.w-1;
    for(int i=0;i<n;i++)ans[i]=count[group[i]];
    return ans;
}
