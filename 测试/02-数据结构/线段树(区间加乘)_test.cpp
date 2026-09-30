// 线段树(区间加乘) 的测试与对拍代码
// 模板本体：02-数据结构/线段树(区间加乘).cpp
#include "../../02-数据结构/线段树(区间加乘).cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：大模数 1e9+7 和小模数 97 各跑一遍，与暴力数组对拍
void run(ll md,ll &bad,ll &cnt)
{
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,50);
        seg.mod=md;
        for(int i=1;i<=n;i++)a[i]=rnd(0,100)%md,br[i]=a[i];
        seg.src=a;
        seg.build(1,n,1);
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,3),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==3)
            {
                ll x=seg.query(l,r,1,n,1),z=0;
                for(int i=l;i<=r;i++)z=(z+br[i])%md;
                cnt++;
                if(x!=z)bad++;
                continue;
            }
            ll v=rnd(0,60);
            if(op==1)
            {
                seg.update_mul(l,r,v,1,n,1);
                for(int i=l;i<=r;i++)br[i]=br[i]*v%md;
            }
            else
            {
                seg.update_add(l,r,v,1,n,1);
                for(int i=l;i<=r;i++)br[i]=(br[i]+v)%md;
            }
        }
    }
}

int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    run(1000000007,bad,cnt);
    run(97,bad,cnt);
    printf("线段树(区间加乘) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,1,1]，先 [1,3] 乘 2、再 [1,3] 加 3，区间和为 15
    n=3,seg.mod=1000000007;
    for(int i=1;i<=3;i++)a[i]=1;
    seg.src=a;
    seg.build(1,n,1);
    seg.update_mul(1,3,2,1,n,1);
    seg.update_add(1,3,3,1,n,1);
    printf("小样例: sum[1,3]=%lld sum[2,2]=%lld\n",seg.query(1,3,1,n,1),seg.query(2,2,1,n,1));
    return 0;
}
