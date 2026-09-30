// 区间DP 的测试与对拍代码
// 模板本体：06-动态规划/区间DP.cpp
#include "../../06-动态规划/区间DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(n^3)，环形石子合并最小代价
// f[l][r] 表示把 [l,r] 合并成一堆的最小代价，枚举断点 k
// 断环成链 a[i+n]=a[i]，答案是 min f[i][i+n-1]
int stone_merge_min(int n,int a[])
{
    for(int i=1;i<=n;i++)a[i+n]=a[i];
    for(int i=1;i<=2*n;i++)sum[i]=sum[i-1]+a[i];
    for(int i=1;i<=2*n;i++)
        for(int j=1;j<=2*n;j++)gmin[i][j]=(i==j)?0:INF;// 边界：一堆不用合并
    for(int len=2;len<=n;len++)// 必须按区间长度从小到大
        for(int l=1;l+len-1<=2*n;l++)
        {
            int r=l+len-1;
            for(int k=l;k<r;k++)
                gmin[l][r]=min(gmin[l][r],gmin[l][k]+gmin[k+1][r]+sum[r]-sum[l-1]);
        }
    int ans=INF;
    for(int i=1;i<=n;i++)ans=min(ans,gmin[i][i+n-1]);
    return ans;
}

// O(n^3)，环形石子合并最大代价
int stone_merge_max(int n,int a[])
{
    for(int i=1;i<=n;i++)a[i+n]=a[i];
    for(int i=1;i<=2*n;i++)sum[i]=sum[i-1]+a[i];
    for(int i=1;i<=2*n;i++)
        for(int j=1;j<=2*n;j++)gmax[i][j]=0;
    for(int len=2;len<=n;len++)
        for(int l=1;l+len-1<=2*n;l++)
        {
            int r=l+len-1;
            for(int k=l;k<r;k++)
                gmax[l][r]=max(gmax[l][r],gmax[l][k]+gmax[k+1][r]+sum[r]-sum[l-1]);
        }
    int ans=0;
    for(int i=1;i<=n;i++)ans=max(ans,gmax[i][i+n-1]);
    return ans;
}

// O(n^3)，能量项链：emax[l][r] 表示 [l,r] 合成一颗珠子的最大能量
// 合并 (l..k) 和 (k..r) 时释放 a[l]*a[k]*a[r]，注意端点共用
int energy_necklace(int n,int a[])
{
    for(int i=1;i<=n+1;i++)a[i+n]=a[i];// 链要开到 2n+1
    for(int i=1;i<=2*n+1;i++)
        for(int j=1;j<=2*n+1;j++)emax[i][j]=0;
    for(int len=2;len<=n;len++)
        for(int l=1;l+len<=2*n+1;l++)
        {
            int r=l+len;
            for(int k=l+1;k<r;k++)
                emax[l][r]=max(emax[l][r],emax[l][k]+emax[k][r]+a[l]*a[k]*a[r]);
        }
    int ans=0;
    for(int i=1;i<=n;i++)ans=max(ans,emax[i][i+n]);
    return ans;
}

// O(n^4)，多边形游戏：枚举删哪条边(旋转)，再区间 dp 求最大值
// 乘法遇到负数会让"最小"翻成"最大"，所以 max 由 max*max / max*min / min*max / min*min 取
int polygon_game(int n,int val[],char op[])
{
    int ans=NEG;
    for(int cut=1;cut<=n;cut++)// 删掉第 cut 条边，链从 val[cut+1] 开始
    {
        int v[N];char o[N];
        for(int i=1;i<=n;i++)v[i]=val[(cut+i-1)%n+1];
        for(int i=1;i<=n-1;i++)o[i]=op[(cut+i-1)%n+1];
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)pmax[i][j]=NEG,pmin[i][j]=INF;
        for(int i=1;i<=n;i++)pmax[i][i]=pmin[i][i]=v[i];
        for(int len=2;len<=n;len++)
            for(int l=1;l+len-1<=n;l++)
            {
                int r=l+len-1;
                for(int k=l;k<r;k++)
                {
                    int cand[4],cnt=0;
                    if(o[k]=='+')
                    {
                        cand[cnt++]=pmax[l][k]+pmax[k+1][r];
                        cand[cnt++]=pmin[l][k]+pmin[k+1][r];
                    }
                    else
                    {
                        cand[cnt++]=pmax[l][k]*pmax[k+1][r];
                        cand[cnt++]=pmax[l][k]*pmin[k+1][r];
                        cand[cnt++]=pmin[l][k]*pmax[k+1][r];
                        cand[cnt++]=pmin[l][k]*pmin[k+1][r];
                    }
                    for(int t=0;t<cnt;t++)
                        pmax[l][r]=max(pmax[l][r],cand[t]),pmin[l][r]=min(pmin[l][r],cand[t]);
                }
            }
        ans=max(ans,pmax[1][n]);
    }
    return ans;
}

