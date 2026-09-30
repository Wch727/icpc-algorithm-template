// 长链剖分 的测试与对拍代码
// 模板本体：04-图论/长链剖分.cpp
#include "../../04-图论/长链剖分.cpp"

int main()
{
    int bad=0;
    LongChain s(3);
    s.add_edge(1,2),s.add_edge(1,3);
    bad+=s.run()[1]!=1;
    mt19937 rnd(722);
    for(int t=1;t<=150;t++)
    {
        int n=rnd()%100+1;
        LongChain tr(n);
        for(int i=2;i<=n;i++)tr.add_edge(i,rnd()%(i-1)+1);
        vector<int> a=tr.run();
        for(int u=1;u<=n;u++)
        {
            vector<pair<int,int> > q(1,{u,0});
            vector<int> cnt(n+1);
            for(int i=0;i<(int)q.size();i++)
            {
                int v=q[i].first,d=q[i].second;
                cnt[d]++;
                for(int w:tr.adj[v])if(tr.fa[w]==v)q.push_back({w,d+1});
            }
            int want=max_element(cnt.begin(),cnt.end())-cnt.begin();
            bad+=a[u]!=want;
        }
    }
    LongChain tr(100000);
    for(int i=1;i<100000;i++)tr.add_edge(i,i+1);
    vector<int> a=tr.run();
    for(int i=1;i<=100000;i++)bad+=a[i]!=0;
    printf("长链剖分：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
