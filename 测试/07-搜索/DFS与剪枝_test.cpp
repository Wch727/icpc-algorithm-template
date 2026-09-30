// DFS与剪枝 的测试与对拍代码
// 模板本体：07-搜索/DFS与剪枝.cpp
#include "../../07-搜索/DFS与剪枝.cpp"

// 全暴力分组，用来对拍

int brute_split(int idx,int s1,int s2)
{
    if(idx>n)return abs(s1-s2);
    return min(brute_split(idx+1,s1+a[idx],s2),brute_split(idx+1,s1,s2+a[idx]));
}

// ---------- 四、自测 ----------

void test_enum()
{
    n=3,k=2;
    printf("[perm] n=3 全排列:\n");
    dfs_perm(1);
    comb_cnt=0;
    dfs_comb(0,1);
    printf("[comb] C(3,2)=%d (期望 3)\n",comb_cnt);
    sub_cnt=0;
    dfs_subset(1);
    printf("[subset] 3 个元素的子集数=%d (期望 8)\n",sub_cnt);
}

void test_prime()
{
    n=4,k=3;
    a[0]=3,a[1]=7,a[2]=12,a[3]=19;
    prime_cnt=0;
    dfs_prime(0,0,0);
    printf("[prime] 答案为 %d (期望 1)\n",prime_cnt);
}

void test_queen()
{
    for(int i=1;i<=4;i++)
    {
        nn=i,cnt_queen=0;
        memset(col,0,sizeof(col));
        memset(dg,0,sizeof(dg));
        memset(udg,0,sizeof(udg));
        dfs_queen(1);
        printf("[n-queen] n=%d 方案数=%d\n",i,cnt_queen);
    }
}

void test_split()
{
    int ok=1;
    mt19937 rnd(12345);
    // 注意：不要直接写 rnd()%k！mt19937 返回 unsigned int，
    // 转成 int 后再取模遇到 >2^31 的值是溢出（UB），必须先缩到有效位
    for(int t=1;t<=200;t++)
    {
        n=rnd()%10+1;
        for(int i=1;i<=n;i++)a[i]=rnd()%50+1;
        sum_rest[n+1]=0;
        for(int i=n;i>=1;i--)sum_rest[i]=sum_rest[i+1]+a[i];
        best=INT_MAX;
        dfs_split(1,0,0);
        int bf=brute_split(1,0,0);
        if(best!=bf)
        {
            ok=0;
            printf("  第 %d 组对拍失败: 剪枝=%d 暴力=%d\n",t,best,bf);
        }
    }
    printf("[split] 200 组随机数据与暴力对拍 %s\n",ok?"全部通过":"失败");
    n=5;
    for(int i=1;i<=5;i++)a[i]=i;
    sum_rest[n+1]=0;
    for(int i=n;i>=1;i--)sum_rest[i]=sum_rest[i+1]+a[i];
    best=INT_MAX;
    dfs_split(1,0,0);
    printf("[split] 样例 a={1,2,3,4,5} 最小差=%d (期望 1)\n",best);
}

int main()
{
    test_enum();
    test_prime();
    test_queen();
    test_split();
    return 0;
}
