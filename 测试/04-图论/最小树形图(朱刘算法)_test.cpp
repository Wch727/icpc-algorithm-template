// 最小树形图(朱刘算法) 的测试与对拍代码
// 模板本体：04-图论/最小树形图(朱刘算法).cpp
#include "../../04-图论/最小树形图(朱刘算法).cpp"

int on,om,oroot;// 自测用：原始点数/边数/根，都是 1-based
int ou[N*N],ov[N*N];
ll ow[N*N];
ll bval;// 暴力算出的最小权值
int bok;// 暴力是否找到

ll run()// 把 1-based 数据转成 0..on-1 编号再调用模板
{
    n=on,m=om,root=oroot-1;
    for(int i=1;i<=m;i++)e[i].u=ou[i]-1,e[i].v=ov[i]-1,e[i].w=ow[i];
    return zhuliu();
}

void brute()// 枚举每个非根点选哪条入边，检查是否构成以 oroot 为根的树形图
{
    bok=0,bval=0;
    int k=on-1;// 非根点的个数
    int tot=1;
    for(int i=1;i<=k;i++)tot*=(om+1);// 每位 0..om，0 表示不选
    for(int mask=0;mask<tot;mask++)
    {
        int x=mask,par[10]={0},good=1;
        ll s=0;
        for(int node=1;node<=on&&good;node++)
        {
            if(node==oroot)continue;
            int d=x%(om+1);
            x/=(om+1);
            if(d==0||ov[d]!=node)good=0;// 这个点没选入边，或者选的边入点不对
            else par[node]=ou[d],s+=ow[d];
        }
        if(!good)continue;
        for(int node=1;node<=on&&good;node++)// 从每个点往根爬，爬不到就非法
        {
            int v=node,step=0;
            while(v!=oroot&&step<=on)v=par[v],step++;
            if(v!=oroot)good=0;
        }
        if(good&&(!bok||s<bval))bok=1,bval=s;
    }
}

int main()
{
    // 自测 1：手造图，根 1，边 1->2(1) 1->3(5) 2->3(2) 3->2(1)，取 1->2(1)+2->3(2)=3
    on=3,om=4,oroot=1;
    int a[7]={0,1,1,2,3},b[7]={0,2,3,3,2},c[7]={0,1,5,2,1};
    for(int i=1;i<=om;i++)ou[i]=a[i],ov[i]=b[i],ow[i]=c[i];
    ll v=run();
    int gok=ok;// 先存下来，别把 run() 和 ok 写在同一个 printf 里（求值顺序不确定）
    brute();
    printf("手造图 最小树形图=%lld ok=%d 暴力=%lld ok=%d（期望 3 1 3 1）\n",v,gok,bval,bok);
    // 自测 2：根不可达，边只有 2->3(1) 3->2(1)，点 1 是根但没人能到
    on=3,om=2,oroot=1;
    ou[1]=2,ov[1]=3,ow[1]=1;
    ou[2]=3,ov[2]=2,ow[2]=1;
    run();
    gok=ok;
    brute();
    printf("根不可达 最小树形图 ok=%d 暴力 ok=%d（期望 0 0）\n",gok,bok);
    // 自测 3：随机小图（含负权边），与暴力枚举对拍
    for(int T=1;T<=300;T++)
    {
        on=rand()%3+2;// 2..4 个点
        oroot=rand()%on+1;
        om=0;
        for(int i=1;i<=on;i++)
            for(int j=1;j<=on;j++)
                if(i!=j&&om<7&&rand()%2)
                {
                    om++;
                    ou[om]=i,ov[om]=j,ow[om]=rand()%10-3;
                }
        ll got=run();
        int gok=ok;
        brute();
        if(gok!=bok||(bok&&got!=bval))
        {
            printf("WA T=%d n=%d m=%d root=%d got=(%lld,%d) want=(%lld,%d)\n",T,on,om,oroot,got,gok,bval,bok);
            for(int i=1;i<=om;i++)printf("  %d->%d %lld\n",ou[i],ov[i],ow[i]);
            return 0;
        }
    }
    printf("最小树形图 与暴力枚举 300 组通过\n");
    return 0;
}
/* 核心：先给每个非根点选一条最小入边；若没有环，选出来的就是答案；
   若有环就缩点，环内边权减去对应点的最小入边权，再迭代（最多缩 n 轮）
   无解 = 除根以外有点不存在入边（根到不了它），此时置 ok=0 并返回 0
   注意模板会改 n 和边权，多组数据要先存下原始边再重跑；缩点后编号是 0..cnt-1 */
