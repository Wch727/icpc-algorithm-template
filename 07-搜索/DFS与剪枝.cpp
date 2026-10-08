#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=15;
int n,k;
int a[N];

// ---------- 一、枚举类 DFS ----------

// 全排列，O(n!)
int per[N];
bool vis[N];
void dfs_perm(int dep)//dep 从 1 开始，填第 dep 个位置
{
    if(dep>n)
    {
        for(int i=1;i<=n;i++)printf("%d ",per[i]);
        printf("\n");
        return;
    }
    for(int i=1;i<=n;i++)
        if(!vis[i])
        {
            vis[i]=true,per[dep]=i;
            dfs_perm(dep+1);
            vis[i]=false;
        }
}

int comb_cnt;
// 组合，从 [idx,n] 里选，已选 dep 个；O(C(n,k))
void dfs_comb(int dep,int idx)//必须递增选，天然去重
{
    if(dep==k)
    {
        comb_cnt++;
        return;
    }
    for(int i=idx;i<=n;i++)
        dfs_comb(dep+1,i+1);
}

int sub_cnt;
// 子集枚举（DFS 版），O(2^n)
void dfs_subset(int idx)
{
    if(idx>n)
    {
        sub_cnt++;
        return;
    }
    dfs_subset(idx+1);//不选 a[idx]，优先不选可以保证子集不重不漏
    dfs_subset(idx+1);//选 a[idx]
}

// 位掩码枚举子集，O(2^n)，注意 1<<n 在 n>=31 时溢出
void mask_subset()
{
    for(int mask=0;mask<(1<<n);mask++)
        for(int i=0;i<n;i++)
            if(mask >> i & 1)
            {
            } //i 在子集里
}

// ---------- 二、可行性剪枝 ----------

// 组合数之和为素数个数；传入 sum 而不是在叶子处再求和，边界剪枝
int prime_cnt;
bool is_prime(int x)//试除法，O(sqrt(x))
{
    if(x<2)return false;
    for(int i=2;i<=x/i;i++)
        if(x%i==0)return false;
    return true;
}

void dfs_prime(int step,int sum,int idx)//step 已选个数，sum 已选之和
{
    if(step==k)
    {
        if(is_prime(sum))prime_cnt++;
        return;
    }
    if(n-idx<k-step)return;//可行性剪枝：剩下的数不够凑满 k 个
    for(int i=idx;i<n;i++)
        dfs_prime(step+1,sum+a[i],i+1);
}

// N 皇后计数，逐行放；列 / 主对角 r-c+n / 副对角 r+c 三个冲突数组
int nn,cnt_queen;
int col[N],dg[N<<1],udg[N<<1];
void dfs_queen(int r)
{
    if(r>nn)
    {
        cnt_queen++;
        return;
    }
    for(int c=1;c<=nn;c++)
    {
        if(col[c]||dg[r-c+nn]||udg[r+c])continue;
        col[c]=dg[r-c+nn]=udg[r+c]=1;
        dfs_queen(r+1);
        col[c]=dg[r-c+nn]=udg[r+c]=0;
    }
}

// ---------- 三、最优性剪枝（Branch and Bound）----------

int best;
int sum_rest[N+1];//sum_rest[i] = a[i]+...+a[n]（非负元素之和），必须是「还没放下去的元素」之和

// 把 n 个数分成两组，最小化两组和的差；O(2^n)，剪枝后远小于
// 最优性剪枝：还没放的数最多把差值拉小 sum_rest[idx]，
// 所以 |s1-s2|-sum_rest[idx] > best 时这一支不可能更优，直接砍
// 注意 idx 这个位置的 a[idx] 还没选，sum_rest[idx] 必须包含它
void dfs_split(int idx,int s1,int s2)
{
    if(idx>n)
    {
        best=min(best,abs(s1-s2));
        return;
    }
    if(abs(s1-s2)-sum_rest[idx]>best)return;//乐观下界剪枝
    dfs_split(idx+1,s1+a[idx],s2);
    dfs_split(idx+1,s1,s2+a[idx]);
}
