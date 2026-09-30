// 莫队算法（普通莫队）：离线处理区间询问，本例求「区间不同数个数」
// 分块 + 排序后左右指针总移动 O(n*sqrt(n))，单次移动 O(1)
// 排序时奇数块右端点反过来排，常数更小（这行别删）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m,block;
int a[N],ans[N],cnt[N],now;
int ql[N],qr[N];                 // 自测用：保留原始询问左右端点（solve 会把 q 排序）

struct Query{
    int l,r,id;
}q[N];

bool cmp(const Query &x,const Query &y)
{
    int bx=x.l/block,by=y.l/block;
    if(bx!=by)return bx<by;
    if(bx&1)return x.r>y.r;         // 奇数块右端点递减
    return x.r<y.r;
}

void add(int pos)
{
    if(cnt[a[pos]]++==0)now++;
}
void del(int pos)
{
    if(--cnt[a[pos]]==0)now--;
}

// 把询问排好序后统一回答，结果写回 ans[id]
void solve()
{
    block=max(1,(int)sqrt(n));
    sort(q+1,q+m+1,cmp);
    int l=1,r=0;
    memset(cnt,0,sizeof(cnt));
    now=0;
    for(int i=1;i<=m;i++)
    {
        while(l>q[i].l)add(--l);
        while(r<q[i].r)add(++r);
        while(l<q[i].l)del(l++);
        while(r>q[i].r)del(r--);
        ans[q[i].id]=now;
    }
}
