#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int M=1005;//二维树状数组的默认边长

// 树状数组：单点加 + 前缀和/区间和，O(log n)
template<typename T,int NMAX=N>
struct BIT
{
    T tr[NMAX];
    int n;
    void init(int n_)//下标 1..n_
    {
        n=n_;
        for(int i=1;i<=n;i++)tr[i]=0;
    }
    void add(int x,T v)
    {
        for(;x<=n;x+=x&-x)tr[x]+=v;
    }
    T sum(int x)//前缀和 [1,x]
    {
        T s=0;
        for(;x>0;x-=x&-x)s+=tr[x];
        return s;
    }
    T query(int l,int r)//区间和 [l,r]
    {
        if(l>r)return 0;
        return sum(r)-sum(l-1);
    }
    // 求最小的 x 使前缀和 >= k(要求 tr 全程非负)，不存在返回 n+1，O(log n)
    int kth(T k)
    {
        int p=0,lg=1;
        T s=0;
        while((lg<<1)<=n)lg<<=1;//不超过 n 的最大 2 的幂
        for(int j=lg;j;j>>=1)
            if(p+j<=n&&s+tr[p+j]<k)s+=tr[p+j],p+=j;
        return p+1;
    }
};

// 区间加 + 单点查：差分树状数组，update/query 都是 O(log n)
template<typename T,int NMAX=N>
struct DiffBIT
{
    BIT<T,NMAX> c;
    int n;
    void init(int n_)
    {
        n=n_,c.init(n_);
    }
    void update(int l,int r,T v)
    {
        c.add(l,v);
        if(r+1<=n)c.add(r+1,-v);//边界：r=n 时右端点不越界
    }
    T query(int x)
    {
        return c.sum(x);
    }
};

// 二维树状数组：单点加 + 子矩阵和，O(log n log m)
template<typename T,int NMAX=M>
struct BIT2D
{
    T tr[NMAX][NMAX];
    int n,m;
    void init(int n_,int m_)
    {
        n=n_,m=m_;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)tr[i][j]=0;
    }
    void add(int x,int y,T v)
    {
        for(int i=x;i<=n;i+=i&-i)
            for(int j=y;j<=m;j+=j&-j)tr[i][j]+=v;
    }
    T sum(int x,int y)//左上角前缀和
    {
        T s=0;
        for(int i=x;i>0;i-=i&-i)
            for(int j=y;j>0;j-=j&-j)s+=tr[i][j];
        return s;
    }
    T query(int x1,int y1,int x2,int y2)//子矩阵和
    {
        return sum(x2,y2)-sum(x1-1,y2)-sum(x2,y1-1)+sum(x1-1,y1-1);
    }
};

int n;
int a[N],tmp[N];
BIT<int> bit_inv;

// 离散化求逆序对对数，O(n log n)
ll calc_inv(int len)
{
    for(int i=1;i<=len;i++)tmp[i]=a[i];
    sort(tmp+1,tmp+len+1);
    int tot=unique(tmp+1,tmp+len+1)-tmp-1;
    bit_inv.init(tot);
    ll ans=0;
    for(int i=1;i<=len;i++)
    {
        int rk=lower_bound(tmp+1,tmp+tot+1,a[i])-tmp;
        ans+=i-1-bit_inv.sum(rk);//前面比 a[i] 严格大的个数
        bit_inv.add(rk,1);
    }
    return ans;
}

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：五种用法全部与暴力对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    // 1. 单点加 + 区间和
    BIT<ll,105> bit;
    ll b1[105];
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,60);
        bit.init(n);
        for(int i=1;i<=n;i++)b1[i]=0;
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,3),x=rnd(1,n),y=rnd(1,n);
            if(op<=2)
            {
                ll v=rnd(-20,20);
                bit.add(x,v),b1[x]+=v;
            }
            else
            {
                if(x>y)swap(x,y);
                ll s=bit.query(x,y),z=0;
                for(int i=x;i<=y;i++)z+=b1[i];
                cnt++;
                if(s!=z)bad++;
            }
        }
    }
    // 2. 差分：区间加 + 单点查
    DiffBIT<ll,105> db;
    ll b2[105];
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,60);
        db.init(n);
        for(int i=1;i<=n;i++)b2[i]=0;
        for(int q=1;q<=200;q++)
        {
            int x=rnd(1,n),y=rnd(1,n),op=rnd(1,2);
            if(x>y)swap(x,y);
            if(op==1)
            {
                ll v=rnd(-20,20);
                db.update(x,y,v);
                for(int i=x;i<=y;i++)b2[i]+=v;
            }
            else
            {
                cnt++;
                if(db.query(x)!=b2[x])bad++;
            }
        }
    }
    // 3. 求第 k 小(值域 1..n 的权值树)
    int cntb[105];
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,50);
        BIT<int,105> c;
        c.init(n);
        for(int i=1;i<=n;i++)cntb[i]=0;
        for(int q=1;q<=200;q++)
        {
            if(rnd(1,2)==1)
            {
                int x=rnd(1,n),v=rnd(1,3);
                c.add(x,v),cntb[x]+=v;
            }
            else
            {
                int tot=0;
                for(int i=1;i<=n;i++)tot+=cntb[i];
                int k=rnd(1,tot+1);//k=tot+1 时应该返回 n+1
                int expect=n+1,s=0;
                for(int i=1;i<=n;i++)
                {
                    s+=cntb[i];
                    if(s>=k){expect=i;break;}
                }
                cnt++;
                if(c.kth(k)!=expect)bad++;
            }
        }
    }
    // 4. 二维树状数组
    BIT2D<int,55> b2d;
    int br2[55][55];
    for(int t=1;t<=5;t++)
    {
        int nn=rnd(1,20),mm=rnd(1,20);
        b2d.init(nn,mm);
        for(int i=1;i<=nn;i++)
            for(int j=1;j<=mm;j++)br2[i][j]=0;
        for(int q=1;q<=200;q++)
        {
            if(rnd(1,2)==1)
            {
                int x=rnd(1,nn),y=rnd(1,mm),v=rnd(-9,9);
                b2d.add(x,y,v),br2[x][y]+=v;
            }
            else
            {
                int x1=rnd(1,nn),x2=rnd(1,nn),y1=rnd(1,mm),y2=rnd(1,mm);
                if(x1>x2)swap(x1,x2);
                if(y1>y2)swap(y1,y2);
                int s=b2d.query(x1,y1,x2,y2),z=0;
                for(int i=x1;i<=x2;i++)
                    for(int j=y1;j<=y2;j++)z+=br2[i][j];
                cnt++;
                if(s!=z)bad++;
            }
        }
    }
    // 5. 离散化逆序对
    for(int t=1;t<=10;t++)
    {
        int len=rnd(1,60);
        for(int i=1;i<=len;i++)a[i]=rnd(1,20);
        ll s=calc_inv(len),z=0;
        for(int i=1;i<=len;i++)
            for(int j=i+1;j<=len;j++)
                if(a[i]>a[j])z++;
        cnt++;
        if(s!=z)bad++;
    }
    printf("树状数组(单点改/差分/第k小/二维/逆序对) vs 暴力: %s, 校验=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[3,1,2] 逆序对为 2；权值树 {0,1,1,3} 第 2 小是 3
    a[1]=3,a[2]=1,a[3]=2;
    BIT<int,10> c;
    c.init(4);
    c.add(2,1),c.add(3,1),c.add(4,3);
    printf("小样例: 逆序对=%lld, 权值树第2小=%d, 前缀和[1,3]=%d\n",calc_inv(3),c.kth(2),c.query(1,3));
    return 0;
}
