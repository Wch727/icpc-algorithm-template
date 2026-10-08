// 适用：整数多项式卷积；结果以 llround 还原整数，系数太大会有精度误差。
// 下标：a[i]/b[i] 对应 x^i，0-indexed；空多项式相乘返回空。
// 参数：fft 的 inv=1 正变换、inv=-1 逆变换，不是 0/1 约定。
// 关键：长度补到 >=a.size()+b.size()-1 的 2 的幂，避免循环卷积混叠。
// 易错：double 约 53 位有效二进制位，负系数也要舍入；高精度或大系数考虑拆分或 NTT。
// 复杂度：O(L log L) 时间、O(L) 空间；输入按值传递，不修改调用者数组。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const double PI=acos(-1.0);

// O(n log n)，复数 FFT，系数规模需保证浮点舍入可靠
// O(L log L)，原地变换长度 L 的复数向量，L>=1 且为 2 的幂；逆变换最后除 L。
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
    if(inv == -1)
        for(complex<double> &x : a)
            x/= n;
}

// O(L log L)，输出 a.size()+b.size()-1 个整数系数；点乘频域等价于时域卷积。
vector<ll> multiply(vector<ll> a,vector<ll> b)
{
    if(a.empty() || b.empty())
        return {};
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