// 暴力：环形石子合并，直接模拟每次合并相邻两堆
int brute_stone(int m,int vv[],int mode)
{
    if(m==1)return 0;
    int best=(mode==0)?INF:NEG;
    for(int i=0;i<m;i++)
    {
        int j=(i+1)%m,nv[N],idx=0;
        for(int k=0;k<m;k++)
            if(k!=j)nv[idx++]=(k==i)?vv[i]+vv[j]:vv[k];
        int cur=brute_stone(m-1,nv,mode)+vv[i]+vv[j];
        if(mode==0)best=min(best,cur);
        else best=max(best,cur);
    }
    return best;
}

// 暴力：能量项链，环上随便挑相邻两颗珠子合并，删掉中间那个端点
int brute_energy(int m,int vv[])
{
    if(m==1)return 0;
    int best=0;
    for(int i=0;i<m;i++)
    {
        int j=(i+1)%m,nv[N],idx=0;
        for(int k=0;k<m;k++)
            if(k!=j)nv[idx++]=vv[k];
        best=max(best,brute_energy(m-1,nv)+vv[i]*vv[j]*vv[(i+2)%m]);
    }
    return best;
}

// 暴力：线性链上枚举所有合并顺序
int merge_list(int m,int v[],char o[])
{
    if(m==1)return v[1];
    int best=NEG;
    for(int i=1;i<m;i++)
    {
        int nv[N];char no[N];
        for(int k=1;k<=m;k++)
            if(k!=i+1)nv[(k<i+1)?k:k-1]=(k==i)?((o[i]=='+')?v[i]+v[i+1]:v[i]*v[i+1]):v[k];
        for(int k=1;k<=m-2;k++)
        {
            if(k<i)no[k]=o[k];
            else if(k==i)no[k]=o[i+1];
            else no[k]=o[k+1];
        }
        best=max(best,merge_list(m-1,nv,no));
    }
    return best;
}

// 暴力：多边形游戏，枚举删掉哪条边再枚举所有合并顺序
int brute_polygon(int n,int val[],char op[])
{
    int ans=NEG;
    for(int e=0;e<n;e++)
    {
        int v[N];char o[N];int cnt=0;
        for(int k=1;k<=n;k++)v[++cnt]=val[(e+k-1)%n+1];
        for(int k=1;k<=n-1;k++)o[k]=op[(e+k-1)%n+1];
        ans=max(ans,merge_list(cnt,v,o));
    }
    return ans;
}

int main()
{
    srand(20240605);
    printf("==== 固定样例 ====\n");
    int s1[N]={0,4,5,9,4};
    printf("环形石子 n=4 [4 5 9 4] : min=%d max=%d (期望 43 54)\n",
        stone_merge_min(4,s1),stone_merge_max(4,s1));
    int s2[N]={0,2,3,5,10};
    printf("能量项链 n=4 [2 3 5 10] : %d (期望 710)\n",energy_necklace(4,s2));
    int s3[N]={0,-7,4,2,5};
    char o3[N]={'?','+','*','*','+'};// op[i] 连 val[i] 和 val[i+1]
    printf("多边形游戏 n=4 : %d (期望 33，删掉最后一条边)\n",polygon_game(4,s3,o3));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0,ref,cur;
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,7);
        for(int i=1;i<=n;i++)a[i]=rndint(1,10);
        for(int i=0;i<n;i++)bv[i]=a[i+1];
        ref=brute_stone(n,bv,0),cur=stone_merge_min(n,a);
        if(ref!=cur){bad++;printf("WA! 石子min 轮%d n=%d ref=%d cur=%d\n",tt,n,ref,cur);break;}
        for(int i=1;i<=n;i++)a[i]=rndint(1,10);
        for(int i=0;i<n;i++)bv[i]=a[i+1];
        ref=brute_stone(n,bv,1),cur=stone_merge_max(n,a);
        if(ref!=cur){bad++;printf("WA! 石子max 轮%d n=%d ref=%d cur=%d\n",tt,n,ref,cur);break;}
        for(int i=1;i<=n;i++)a[i]=rndint(1,6);
        for(int i=0;i<n;i++)bv[i]=a[i+1];
        ref=brute_energy(n,bv),cur=energy_necklace(n,a);
        if(ref!=cur){bad++;printf("WA! 能量项链 轮%d n=%d ref=%d cur=%d\n",tt,n,ref,cur);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,5);
        for(int i=1;i<=n;i++)val[i]=rndint(-6,6);
        for(int i=1;i<=n;i++)op[i]=(rndint(0,1)==0)?'+':'*';
        ref=brute_polygon(n,val,op),cur=polygon_game(n,val,op);
        if(ref!=cur){bad++;printf("WA! 多边形 轮%d n=%d ref=%d cur=%d\n",tt,n,ref,cur);break;}
    }
    if(!bad)printf("stress OK (石子min/max 300 轮 + 能量项链 300 轮 + 多边形 200 轮 全部通过)\n");
    return 0;
}
