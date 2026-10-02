// 带权并查集 的测试与对拍代码
// 模板本体：02-数据结构/带权并查集.cpp
#include "../../02-数据结构/带权并查集.cpp"

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
