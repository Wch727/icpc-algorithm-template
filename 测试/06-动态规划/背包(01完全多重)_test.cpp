// 背包(01完全多重) 的测试与对拍代码
// 模板本体：06-动态规划/背包(01完全多重).cpp
#include "../../06-动态规划/背包(01完全多重).cpp"

// 生成 [l,r] 的随机整数

int rndint(int l,int r)
{
    return l+rand()%(r-l+1);
}

// 暴力：第 i 个物品起、剩 rem 容量，按 typ 决定能拿几个
int brute(int i,int rem)
{
    if(i>n)return 0;
    int lim;
    if(typ[i]==0)lim=(w[i]<=rem)?1:0;// 01 也要判放不放得下
    else if(typ[i]==1)lim=rem/w[i];
    else lim=min(c[i],rem/w[i]);
    int best=brute(i+1,rem);
    for(int k=1;k<=lim;k++)best=max(best,brute(i+1,rem-k*w[i])+k*v[i]);
    return best;
}

// 暴力：01 背包恰好装满
void dfs_exact(int i,int rem,int val)
{
    if(i>n)
    {
        if(rem==0)exact_ans=max(exact_ans,val);
        return;
    }
    dfs_exact(i+1,rem,val);
    if(w[i]<=rem)dfs_exact(i+1,rem-w[i],val+v[i]);
}

// 出错的现场数据
void print_data()
{
    printf("n=%d V=%d\n",n,V);
    for(int i=1;i<=n;i++)printf("i=%d w=%d v=%d c=%d typ=%d\n",i,w[i],v[i],c[i],typ[i]);
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        int ww[]={0,2},vv[]={0,3},cc[]={0,INT_MAX};
        assert(knap_multiple_binary(1,10,ww,vv,cc)==15);
    }

    srand(20240601);
    printf("==== 固定样例 ====\n");
    int w1[5]={0,71,69,1},v1[5]={0,100,1,2};
    printf("01   P1048 T=70 : %d (期望 3)\n",knap_01(3,70,w1,v1));
    printf("完全 P1616 T=70 : %d (期望 140)\n",knap_complete(3,70,w1,v1));
    int w2[5]={0,3,4},v2[5]={0,4,5},c2[5]={0,2,3};
    printf("多重 二进制 V=10 : %d (期望 13)\n",knap_multiple_binary(2,10,w2,v2,c2));
    printf("多重 单调队列 V=10: %d (期望 13)\n",knap_multiple_deque(2,10,w2,v2,c2));
    int w3[5]={0,3,4,2},v3[5]={0,4,5,3},c3[5]={0,1,3,2},t3[5]={0,0,1,2};
    printf("混合 V=10 : %d (期望 13)\n",knap_mixed(3,10,w3,v3,c3,t3));
    printf("01 恰好装满 V=10 : %d (1061109567 表示装不满)\n",knap_01_exact(2,10,w2,v2));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0,ref,cur;
    for(tt=1;tt<=800;tt++)
    {
        n=rndint(1,7),V=rndint(1,25);
        for(int i=1;i<=n;i++)w[i]=rndint(1,10),v[i]=rndint(1,15),c[i]=rndint(1,4);
        for(int i=1;i<=n;i++)typ[i]=0;// 01
        ref=brute(1,V),cur=knap_01(n,V,w,v);
        if(ref!=cur){bad++;printf("WA! 01 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        for(int i=1;i<=n;i++)typ[i]=1;// 完全
        ref=brute(1,V),cur=knap_complete(n,V,w,v);
        if(ref!=cur){bad++;printf("WA! 完全 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        for(int i=1;i<=n;i++)typ[i]=2;// 多重
        ref=brute(1,V),cur=knap_multiple_binary(n,V,w,v,c);
        if(ref!=cur){bad++;printf("WA! 多重拆分 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        cur=knap_multiple_deque(n,V,w,v,c);
        if(ref!=cur){bad++;printf("WA! 多重单调队列 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        for(int i=1;i<=n;i++)typ[i]=rndint(0,2);// 混合
        ref=brute(1,V),cur=knap_mixed(n,V,w,v,c,typ);
        if(ref!=cur){bad++;printf("WA! 混合 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        exact_ans=-INF;
        dfs_exact(1,V,0);
        cur=knap_01_exact(n,V,w,v);
        if(exact_ans!=cur){bad++;printf("WA! 恰好装满 轮%d ref=%d cur=%d\n",tt,exact_ans,cur);break;}
    }
    if(!bad)printf("stress OK (800 轮，01/完全/多重/混合/恰好装满 全部通过)\n");
    return 0;
}
