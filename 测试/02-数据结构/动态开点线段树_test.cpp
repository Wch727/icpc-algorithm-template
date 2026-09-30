// 动态开点线段树 的测试与对拍代码
// 模板本体：02-数据结构/动态开点线段树.cpp
#include "../../02-数据结构/动态开点线段树.cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

ll xs[XN],bp[XN];

// 暴力：差分数组 df 的每个键都是值的「变化点」，把它们收进升序数组 bp
// cur 是「从最左一直到当前位置」的前缀和，逐段累加；段 [bp[i],bp[i+1]-1] 上值恒定
// 查询端点 L、R 都取自 xs，而 xs 只包含修改的 L，不一定含 R+1，所以必须用 bp 而不是 xs
ll brute(int k,map<ll,ll> &df,ll L,ll R)
{
    ll res=0,cur=0;
    for(int i=1;i<=k;i++)
    {
        if(df.count(bp[i]))cur+=df[bp[i]];
        if(bp[i]>R)break;
        ll a=max(bp[i],L),b=min(R,i+1<=k?bp[i+1]-1:R);
        if(a<=b)res+=cur*(b-a+1);
    }
    return res;
}

int main()
{
    srand(20240513);

    // 1. 小样例（值域 1..1e9，只用到 1..8 这几个位置）
    DynSeg ds(1,V);
    ds.add(5,8,10);
    ds.add(1,5,1);
    printf("小样例: sum[1,4]=%lld sum[5,5]=%lld sum[6,8]=%lld sum[1,1e9]=%lld\n",
        ds.sum(1,4),ds.sum(5,5),ds.sum(6,8),ds.sum(1,V));

    // 2. 对拍：大值域随机区间加 / 随机区间和 vs 差分暴力
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        int m=rnd(1,300),k=0;
        for(int i=1;i<=m;i++)      // 采样 2m 个坐标，落在 1..1e9
        {
            xs[++k]=rnd(1,1000000000);
            xs[++k]=rnd(1,1000000000);
        }
        sort(xs+1,xs+k+1);
        k=unique(xs+1,xs+k+1)-xs-1;
        DynSeg seg(1,V);
        map<ll,ll> df;
        for(int q=1;q<=400;q++)
        {
            int op=rnd(1,2),i=rnd(1,k),j=rnd(1,k);
            if(i>j)swap(i,j);
            ll L=xs[i],R=xs[j];
            if(op==1)
            {
                ll v=rnd(-100,100);
                seg.add(L,R,v);
                df[L]+=v,df[R+1]-=v;
            }
            else
            {
                // 收集当前所有变化点（每次修改的 L 和 R+1）
                int kb=0;
                for(auto &pr:df)bp[++kb]=pr.first;
                sort(bp+1,bp+kb+1);
                kb=unique(bp+1,bp+kb+1)-bp-1;
                ll got=seg.sum(L,R),want=brute(kb,df,L,R);
                if(got!=want)
                {
                    printf("第 %d 轮错: [%lld,%lld] got=%lld want=%lld\n",T,L,R,got,want);
                    ok=false;
                    break;
                }
            }
        }
        printf("动态开点第 %d 轮 %s (结点数=%d)\n",T,ok?"passed":"FAILED",seg.nodes());
    }

    // 3. 规模测试：2e5 次修改，看结点数是不是 O(m log V)
    DynSeg big(1,V);
    for(int i=1;i<=200000;i++)
    {
        ll L=(ll)(rand()%1000000)*1000+1;
        big.add(L,L+5000,1);
    }
    printf("规模: 200000 次修改 -> 结点 %d (上界 200000*31)\n",big.nodes());
    printf("结果: %s\n",ok?"OK":"FAILED");
    return 0;
}
