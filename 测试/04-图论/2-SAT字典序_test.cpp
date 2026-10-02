#include "../../04-图论/2-SAT.cpp"
int main()
{
    mt19937 rng(20261002);
    for(int z=0;z<3000;z++)
    {
        n=1+rng()%8;clear_all();vector<array<int,4>>clauses;
        int m=rng()%28;
        for(int j=0;j<m;j++)
        {
            int x=1+rng()%n,y=1+rng()%n,a=rng()%2,b=rng()%2;
            clauses.push_back({x,a,y,b}),add_clause(x,a,y,b);
        }
        vector<int>best;
        for(int mask=0;mask<(1<<n);mask++)
        {
            vector<int>x(n+1);for(int i=1;i<=n;i++)x[i]=mask>>(n-i)&1;
            bool ok=true;for(auto [u,a,v,b]:clauses)ok&=(x[u]==a||x[v]==b);
            if(ok){best=x;break;}
        }
        assert(solve()==!best.empty()); // 再次判定不能依赖上次残留 SCC。
        assert(solve_lex()==!best.empty());
        if(!best.empty())for(int i=1;i<=n;i++)assert(val[i]==best[i]);
    }
    n=1;clear_all();assert(solve());
    add_clause(1,0,1,0),add_clause(1,1,1,1);assert(!solve());
    adj[id(1,0)].clear();assert(solve()); // 撤销冲突后重判
    cout<<"PASS: lexicographic 2-SAT and repeated solving\n";
}
