// 最大团：着色数是可扩展规模上界，分支界限搜索；最坏指数级。n<=200，0-indexed。
// 输入无自环对称 bitset 邻接表；一般图最大独立集等于补图最大团。
// 极大团枚举：Bron–Kerbosch + 枢轴，n<=63；极大是不再能扩展，未必规模最大。
#include<bits/stdc++.h>
using namespace std;
struct MaximumClique
{
    vector<bitset<200>> g;vector<int> cur,best;
    MaximumClique(vector<bitset<200>> g):g(g){assert(g.size()<=200);}
    void search(vector<int> p)
    {
        vector<int> order,bound;int color=0;
        while(!p.empty())
        {
            ++color;vector<int> rest;bitset<200> used;
            for(int v : p)
                if((g[v] & used).none())
                    used[v]= 1, order.push_back(v), bound.push_back(color);
                else
                    rest.push_back(v);
            p.swap(rest);
        }
        for(int i=(int)order.size()-1;i>=0;i--)
        {
            if(cur.size()+bound[i]<=best.size())return;
            int v=order[i];cur.push_back(v);vector<int> next;
            for(int j= 0; j < i; j++)
                if(g[v][order[j]])
                    next.push_back(order[j]);
            if(next.empty())
            {
                if(cur.size() > best.size())
                    best= cur;
            }
            else
                search(next);
            cur.pop_back();
        }
    }
    vector<int> solve()
    {
        cur.clear();
        best.clear();
        vector<int> p(g.size());
        iota(p.begin(), p.end(), 0);
        search(p);
        return best;
    }
};
// callback 接收一个团的顶点掩码；空图约定枚举空团。输出数量本身可能指数级。
template<class Callback>
void maximal_cliques(const vector<unsigned long long> &g,Callback callback)
{
    using U=unsigned long long;int n=g.size();assert(n<=63);
    auto dfs= [&](auto &&dfs, U r, U p, U x) -> void
    {
        if(!p && !x)
        {
            callback(r);
            return;
        }
        U union_set=p|x;
        int pivot= -1, degree= -1;
        while(union_set)
        {
            int u= __builtin_ctzll(union_set);
            union_set&= union_set - 1;
            int d= __builtin_popcountll(p & g[u]);
            if(d > degree)
                degree= d, pivot= u;
        }
        U cand=p&(pivot<0?~U(0):~g[pivot]);
        while(cand)
        {
            int v= __builtin_ctzll(cand);
            U bit= U(1) << v;
            cand^= bit;
            dfs(dfs, r | bit, p & g[v], x & g[v]);
            p^= bit;
            x|= bit;
        }
    };
    dfs(dfs,0,(1ULL<<n)-1,0);
}
