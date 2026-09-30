// 一般图匹配(带花树) 的测试与对拍代码
// 模板本体：04-图论/一般图匹配(带花树).cpp
#include "../../04-图论/一般图匹配(带花树).cpp"

int brute(int mask,const vector<int> &adj,vector<int> &memo)
{
    if(!mask)return 0;
    if(memo[mask]>=0)return memo[mask];
    int u=__builtin_ctz((unsigned)mask),rest=mask^(1<<u),ans=brute(rest,adj,memo);
    for(int v=0;v<(int)adj.size();v++)if((rest>>v&1)&&(adj[u]>>v&1))ans=max(ans,1+brute(rest^(1<<v),adj,memo));
    return memo[mask]=ans;
}

int main()
{
    int bad=0;
    Blossom s(3);
    s.add_edge(1,2),s.add_edge(2,3),s.add_edge(3,1);
    bad+=s.run()!=1;
    mt19937 rnd(271);
    for(int t=1;t<=400;t++)
    {
        int n=rnd()%11+1;
        Blossom tr(n);
        vector<int> adj(n),memo(1<<n,-1);
        for(int u=1;u<=n;u++)for(int v=u+1;v<=n;v++)if(rnd()%2)tr.add_edge(u,v),adj[u-1]|=1<<(v-1),adj[v-1]|=1<<(u-1);
        int got=tr.run();
        bad+=got!=brute((1<<n)-1,adj,memo);
        for(int u=1;u<=n;u++)if(tr.match[u])bad+=tr.match[tr.match[u]]!=u||!(adj[u-1]>>(tr.match[u]-1)&1);
    }
    printf("一般图匹配：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
