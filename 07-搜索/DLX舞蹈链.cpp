#include<bits/stdc++.h>
using namespace std;

// 精确覆盖：每列恰好选一次；稀疏链表，最少候选列优先，最坏指数复杂度
struct Dlx
{
    vector<int> l,r,u,d,col,row,sz,ans;
    int m;
    void init(int n)
    {
        m=n;
        l.resize(n+1),r.resize(n+1),u.resize(n+1),d.resize(n+1);
        col.assign(n+1,0),row.assign(n+1,0),sz.assign(n+1,0),ans.clear();
        for(int i=0;i<=n;i++)l[i]=i-1,r[i]=i+1,u[i]=d[i]=i;
        l[0]=n,r[n]=0;
    }
    void add_row(int id,const vector<int> &a)
    {
        int first=0;
        for(int c:a)
        {
            assert(c>=1&&c<=m);
            int p=l.size();
            l.push_back(p),r.push_back(p),u.push_back(u[c]),d.push_back(c);
            col.push_back(c),row.push_back(id);
            d[u[c]]=p,u[c]=p,sz[c]++;
            if(!first)first=p;
            else l[p]=l[first],r[p]=first,r[l[first]]=p,l[first]=p;
        }
    }
    void remove(int c)
    {
        r[l[c]]=r[c],l[r[c]]=l[c];
        for(int i=d[c];i!=c;i=d[i])
            for(int j=r[i];j!=i;j=r[j])d[u[j]]=d[j],u[d[j]]=u[j],sz[col[j]]--;
    }
    void resume(int c)
    {
        for(int i=u[c];i!=c;i=u[i])
            for(int j=l[i];j!=i;j=l[j])sz[col[j]]++,d[u[j]]=j,u[d[j]]=j;
        r[l[c]]=c,l[r[c]]=c;
    }
    bool dance()
    {
        if(r[0]==0)return true;
        int c=r[0];
        for(int j=r[c];j;j=r[j])if(sz[j]<sz[c])c=j;
        remove(c);
        bool found=false;
        for(int i=d[c];i!=c&&!found;i=d[i])
        {
            ans.push_back(row[i]);
            for(int j=r[i];j!=i;j=r[j])remove(col[j]);
            found=dance();
            for(int j=l[i];j!=i;j=l[j])resume(col[j]);
            if(!found)ans.pop_back();
        }
        resume(c);// 返回后恢复链表，答案另外保存
        return found;
    }
    bool solve()
    {
        ans.clear();// 链表已恢复，允许重复求解
        return dance();
    }
};

bool test(const vector<int> &a,int m)
{
    Dlx tr;
    tr.init(m);
    for(int i=0;i<(int)a.size();i++)
    {
        vector<int> c;
        for(int j=0;j<m;j++)if(a[i]>>j&1)c.push_back(j+1);
        tr.add_row(i,c);
    }
    bool want=false;
    for(int s=0;s<(1<<(int)a.size());s++)
    {
        int used=0;
        bool ok=true;
        for(int i=0;i<(int)a.size();i++)if(s>>i&1)
        {
            if(used&a[i])ok=false;
            used|=a[i];
        }
        if(ok&&used==(1<<m)-1)want=true;
    }
    bool got=tr.solve();
    if(got!=want)return false;
    if(tr.solve()!=want)return false;
    if(got)
    {
        int used=0;
        for(int x:tr.ans)
        {
            if(used&a[x])return false;
            used|=a[x];
        }
        if(used!=(1<<m)-1)return false;
    }
    return true;
}
