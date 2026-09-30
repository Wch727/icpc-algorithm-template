// NTT 的测试与对拍代码
// 模板本体：05-数学/NTT.cpp
#include "../../05-数学/NTT.cpp"

int main()
{
    int bad=0;
    mt19937_64 rnd(20250909);
    // 1) 小规模与暴力卷积对拍
    for(int t=1;t<=300;t++)
    {
        int n=rnd()%40+1,m=rnd()%40+1;
        static ll a[N],b[N],c[N],d[N];
        for(int i=0;i<n;i++)a[i]=rnd()%mod;
        for(int i=0;i<m;i++)b[i]=rnd()%mod;
        int cl,dl;
        poly_mul(a,n,b,m,c,cl);
        mul_naive(a,n,b,m,d,dl);
        if(cl!=dl)bad++;
        for(int i=0;i<cl;i++)
            if(c[i]!=d[i])bad++;
        // 2) 自卷积也验一遍
        poly_mul(a,n,a,n,c,cl);
        mul_naive(a,n,a,n,d,dl);
        for(int i=0;i<cl;i++)
            if(c[i]!=d[i])bad++;
    }
    // 3) 大整数乘法风格：把十进制数按位拆开做卷积（对拍大数乘）
    // 987654321 * 123456789 = 121932631112635269
    string s1="987654321",s2="123456789";
    static ll A[N],B[N],C[N];
    int n1=s1.size(),n2=s2.size();
    for(int i=0;i<n1;i++)A[i]=s1[n1-1-i]-'0';
    for(int i=0;i<n2;i++)B[i]=s2[n2-1-i]-'0';
    int clen;
    poly_mul(A,n1,B,n2,C,clen);
    // 处理进位
    ll carry=0;
    for(int i=0;i<clen;i++)
    {
        ll t=C[i]+carry;
        C[i]=t%10;
        carry=t/10;
    }
    while(carry){C[clen++]=carry%10;carry/=10;}
    string res;
    for(int i=clen-1;i>=0;i--)res+=char('0'+C[i]);
    if(res!="121932631112635269")bad++;
    printf("%s * %s = %s\n",s1.c_str(),s2.c_str(),res.c_str());
    // 4) 长度 1e5 的多项式乘法（性能与正确性抽检）
    int big=100000;
    static ll p[N],q[N],r[N];
    for(int i=0;i<N;i++)p[i]=0,q[i]=0;// poly_mul 会把数组读到 len，必须清干净
    for(int i=0;i<big;i++)p[i]=i+1,q[i]=big-i;
    int rl;
    poly_mul(p,big,q,big,r,rl);
    ll want0=1LL*big%mod;
    if(r[0]!=want0)bad++;
    // 一次项可手算核对：c[1]=p[0]q[1]+p[1]q[0]=1*(big-1)+2*big=3*big-1
    ll y1=(3LL*big-1)%mod;
    if(r[1]!=y1)bad++;
    printf("deg 1e5 convolution: c[0]=%lld c[1]=%lld (expect %lld %lld)\n",r[0],r[1],want0,y1);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1919 高精度乘法 A*B，位数 <= 1e6，转成多项式卷积 + 进位
// 样例：P3807 Lucas 与多项式无关，但同属数论组合工具
// 边界：len 必须是 2 的幂且不超过 2^23；逆变换最后要乘 n 的逆元

/*
自测记录：
  1) 300 组随机长度 <= 40 的多项式，与 O(n^2) 暴力卷积对拍，另加自卷积；
  2) 高精度乘法 987654321*123456789 与十进制真值比对；
  3) 长度 1e5 的卷积，用解析式验证 c[0] 与 c[1]。
*/
