#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;

// 种类并查集(3n 拆点)：x 同类域 / x+n x 的猎物 / x+n*2 x 的天敌，n 个点要开 3n
// 单次操作近似 O(alpha(n))，只能表示"同类/吃"这种循环关系
template<int NMAX=N>
struct KindDSU
{
    int fa[NMAX*3],n;
    void init(int n_)
    {
        n=n_;
        for(int i=1;i<=n*3;i++)fa[i]=i;
    }
    int find(int x)
    {
        return fa[x]==x?x:fa[x]=find(fa[x]);
    }
    void unionn(int x,int y)
    {
        fa[find(x)]=find(y);
    }
    // 声明 x,y 同类，返回 false 表示与已有关系矛盾
    bool same(int x,int y)
    {
        if(find(x)==find(y+n)||find(x)==find(y+2*n))return false;//已经互相吃
        unionn(x,y),unionn(x+n,y+n),unionn(x+2*n,y+2*n);
        return true;
    }
    // 声明 x 吃 y，返回 false 表示矛盾
    bool eat(int x,int y)
    {
        if(find(x)==find(y)||find(x)==find(y+n))return false;//同类或已被 y 吃
        unionn(x+n,y),unionn(x,y+2*n),unionn(x+2*n,y+n);
        return true;
    }
};

// 边权并查集：d[x] = val[x]-val[fa[x]]，find 时先递归到根再累加权值
// merge(x,y,w) 表示 val[y]-val[x]=w；mod>0 时权值对 mod 取模(种类并查集用 mod=3，食物链里 2 表示 x 吃 y)
template<int NMAX=N>
struct WeightDSU
{
    int fa[NMAX],d[NMAX],mod;
    int norm(int x)
    {
        if(mod==0)return x;
        return ((x%mod)+mod)%mod;
    }
    void init(int n,int m=0)
    {
        mod=m;
        for(int i=1;i<=n;i++)fa[i]=i,d[i]=0;
    }
    int find(int x)
    {
        if(fa[x]==x)return x;
        int f=fa[x];
        fa[x]=find(f);
        d[x]=norm(d[x]+d[f]);//val[x]-val[根]
        return fa[x];
    }
    // 加入关系 val[y]-val[x]=w，返回 false 表示与已有关系矛盾
    bool merge(int x,int y,int w)
    {
        int fx=find(x),fy=find(y);
        if(fx==fy)return norm(d[y]-d[x])==norm(w);
        fa[fy]=fx;
        d[fy]=norm(d[x]+w-d[y]);//解出 val[fy]-val[fx]
        return true;
    }
    // 同集合返回 val[y]-val[x]，否则返回 INF
    int query(int x,int y)
    {
        if(find(x)!=find(y))return INF;
        return norm(d[y]-d[x]);
    }
};

int n,vmod;
int gid[305],val[305];//暴力：每个点的集合编号 + 在该集合内的"绝对"权值

int norm(int x)
{
    if(vmod==0)return x;
    return ((x%vmod)+vmod)%vmod;
}

void brute_init(int n_)
{
    n=n_;
    for(int i=1;i<=n;i++)gid[i]=i,val[i]=0;
}

// 暴力合并：把 y 那一整块的权值整体平移 delta
void brute_link(int x,int y,int w)
{
    if(gid[x]==gid[y])return;
    int a=gid[x],b=gid[y];
    int delta=norm(val[x]+w-val[y]);
    for(int i=1;i<=n;i++)
        if(gid[i]==b)gid[i]=a,val[i]=norm(val[i]+delta);
}

// 暴力查询：同集合返回 val[y]-val[x]，否则返回 INF(注意别用 -1 当哨兵，差值本身可能是 -1)
int brute_query(int x,int y)
{
    if(gid[x]!=gid[y])return INF;
    return norm(val[y]-val[x]);
}

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：食物链(mod 3) 与"边权距离"(mod 0) 两种情况都和暴力对拍
int main()
{
    srand(19260817);
    ll bad=0,fake=0,cnt=0;
    // 第一组：食物链，op1 同类(w=0)、op2 x 吃 y(w=2)，两种并查集都要和暴力一致
    for(int t=1;t<=10;t++)
    {
        KindDSU<305> kd;
        WeightDSU<305> wd;
        n=rnd(1,20);
        kd.init(n);
        wd.init(n,3);
        vmod=3;
        brute_init(n);
        for(int q=1;q<=500;q++)
        {
            int op=rnd(1,2),x=rnd(1,n),y=rnd(1,n);
            int w=(op==1?0:2);
            int br=brute_query(x,y);
            int bfake=(br!=INF&&br!=w);//暴力判定这句话是假话
            int k_ok=(op==1?kd.same(x,y):kd.eat(x,y));
            int w_ok=wd.merge(x,y,w);
            cnt++;
            if(k_ok!=!bfake||w_ok!=!bfake)bad++;
            if(bfake)fake++;
            else brute_link(x,y,w);
        }
    }
    printf("食物链(种类并查集/边权mod3) vs 暴力: %s, 操作=%lld, 假话=%lld\n",bad?"FAIL":"OK",cnt,fake);
    // 第二组：边权并查集(不取模)，随机加关系与随机查差值
    ll bad2=0;
    for(int t=1;t<=10;t++)
    {
        WeightDSU<305> wd;
        n=rnd(1,20);
        wd.init(n,0);
        vmod=0;
        brute_init(n);
        for(int q=1;q<=500;q++)
        {
            int op=rnd(1,3),x=rnd(1,n),y=rnd(1,n);
            if(op==1)//随机给一条关系
            {
                int w=rnd(-10,10);
                int br=brute_query(x,y);
                int expect=(br==INF||br==w);
                int ok=wd.merge(x,y,w);
                if(ok!=expect)bad2++;
                if(expect)brute_link(x,y,w);
            }
            else
            {
                int a=wd.query(x,y),b=brute_query(x,y);
                if(a!=b)bad2++;//不同集合时两边都返回 INF
            }
        }
    }
    printf("边权并查集(差值) vs 暴力: %s, 错=%lld\n",bad2?"FAIL":"OK",bad2);
    return 0;
}
