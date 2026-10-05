// 树的重心与直径 的测试与对拍代码
// 模板本体：04-图论/树的重心与直径.cpp
#include "../../04-图论/树的重心与直径.cpp"

int main()
{
    srand(20240514);

    // 自测 1：手造链 1-2-3-4-5，直径 4、重心 3、最远点
    n=5;
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=1;i<n;i++)add_edge(i,i+1);
    printf("链 n=5：直径 %d（期望 4），重心 %d（期望 3）\n",get_diameter(),get_centroid());
    build_far();
    int c=get_centroid();
    printf("重心 %d 到直径端点之一 %d 的距离 %d（期望 2）\n",c,far_node(c),eccentricity(c));

    // 自测 2：手造星形 1 为心，直径 2、重心 1
    n=6;
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=2;i<=n;i++)add_edge(1,i);
    printf("星形 n=6：直径 %d（期望 2），重心 %d（期望 1）\n",get_diameter(),get_centroid());

    // 自测 3：三条叉的树 1-2,2-3,3-4,2-5,5-6，直径 4，重心 2 或 3（取小编号 2）
    n=6;
    for(int i=1;i<=n;i++)adj[i].clear();
    add_edge(1,2),add_edge(2,3),add_edge(3,4),add_edge(2,5),add_edge(5,6);
    printf("叉树 n=6：直径 %d（期望 4），重心 %d（期望 2）\n",get_diameter(),get_centroid());

    // 自测 4：随机树对拍，直径（两遍 bfs / DP / 递归）与重心全部和暴力比
    bool ok=true;
    int round=0;
    for(int T=1;T<=300;T++)
    {
        n=rand()%12+2;
        for(int i=1;i<=n;i++)adj[i].clear();
        for(int i=2;i<=n;i++)add_edge(rand()%(i-1)+1,i);
        // 暴力：矩阵 bfs 求所有点对距离
        int bd[15][15];
        for(int s=1;s<=n;s++)
        {
            for(int i=1;i<=n;i++)bd[s][i]=-1;
            int q2[15],h2=0,t2=0;
            q2[t2++]=s,bd[s][s]=0;
            while(h2<t2)
            {
                int u=q2[h2++];
                for(int v:adj[u])
                    if(bd[s][v]==-1)bd[s][v]=bd[s][u]+1,q2[t2++]=v;
            }
        }
        int bdia=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)bdia=max(bdia,bd[i][j]);
        int d1=get_diameter();
        // get_diameter() 最后跑的是 bfs(da)，此刻 dis[da]=0、dis[db]=dia，赶紧校验
        if(dis[da]!=0||dis[db]!=d1)
        {
            printf("FAILED 直径端点 t=%d dis[da]=%d dis[db]=%d dia=%d\n",T,dis[da],dis[db],d1);
            ok=false;
            break;
        }
        // 最远点：先 build_far() 备好「到 da / 到 db」两组距离
        build_far();
        // 偏心距 = max(d(x,da), d(x,db))，并通过 far_node(x) 校验距离
        // 注意：树上可能有并列最远点，最远点不一定是直径端点，所以只校验「距离」对不对
        for(int x=1;x<=n&&ok;x++)
        {
            int best=0;
            for(int y=1;y<=n;y++)best=max(best,bd[x][y]);
            if(eccentricity(x)!=best||bd[x][far_node(x)]!=best)
            {
                printf("FAILED 最远点 t=%d x=%d got=%d want=%d\n",T,x,eccentricity(x),best);
                ok=false;
                break;
            }
            for(int y=1;y<=n;y++)// 直径性质：|d(x,y)-d(y,da)| <= d(x,da)
                if(abs(bd[x][y]-bd[y][da])>bd[x][da])
                {
                    printf("FAILED 直径性质 t=%d x=%d y=%d\n",T,x,y);
                    ok=false;
                    break;
                }
        }
        if(!ok)break;
        int d2=get_diameter_dp();
        int d3=0;
        dfs_dia(1,0,d3);
        if(d1!=bdia||d2!=bdia||d3!=bdia)
        {
            printf("FAILED 直径 t=%d n=%d bfs=%d dp=%d 递归=%d 暴力=%d\n",T,n,d1,d2,d3,bdia);
            ok=false;
            break;
        }
        // 暴力重心：删掉该点后最大连通块最小
        int bm2=n+1,bc2=-1;
        for(int u=1;u<=n;u++)
        {
            int mark[15]={0};
            mark[u]=1;
            int m=0;
            for(int s=1;s<=n;s++)
            {
                if(mark[s])continue;
                int cnt=0,q2[15],h2=0,t2=0;
                mark[s]=1,q2[t2++]=s;
                while(h2<t2)
                {
                    int x=q2[h2++];
                    cnt++;
                    for(int v:adj[x])
                        if(!mark[v])mark[v]=1,q2[t2++]=v;
                }
                m=max(m,cnt);
            }
            if(m<bm2)bm2=m,bc2=u;
        }
        int c1=get_centroid();
        if(c1!=bc2)
        {
            printf("FAILED 重心 t=%d n=%d got=%d want=%d\n",T,n,c1,bc2);
            ok=false;
            break;
        }
        if(!ok)break;
        if(++round%6==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("随机树直径/重心/最远点对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 5：n=1e5 的链（深度 1e5），迭代版不能爆栈
    n=100000;
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=1;i<n;i++)add_edge(i,i+1);
    get_diameter(),build_far();
    // 注意：最远点查询依赖 dis/dist_db，get_centroid() 会改写 dis，所以先把答案存下来
    int far_da=far_node(da),ecc_da=eccentricity(da),ecc_mid=eccentricity(50000);
    printf("大链 n=100000：直径 %d（期望 99999），DP 直径 %d（期望 99999），重心 %d（期望 50000 或 50001）\n",
           dia,get_diameter_dp(),get_centroid());
    printf("端点 %d 的最远点 %d，偏心距 %d（期望 99999）；中点 50000 的偏心距 %d（期望 50000）\n",
           da,far_da,ecc_da,ecc_mid);
    return 0;
}
/* 用法：
   1) get_diameter() 得到 da/db/dia；2) build_far() 备好最远点查询；3) 查询 O(1)。
   关键点：最远点查询需要两组距离（到 da、到 db），不能只留一组 dis。
   重心若有 2 个，本模板返回编号小的那个；直径长度按边数算（边权 >1 请累加边权）。 */
