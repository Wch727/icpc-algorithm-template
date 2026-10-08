// Ukkonen 后缀树，完整字符串构建 O(27n)，小写字母，内部追加唯一终止符 '{'。
// 边为 text[l[v]..r[v])，root=0，节点 1 为辅助根；parent/link 是父亲/后缀链接。
// 比 SAM/SA 更适合需要压缩后缀 trie 边的题。动态 vector，最多约 2n+3 节点。
// 构建核心参考 KACTL SuffixTree.h / e-maxx Ukkonen；原始接口已改成动态存储。
#include<bits/stdc++.h>
using namespace std;
struct SuffixTree
{
    string text;int original,v=0,q=0,nodes=2;
    vector<array<int,27>> next;vector<int> l,r,parent,link;
    int code(char c)const{return c=='{'?26:c-'a';}
    void append(int i,int c)
    {
        again:
            if(r[v] <= q)
            {
                if(next[v][c] < 0)
                {
                    next[v][c]= nodes;
                    l[nodes]= i;
                    parent[nodes++]= v;
                    v= link[v];
                    q= r[v];
                    goto again;
                }
                v= next[v][c];
                q= l[v];
            }
        if(q==-1||c==code(text[q]))q++;
        else
        {
            l[nodes+1]=i;parent[nodes+1]=nodes;l[nodes]=l[v];r[nodes]=q;parent[nodes]=parent[v];next[nodes][c]=nodes+1;next[nodes][code(text[q])]=v;
            l[v]=q;parent[v]=nodes;next[parent[nodes]][code(text[l[nodes]])]=nodes;
            v= link[parent[nodes]];
            q= l[nodes];
            while(q < r[nodes])
            {
                v= next[v][code(text[q])];
                q+= r[v] - l[v];
            }
            link[nodes]=q==r[nodes]?v:nodes+2;q=r[v]-(q-r[nodes]);nodes+=2;goto again;
        }
    }
    SuffixTree(string s):text(s+"{"),original(s.size()),next(2*text.size()+3),l(next.size()),r(next.size(),text.size()),parent(next.size()),link(next.size())
    {
        for(char c : s)
            assert(c >= 'a' && c <= 'z');
        for(auto &a : next)
            a.fill(-1);
        next[1].fill(0);
        link[0]=1;l[0]=l[1]=-1;r[0]=r[1]=0;for(int i=0;i<(int)text.size();i++)append(i,code(text[i]));
    }
    bool contains(const string &s)const
    {
        int u= 0, i= 0;
        while(i < (int)s.size())
        {
            if(s[i] < 'a' || s[i] > 'z')
                return false;
            u= next[u][code(s[i])];
            if(u < 0)
                return false;
            for(int j= l[u]; j < r[u] && i < (int)s.size(); j++, i++)
                if(text[j] != s[i])
                    return false;
        }
        return true;
    }
};
