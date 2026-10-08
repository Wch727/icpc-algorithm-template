#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(n)，长儿子继承深度桶，求子树中结点最多的相对深度；并列取小
struct LongChain
{
    int n,tot;
    vector<vector<int> > adj;
    vector<int> fa,len,son,off,ans,buf,ord;
    LongChain(int n):n(n),tot(0),adj(n+1),fa(n+1),len(n+1),son(n+1),off(n+1),ans(n+1),buf(n+1){}
    void add_edge(int u,int v)
    {
        adj[u].push_back(v),adj[v].push_back(u);
    }
    vector<int> run()
    {
        ord.assign(1,1),fa[1]=0,tot=0;
        fill(buf.begin(),buf.end(),0);
        for(int i= 0; i < (int)ord.size(); i++)
            for(int v : adj[ord[i]])
                if(v != fa[ord[i]])
                    fa[v]= ord[i], ord.push_back(v);
        for(int i=n-1;i>=0;i--)
        {
            int u=ord[i];
            len[u]=1,son[u]=0;
            for(int v : adj[u])
                if(fa[v] == u && len[v] + 1 > len[u])
                    len[u]= len[v] + 1, son[u]= v;
        }
        off[1]=tot,tot+=len[1];
        for(int u : ord)
            for(int v : adj[u])
                if(fa[v] == u)
                {
                    if(v == son[u])
                        off[v]= off[u] + 1;
                    else
                        off[v]= tot, tot+= len[v];
                }
        for(int i=n-1;i>=0;i--)
        {
            int u=ord[i],p=off[u];
            buf[p]=1,ans[u]=son[u]?ans[son[u]]+1:0;
            for(int v : adj[u])
                if(fa[v] == u && v != son[u])
                    for(int j= 0; j < len[v]; j++)
                    {
                        buf[p + j + 1]+= buf[off[v] + j];
                        int d= j + 1;
                        if(buf[p + d] > buf[p + ans[u]] ||
                           (buf[p + d] == buf[p + ans[u]] && d < ans[u]))
                            ans[u]= d;
                    }
            if(buf[p]==buf[p+ans[u]])ans[u]=0;
        }
        return ans;
    }
};
