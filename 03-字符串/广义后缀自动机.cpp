// 多串子串集合的 SAM，小写字母；每条串从根重新插入，不引入跨串子串。
// 已有转移也可能需要 clone，不能把普通 extend 原样 last=根 就用。
// 状态数 O(总长)，26 固定字母表；支持子串判定及不同子串数，不统计各串出现次数。
#include<bits/stdc++.h>
using namespace std;
struct GeneralSAM
{
    struct Node{array<int,26> next{};int len=0,link=-1;};vector<Node> t={{}};
    int clone(int q, int len)
    {
        Node v= t[q];
        v.len= len;
        t.push_back(v);
        return t.size() - 1;
    }
    int extend(int last,int c)
    {
        int p=last,q=t[p].next[c];
        if(q)
        {
            if(t[p].len+1==t[q].len)return q;
            int z=clone(q,t[p].len+1);while(p>=0&&t[p].next[c]==q)t[p].next[c]=z,p=t[p].link;t[q].link=z;return z;
        }
        int cur=t.size();t.push_back({});t[cur].len=t[last].len+1;
        while(p>=0&&!t[p].next[c])t[p].next[c]=cur,p=t[p].link;
        if(p<0)t[cur].link=0;
        else
        {
            q=t[p].next[c];if(t[p].len+1==t[q].len)t[cur].link=q;
            else
            {
                int z= clone(q, t[p].len + 1);
                while(p >= 0 && t[p].next[c] == q)
                    t[p].next[c]= z, p= t[p].link;
                t[q].link= t[cur].link= z;
            }
        }
        return cur;
    }
    void insert(const string &s)
    {
        int last= 0;
        for(char c : s)
        {
            assert(c >= 'a' && c <= 'z');
            last= extend(last, c - 'a');
        }
    }
    bool contains(const string &s) const
    {
        int p= 0;
        for(char c : s)
        {
            if(c < 'a' || c > 'z')
                return false;
            p= t[p].next[c - 'a'];
            if(!p)
                return false;
        }
        return true;
    }
    long long distinct() const
    {
        long long ans= 0;
        for(int i= 1; i < (int)t.size(); i++)
            ans+= t[i].len - t[t[i].link].len;
        return ans;
    }
};
