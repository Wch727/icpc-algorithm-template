#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;

// ================= 一、随机数 =================
// mt19937 比 rand() 快且质量好；范围用 [l,r] 闭区间

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
// 固定种子方便复现：mt19937 rnd(20240607);

int rand_int(int l,int r)//[l,r]，l<=r
{
    return l+(int)(rnd()%(unsigned)(r-l+1));
}

double rand_real(double l,double r)//[l,r)
{
    return l+(r-l)*rnd()/4294967296.0;
}

void rand_array(int n,int l,int r,int *a)
{
    for(int i=1;i<=n;i++)a[i]=rand_int(l,r);
}

void rand_perm(int n,int *p)//1..n 的随机排列
{
    for(int i=1;i<=n;i++)p[i]=i;
    for(int i=n;i>=2;i--)swap(p[i],p[rand_int(1,i)]);
}

// 随机无根树（每个点随机接向它前面的点），边权 1..w
void rand_tree(int n,int w,vector<pair<int,int> > &edges)
{
    edges.clear();
    for(int i=2;i<=n;i++)edges.push_back(make_pair(rand_int(1,i-1),i));
}

// ================= 二、对拍框架 =================
// 思路：同一个进程里写「正解」和「暴力」，用同一份随机数据各跑一次比输出。
// 真·双进程对拍（考场常用）：
//   :loop
//     gen.exe > in.txt
//     a.exe < in.txt > a.out
//     b.exe < in.txt > b.out
//     fc a.out b.out || pause
// 或者用 bat 的 fc 比较，见文件末尾注释。

int g_pass,g_fail;
// 返回值：1 通过，0 失败
int check_case(int id,bool equal,const string &msg)
{
    if(equal)
    {
        g_pass++;
        return 1;
    }
    g_fail++;
    if(g_fail<=3)printf("  第 %d 组对拍失败: %s\n",id,msg.c_str());
    return 0;
}

// 例题：正解 = 排序后取第 k 大；暴力 = 双重循环数第 k 大（值域小，直接桶）
int fast_kth(int *a,int n,int k)
{
    vector<int> v(a+1,a+n+1);
    sort(v.begin(),v.end(),greater<int>());
    return v[k-1];
}

int brute_kth(int *a,int n,int k)
{
    for(int v=1000;v>=1;v--)
    {
        int c=0;
        for(int i=1;i<=n;i++)
            if(a[i]==v)c++;
        if(c>=k)return v;
        k-=c;
    }
    return -1;
}

// 例题：正解 = 欧几里得 gcd；暴力 = 从大到小试除
int fast_gcd(int a,int b)
{
    while(b){int t=a%b;a=b;b=t;}
    return a;
}

int brute_gcd(int a,int b)
{
    int g=1;
    for(int i=1;i<=min(a,b);i++)
        if(a%i==0&&b%i==0)g=i;
    return g;
}

void stress_kth()
{
    g_pass=g_fail=0;
    int a[N];
    for(int t=1;t<=1000;t++)
    {
        int n=rand_int(1,30),k;
        rand_array(n,1,20,a);
        k=rand_int(1,n);
        char buf[128];
        sprintf(buf,"n=%d k=%d",n,k);
        check_case(t,fast_kth(a,n,k)==brute_kth(a,n,k),buf);
    }
    printf("[对拍] 第 k 大：%d 组 通过 %d 失败 %d\n",g_pass+g_fail,g_pass,g_fail);
}

void stress_gcd()
{
    g_pass=g_fail=0;
    for(int t=1;t<=2000;t++)
    {
        int a=rand_int(1,200),b=rand_int(1,200);
        char buf[128];
        sprintf(buf,"a=%d b=%d",a,b);
        check_case(t,fast_gcd(a,b)==brute_gcd(a,b),buf);
    }
    printf("[对拍] gcd：%d 组 通过 %d 失败 %d\n",g_pass+g_fail,g_pass,g_fail);
}

// ================= 三、生成器写文件的写法 =================
// 赛场/本地需要生成数据文件时用这种 main，写完重定向到文件即可
void gen_data_file(const char *name,int cnt)
{
    FILE *fp=fopen(name,"w");
    if(!fp)return;
    fprintf(fp,"%d\n",cnt);
    for(int t=1;t<=cnt;t++)
    {
        int n=rand_int(1,100);
        fprintf(fp,"%d\n",n);
        for(int i=1;i<=n;i++)fprintf(fp,"%d ",rand_int(1,1000));
        fprintf(fp,"\n");
    }
    fclose(fp);
    printf("[生成器] 已写出 %s，共 %d 组数据\n",name,cnt);
}

// ================= 四、自测 =================

void test_rng()
{
    // 随机性检查：1..6 各面出现次数应大致均匀，并且同一序列可复现
    mt19937 r2(12345);
    int cnt[7]={0};
    for(int i=1;i<=60000;i++)cnt[uniform_int_distribution<int>(1,6)(r2)]++;
    int mn=1e9,mx=0;
    for(int i=1;i<=6;i++){mn=min(mn,cnt[i]);mx=max(mx,cnt[i]);}
    printf("[随机] 6 万次掷骰子: 最少 %d 最多 %d 极差 %d (期望接近 10000，极差 <1000)\n",mn,mx,mx-mn);

    mt19937 r3(12345),r4(12345);
    int same=1;
    for(int i=1;i<=100;i++)if(r3()!=r4())same=0;
    printf("[随机] 同种子序列一致 = %d (期望 1)\n",same);

    int p[N];
    rand_perm(10,p);
    sort(p+1,p+11);
    int ok=1;
    for(int i=1;i<=10;i++)if(p[i]!=i)ok=0;
    printf("[随机] 随机排列是 1..n 的排列 = %d (期望 1)\n",ok);
}

int main()
{
    test_rng();
    stress_kth();
    stress_gcd();
    // 生成器默认不跑，避免产生多余文件；需要时打开下面这行
    // gen_data_file("in.txt",10);
    printf("[对拍] 全部通过 = %d\n",g_fail==0?1:0);
    return 0;
}

/*
双进程对拍 bat（把三个 exe 放同目录）：
    :loop
        gen.exe > in.txt
        std.exe < in.txt > std.out
        my.exe < in.txt > my.out
        fc std.out my.out
        if not errorlevel 1 goto loop
        pause
命令行单行版（PowerShell）：
    while($true){ ./gen.exe > in.txt; ./std.exe < in.txt > a.txt; ./my.exe < in.txt > b.txt;
                  if(Compare-Object (cat a.txt) (cat b.txt)){ break } }
*/
