// 线段树(区间开方) 的测试与对拍代码
// 模板本体：02-数据结构/线段树(区间开方).cpp
#include "../../02-数据结构/线段树(区间开方).cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：随机区间开方 + 随机区间和，与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=20;t++)
    {
        n=rnd(1,60);
        for(int i=1;i<=n;i++)a[i]=rnd(0,1000000),br[i]=a[i];
        a[1]=0,a[n]=1;//边界：0 和 1 开方不变
        br[1]=a[1],br[n]=a[n];
        seg.src=a;
        seg.build(1,n,1);
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,2),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                seg.update(l,r,1,n,1);
                for(int i=l;i<=r;i++)br[i]=(ll)sqrt((double)br[i]);
            }
            else
            {
                ll x=seg.query(l,r,1,n,1),z=0;
                for(int i=l;i<=r;i++)z+=br[i];
                cnt++;
                if(x!=z)bad++;
            }
        }
    }
    printf("线段树(区间开方) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,4,9,16]，[1,4] 开方后为 [1,2,3,4]，和为 10
    n=4;
    for(int i=1;i<=4;i++)a[i]=(ll)(i+0)*(i+0);
    seg.src=a;
    seg.build(1,n,1);
    seg.update(1,4,1,n,1);
    printf("小样例: sum[1,4]=%lld sum[2,3]=%lld\n",seg.query(1,4,1,n,1),seg.query(2,3,1,n,1));
    return 0;
}
