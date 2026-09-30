#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const double PI=acos(-1.0);

// O(n log n)，复数 FFT，系数规模需保证浮点舍入可靠
void fft(vector<complex<double> > &a,int inv)
{
    int n=a.size();
    for(int i=1,j=0;i<n;i++)
    {
        int bit=n>>1;
        for(;j&bit;bit>>=1)j^=bit;
        j^=bit;
        if(i<j)swap(a[i],a[j]);
    }
    for(int len=2;len<=n;len<<=1)
    {
        complex<double> wlen(cos(2*PI/len),inv*sin(2*PI/len));
        for(int i=0;i<n;i+=len)
        {
            complex<double> w(1,0);
            for(int j=0;j<len/2;j++,w*=wlen)
            {
                complex<double> u=a[i+j],v=a[i+j+len/2]*w;
                a[i+j]=u+v,a[i+j+len/2]=u-v;
            }
        }
    }
    if(inv==-1)for(complex<double> &x:a)x/=n;
}

vector<ll> multiply(vector<ll> a,vector<ll> b)
{
    if(a.empty()||b.empty())return {};
    int sz=a.size()+b.size()-1,n=1;
    while(n<sz)n<<=1;
    vector<complex<double> > x(n),y(n);
    for(int i=0;i<(int)a.size();i++)x[i]=a[i];
    for(int i=0;i<(int)b.size();i++)y[i]=b[i];
    fft(x,1),fft(y,1);
    for(int i=0;i<n;i++)x[i]*=y[i];
    fft(x,-1);
    vector<ll> ans(sz);
    for(int i=0;i<sz;i++)ans[i]=llround(x[i].real());
    return ans;
}
