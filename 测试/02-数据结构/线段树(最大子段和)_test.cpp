// 线段树(最大子段和) 的测试与对拍代码
// 模板本体：02-数据结构/线段树(最大子段和).cpp
#include "../../02-数据结构/线段树(最大子段和).cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

ll brute_max(int l,int r)//暴力 Kadane，区间非空
{
    ll best=br[l],cur=br[l];
    for(int i=l+1;i<=r;i++)
    {
        cur=max(cur+br[i],br[i]);
        best=max(best,cur);
    }
    return best;
}

// 自测：随机单点改 + 随机区间查最大子段和，与暴力 Kadane 对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=20;t++)
    {
        n=rnd(1,50);
        for(int i=1;i<=n;i++)a[i]=rnd(-20,20),br[i]=a[i];//含负数
        seg.src=a;
        seg.build(1,n,1);
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,2),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                ll v=rnd(-20,20);
                seg.update(l,v,1,n,1);
                br[l]=v;
            }
            else
            {
                ll x=seg.query(l,r,1,n,1).mx,z=brute_max(l,r);
                cnt++;
                if(x!=z)bad++;
            }
        }
    }
    printf("线段树(最大子段和) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[-2,3,-1,4,-5]，最大子段和 6(取 3,-1,4)
    n=5;
    ll s[6]={0,-2,3,-1,4,-5};
    for(int i=1;i<=5;i++)a[i]=s[i],br[i]=s[i];
    seg.src=a;
    seg.build(1,n,1);
    SegmentTree<ll,205>::Node res=seg.query(1,5,1,n,1);
    printf("小样例: mx[1,5]=%lld sum[1,5]=%lld mx[2,4]=%lld\n",res.mx,res.sum,seg.query(2,4,1,n,1).mx);
    return 0;
}
