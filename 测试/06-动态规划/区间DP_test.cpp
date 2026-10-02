// 区间DP 的测试与对拍代码
// 模板本体：06-动态规划/区间DP.cpp
#include "../../06-动态规划/区间DP.cpp"

int bv[N];// 暴力用的临时环

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

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
