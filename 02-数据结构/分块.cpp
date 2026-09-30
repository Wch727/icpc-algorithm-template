#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 分块：区间加 + 区间和，块长 sqrt(n)
// update/query 复杂度 O(sqrt(n))：散块暴力，整块用 tag/sum
template<typename T,int NMAX=N>
struct Block
{
    T a[NMAX],tag[NMAX],sum[NMAX];
    int id[NMAX],L[NMAX],R[NMAX],B,cnt,n;
    void init(int n_,T *src)//用 1..n_ 的原数组建块
    {
        n=n_;
        B=max(1,(int)sqrt((double)n));//块长
        cnt=(n-1)/B+1;
        for(int i=1;i<=cnt;i++)
        {
            L[i]=(i-1)*B+1;
            R[i]=min(i*B,n);
            tag[i]=0,sum[i]=0;
        }
        for(int i=1;i<=n;i++)
        {
            a[i]=src[i];
            id[i]=(i-1)/B+1;
            sum[id[i]]+=a[i];
        }
    }
    void update(int l,int r,T c)
    {
        if(id[l]==id[r])//同块
        {
            for(int i=l;i<=r;i++)a[i]+=c,sum[id[l]]+=c;
            return;
        }
        for(int i=l;i<=R[id[l]];i++)a[i]+=c,sum[id[l]]+=c;
        for(int i=L[id[r]];i<=r;i++)a[i]+=c,sum[id[r]]+=c;
        for(int i=id[l]+1;i<id[r];i++)tag[i]+=c,sum[i]+=(T)(R[i]-L[i]+1)*c;
    }
    T query(int l,int r)
    {
        T s=0;
        if(id[l]==id[r])
        {
            for(int i=l;i<=r;i++)s+=a[i]+tag[id[l]];
            return s;
        }
        for(int i=l;i<=R[id[l]];i++)s+=a[i]+tag[id[l]];
        for(int i=L[id[r]];i<=r;i++)s+=a[i]+tag[id[r]];
        for(int i=id[l]+1;i<id[r];i++)s+=sum[i];
        return s;
    }
};

int n;
ll a[205],br[205];
Block<ll,205> blk;

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：随机区间加 + 随机区间和，与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=30;t++)
    {
        n=rnd(1,80);
        for(int i=1;i<=n;i++)a[i]=rnd(-50,50),br[i]=a[i];
        blk.init(n,a);
        for(int q=1;q<=200;q++)
        {
            int op=rnd(1,2),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                ll v=rnd(-30,30);
                blk.update(l,r,v);
                for(int i=l;i<=r;i++)br[i]+=v;
            }
            else
            {
                ll x=blk.query(l,r),z=0;
                for(int i=l;i<=r;i++)z+=br[i];
                cnt++;
                if(x!=z)bad++;
            }
        }
    }
    printf("分块(区间加区间和) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,2,3,4,5]，[2,4] 加 10 后 sum[1,5]=25、sum[3,3]=13
    n=5;
    for(int i=1;i<=5;i++)a[i]=i;
    blk.init(n,a);
    blk.update(2,4,10);
    printf("小样例: sum[1,5]=%lld sum[3,3]=%lld\n",blk.query(1,5),blk.query(3,3));
    return 0;
}
