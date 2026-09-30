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

// 暴力：数字 d 在 1..x 里出现几次（0 不算任何数字）
int brute_cnt(ll x,int d)
{
    int s=0;
    for(ll i=1;i<=x;i++)
    {
        ll t=i;
        while(t>0)
        {
            if(t%10==d)s++;
            t/=10;
        }
    }
    return s;
}

// 暴力：区间内不含 4 的数的个数
int brute_no4(ll l,ll r)
{
    int s=0;
    for(ll i=l;i<=r;i++)
    {
        ll t=i;
        int ok=1;
        while(t>0)
        {
            if(t%10==4)ok=0;
            t/=10;
        }
        s+=ok;
    }
    return s;
}

// 暴力：区间内相邻数字差 >= 2 的数的个数
int brute_windy(ll l,ll r)
{
    int s=0;
    for(ll i=l;i<=r;i++)
    {
        ll t=i;
        int ok=1,last=-1;
        while(t>0)
        {
            int c=t%10;
            if(last>=0&&abs(c-last)<2)ok=0;
            last=c,t/=10;
        }
        s+=ok;
    }
    return s;
}

// 暴力：区间内数位和 <= K 的数的个数
int brute_sumk(ll l,ll r,int k)
{
    int s=0;
    for(ll i=l;i<=r;i++)
    {
        ll t=i;
        int sum=0;
        while(t>0)sum+=t%10,t/=10;
        if(sum<=k)s++;
    }
    return s;
}

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

int main()
{
    srand(20240609);
    printf("==== 固定样例 ====\n");
    printf("1..11 中 1 出现次数 : %lld (期望 4)\n",solve_cnt(11,1));
    printf("1..100 中不含 4 的个数 : %lld  暴力 %d\n",solve_no4(100)-solve_no4(0),brute_no4(1,100));
    printf("1..100 中 windy 数个数 : %lld  暴力 %d\n",solve_windy(100)-solve_windy(0),brute_windy(1,100));
    printf("1..100 中数位和<=5个数 : %lld  暴力 %d\n",solve_sumk(100,5)-solve_sumk(0,5),brute_sumk(1,100,5));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=300;tt++)
    {
        ll x=rndint(0,3000);
        int dd=rndint(0,9);
        if(solve_cnt(x,dd)!=brute_cnt(x,dd))
        {
            bad++,printf("WA! 数字次数 轮%d x=%lld d=%d dp=%lld brute=%d\n",tt,x,dd,solve_cnt(x,dd),brute_cnt(x,dd));
            break;
        }
        ll l=rndint(1,3000),r=rndint((int)l,3000);
        ll dp1=solve_no4(r)-solve_no4(l-1);
        if(dp1!=brute_no4(l,r)){bad++,printf("WA! 不含4 轮%d l=%lld r=%lld dp=%lld brute=%d\n",tt,l,r,dp1,brute_no4(l,r));break;}
        ll dp2=solve_windy(r)-solve_windy(l-1);
        if(dp2!=brute_windy(l,r)){bad++,printf("WA! windy 轮%d l=%lld r=%lld dp=%lld brute=%d\n",tt,l,r,dp2,brute_windy(l,r));break;}
        int k=rndint(1,25);
        ll dp3=solve_sumk(r,k)-solve_sumk(l-1,k);
        if(dp3!=brute_sumk(l,r,k)){bad++,printf("WA! 数位和 轮%d l=%lld r=%lld k=%d dp=%lld brute=%d\n",tt,l,r,k,dp3,brute_sumk(l,r,k));break;}
    }
    if(!bad)printf("stress OK (300 轮，数字出现次数/不含4/windy/数位和 全部通过)\n");
    printf("大范围抽查：1..1e18 中 windy 数个数 = %lld\n",solve_windy(1000000000000000000LL)-solve_windy(0));
    return 0;
}
