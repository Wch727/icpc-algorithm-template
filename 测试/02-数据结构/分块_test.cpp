// 分块 的测试与对拍代码
// 模板本体：02-数据结构/分块.cpp
#include "../../02-数据结构/分块.cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：随机区间加 + 随机区间和，与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=30;t++)
    {
        n=rnd(1,80);
        for(int i=1;i<=n;i++)a[i]=rnd(-50,50),br[i]=a[i];
        blk.init(n,a);
        for(int q=1;q<=200;q++)
        {
            int op=rnd(1,2),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                ll v=rnd(-30,30);
                blk.update(l,r,v);
                for(int i=l;i<=r;i++)br[i]+=v;
            }
            else
            {
                ll x=blk.query(l,r),z=0;
                for(int i=l;i<=r;i++)z+=br[i];
                cnt++;
                if(x!=z)bad++;
            }
        }
    }
    printf("分块(区间加区间和) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,2,3,4,5]，[2,4] 加 10 后 sum[1,5]=25、sum[3,3]=13
    n=5;
    for(int i=1;i<=5;i++)a[i]=i;
    blk.init(n,a);
    blk.update(2,4,10);
    printf("小样例: sum[1,5]=%lld sum[3,3]=%lld\n",blk.query(1,5),blk.query(3,3));
    return 0;
}
