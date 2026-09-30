#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=26;

// 字母表为连续小写字母 a..a+sig-1；根为 1，0 不使用。
// 禁止串 AC 自动机 + 计数 DP；建图 O(总串长+状态数*sig)。
// init -> insert 所有模式 -> build -> count；build 后不可再插入。
struct AutomatonDP
{
    struct Node
    {
        int go[N],nxt;
        bool bad;
        Node()
        {
            memset(go,0,sizeof(go));
            nxt=0,bad=false;
        }
    };
    vector<Node> tr;
    int sig;
    bool built;

    void init(int s)
    {
        if(s<1||s>N)throw invalid_argument("alphabet");
        sig=s,built=false;
        tr.assign(2,Node());
    }

    // 插入空模式会把根标坏，表示包括空串在内的所有串都被禁止。
    void insert(const string &s)
    {
        if(built)throw logic_error("insert after build");
        for(int i=0;i<(int)s.length();i++)
            if(s[i]<'a'||s[i]>='a'+sig)throw invalid_argument("character");
        int p=1;
        for(int i=0;i<(int)s.length();i++)
        {
            int c=s[i]-'a';
            if(!tr[p].go[c])
            {
                int v=tr.size();
                tr[p].go[c]=v;
                tr.push_back(Node());
            }
            p=tr[p].go[c];
        }
        tr[p].bad=true;//重复禁串不影响合法性
    }

    // BFS 补全转移，并继承 fail 链上的禁串信息；重复调用不重新建图。
    void build()
    {
        if(built)return;
        queue<int> q;
        tr[1].nxt=1;
        for(int c=0;c<sig;c++)
        {
            int v=tr[1].go[c];
            if(v)tr[v].nxt=1,q.push(v);
            else tr[1].go[c]=1;
        }
        while(!q.empty())
        {
            int u=q.front();
            q.pop();
            tr[u].bad=tr[u].bad||tr[tr[u].nxt].bad;
            for(int c=0;c<sig;c++)
            {
                int v=tr[u].go[c];
                if(v)tr[v].nxt=tr[tr[u].nxt].go[c],q.push(v);
                else tr[u].go[c]=tr[tr[u].nxt].go[c];
            }
        }
        built=true;
    }

    // O(n*状态数*sig)，滚动数组空间 O(状态数)，返回模 mod 的合法串数。
    // mod 必须为正；用 __int128 加法，支持正 ll 范围内的模数。
    ll count(int n,ll mod)
    {
        if(!built)throw logic_error("build first");
        if(n<0||mod<=0)throw invalid_argument("length or modulus");
        if(tr[1].bad)return 0;
        int tot=tr.size();
        vector<ll> f(tot),g(tot);
        f[1]=1%mod;
        for(int i=1;i<=n;i++)
        {
            fill(g.begin(),g.end(),0);
            for(int u=1;u<tot;u++)
            {
                if(!f[u]||tr[u].bad)continue;
                for(int c=0;c<sig;c++)
                {
                    int v=tr[u].go[c];
                    if(!tr[v].bad)g[v]=(ll)(((__int128)g[v]+f[u])%mod);
                }
            }
            f.swap(g);
        }
        ll ans=0;
        for(int u=1;u<tot;u++)ans=(ll)(((__int128)ans+f[u])%mod);
        return ans;
    }
};

AutomatonDP ac;
ll cnt=0;
mt19937 rnd(19260817);
