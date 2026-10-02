// 动态DP 的测试与对拍代码
// 模板本体：06-动态规划/动态DP.cpp
#include "../../06-动态规划/动态DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

ll intree(int u,int p,int mode)// mode=1 表示 u 选，0 表示不选
{
    ll res=mode?w[u]:0;
    for(int v:adj[u])
    {
        if(v==p)continue;
        if(mode)res+=intree(v,u,0);
        else res+=max(intree(v,u,0),intree(v,u,1));
    }
    return res;
}

ll brute_ans()
{
    // 独立暴力枚举点集，不复用树形转移
    ll ans=0;
    for(int mask=0;mask<(1<<n);mask++)
    {
        ll sum=0;
        bool ok=true;
        for(int u=1;u<=n;u++)if(mask>>(u-1)&1)
        {
            sum+=w[u];
            for(int v:adj[u])if(mask>>(v-1)&1)ok=false;
        }
        if(ok)ans=max(ans,sum);
    }
    return ans;
}

int main()
{
    srand(20240625);
    printf("==== 固定样例 ====\n");
    // 原期望 10 写错：相邻的点 2、3 不能同选，枚举子集得 6
    n=4;
    for(int i=1;i<=n;i++)adj[i].clear();
    int e1[3][2]={{1,2},{2,3},{3,4}};
    for(int i=0;i<3;i++)
    {
        int u=e1[i][0],v=e1[i][1];
        adj[u].push_back(v),adj[v].push_back(u);
    }
    w[1]=1,w[2]=5,w[3]=5,w[4]=1;
    build_all();
    ll f0,f1;get_f(1,f0,f1);
    printf("链 1-2-3-4 权 1 5 5 1 : %lld (期望 6)\n",max(f0,f1));
    update(2,100);
    get_f(1,f0,f1);
    printf("把点 2 权改成 100 后 : %lld (期望 101 = 100+1)\n",max(f0,f1));

    printf("==== 随机对拍（含随机修改）====\n");
    int bad=0;
    for(int tt=1;tt<=60;tt++)
    {
        n=rndint(1,10);
        for(int i=1;i<=n;i++)adj[i].clear();
        for(int i=1;i<=n;i++)w[i]=rndint(-10,20);
        for(int v=2;v<=n;v++)
        {
            int u=rndint(1,v-1);
            adj[u].push_back(v),adj[v].push_back(u);
        }
        build_all();
        for(int step=1;step<=20;step++)
        {
            int u=rndint(1,n);
            ll x=rndint(-10,20);
            update(u,x);
            get_f(1,f0,f1);
            ll cur=max(f0,f1),ref=brute_ans();
            if(cur!=ref)
            {
                bad++;
                printf("FAILED! 轮%d 第%d次修改 n=%d u=%d x=%lld ref=%lld cur=%lld\n",tt,step,n,u,x,ref,cur);
                break;
            }
        }
        if(bad)break;
    }
    if(!bad)printf("stress OK (60 组随机树 x 每组 20 次随机点权修改，共 1200 次全部与暴力一致)\n");
    return 0;
}

/*
动态 dp 用法（带修改的最大权独立集）：
1. 先 dfs1 求重儿子，dfs2 剖重链，链内顺序按 链头->链尾
2. 线段树每条链一棵，叶子下标 0 是链尾；合并按 浅*深 的顺序做矩阵乘
3. 修改实际结点的矩阵，再把链头 dp 的变化传给其父结点，沿重链向上跳
4. 查询答案直接 get_f(1) 后取 max
也可换成全局线段树（所有链首尾相接存一个数组）把复杂度降到 O(log^2 n)
*/
