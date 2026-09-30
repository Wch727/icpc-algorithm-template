#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=25;// 位数
int dig[N],len;// 拆位，dig[1] 是个位
int tar,K;// 目标数字 与 数位和上限
ll ca[N],sa[N];// 示例1 的记搜：数的个数 与 数字出现次数
ll g2[N][12];// 示例2 不含 4
ll g3[N][12];// 示例3 相邻数字差 >= 2
ll g4[N][205];// 示例4 数位和 <= K

// 拆位，dig[1] 是个位
void get_dig(ll x)
{
    len=0;
    if(x==0){dig[++len]=0;return;}
    while(x>0)dig[++len]=x%10,x/=10;
}

// 示例1：统计数字 tar 在 0..x 里出现的总次数
// 返回 (合法数的个数, 这些数里 tar 的出现次数)
// 关键边界：记忆化只在 !limit && !lead 时记录和取用
pair<ll,ll> dfs_cnt(int pos,bool limit,bool lead)
{
    if(pos==0)return make_pair(1,0);// 数完了，算 1 个补全
    if(!limit&&!lead&&ca[pos]!=-1)return make_pair(ca[pos],sa[pos]);
    int up=limit?dig[pos]:9;
    ll cnt=0,sum=0;
    for(int i=0;i<=up;i++)
    {
        pair<ll,ll> t=dfs_cnt(pos-1,limit&&i==up,lead&&i==0);
        cnt+=t.first;
        if(i==tar&&!(lead&&i==0))sum+=t.first;// 这一位贡献 t.first 次
        sum+=t.second;
    }
    if(!limit&&!lead)ca[pos]=cnt,sa[pos]=sum;// 贴上界/前导零的状态不能记
    return make_pair(cnt,sum);
}

// 示例2：统计 0..x 里十进制表示不含 4 的数的个数
// 不含 4 跟前导零无关，所以不用 lead 也可以直接记
ll dfs_no4(int pos,bool limit)
{
    if(pos==0)return 1;
    if(!limit&&g2[pos][0]!=-1)return g2[pos][0];
    int up=limit?dig[pos]:9;
    ll res=0;
    for(int i=0;i<=up;i++)
    {
        if(i==4)continue;
        res+=dfs_no4(pos-1,limit&&i==up);
    }
    if(!limit)g2[pos][0]=res;
    return res;
}

// 示例3：统计 0..x 里相邻两位数字差 >= 2 的数的个数(windy 数)
// pre=10 表示前面还没有有效数字(还在前导零阶段)
ll dfs_windy(int pos,int pre,bool limit,bool lead)
{
    if(pos==0)return 1;
    if(!limit&&!lead&&g3[pos][pre]!=-1)return g3[pos][pre];
    int up=limit?dig[pos]:9;
    ll res=0;
    for(int i=0;i<=up;i++)
    {
        if(!(lead&&i==0)&&pre!=10&&abs(i-pre)<2)continue;// 相邻差不够就剪掉
        res+=dfs_windy(pos-1,(lead&&i==0)?10:i,limit&&i==up,lead&&i==0);
    }
    if(!limit&&!lead)g3[pos][pre]=res;
    return res;
}

// 示例4：统计 0..x 里数位和 <= K 的数的个数
// 前导零对 digit sum 没有影响，所以状态只需要 (pos,sum)
ll dfs_sumk(int pos,int sum,bool limit)
{
    if(sum>K)return 0;
    if(pos==0)return 1;
    if(!limit&&g4[pos][sum]!=-1)return g4[pos][sum];
    int up=limit?dig[pos]:9;
    ll res=0;
    for(int i=0;i<=up;i++)res+=dfs_sumk(pos-1,sum+i,limit&&i==up);
    if(!limit)g4[pos][sum]=res;
    return res;
}

// 求 [0,x]，x<0 时返回 0
ll solve_cnt(ll x,int dd)
{
    if(x<0)return 0;
    tar=dd,get_dig(x);
    memset(ca,-1,sizeof(ca)),memset(sa,-1,sizeof(sa));
    return dfs_cnt(len,true,true).second;
}

ll solve_no4(ll x)
{
    if(x<0)return 0;
    get_dig(x);
    memset(g2,-1,sizeof(g2));
    return dfs_no4(len,true);
}

ll solve_windy(ll x)
{
    if(x<0)return 0;
    get_dig(x);
    memset(g3,-1,sizeof(g3));
    return dfs_windy(len,10,true,true);
}

ll solve_sumk(ll x,int k)
{
    if(x<0)return 0;
    K=k,get_dig(x);
    memset(g4,-1,sizeof(g4));
    return dfs_sumk(len,0,true);
}
