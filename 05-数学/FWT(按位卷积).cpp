#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD=998244353,INV2=(MOD+1)/2;

// type：0 XOR，1 AND，2 OR；长度必须为 2 的幂，系数先归一到 [0,MOD)。
// XOR：(u+v,u-v)，逆变换每层乘 1/2；AND：(u+v,v)；OR：(u,u+v)。
// 与 FFT 不同，结果下标是 i^j / i&j / i|j，长度取 >=max(输入项数)。
void fwt(vector<ll> &a,int type,bool inverse=false)
{
    int n=a.size();
    assert(n>0&&(n&(n-1))==0&&0<=type&&type<=2);
    for(int len=1;len<n;len*=2)
        for(int i=0;i<n;i+=len*2)
            for(int j=0;j<len;j++)
            {
                ll u=a[i+j],v=a[i+j+len];
                if(type==0)
                {
                    a[i+j]=(u+v)%MOD,a[i+j+len]=(u-v+MOD)%MOD;
                    if(inverse)a[i+j]=a[i+j]*INV2%MOD,a[i+j+len]=a[i+j+len]*INV2%MOD;
                }
                else if(type==1)a[i+j]=(u+(inverse?MOD-v:v))%MOD;
                else a[i+j+len]=(v+(inverse?MOD-u:u))%MOD;
            }
}
// O(L log L)，返回补到 L 项后的卷积；XOR 逆变换要求 2 在模数下可逆。
vector<ll> bit_convolution(vector<ll> a,vector<ll> b,int type)
{
    if(a.empty() || b.empty())
        return {};
    int n=1;
    while(n<(int)max(a.size(),b.size()))n*=2;
    a.resize(n),b.resize(n);
    for(ll &x : a)
    {
        x%= MOD;
        if(x < 0)
            x+= MOD;
    }
    for(ll &x : b)
    {
        x%= MOD;
        if(x < 0)
            x+= MOD;
    }
    fwt(a,type),fwt(b,type);
    for(int i=0;i<n;i++)a[i]=a[i]*b[i]%MOD;
    fwt(a,type,true);
    return a;
}
