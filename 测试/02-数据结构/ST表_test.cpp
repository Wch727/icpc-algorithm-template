// ST表 的测试与对拍代码
// 模板本体：02-数据结构/ST表.cpp
#include "../../02-数据结构/ST表.cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：每个单点区间 + 随机区间，最大/最小都与暴力对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=20;t++)
    {
        n=rnd(1,60);
        for(int i=1;i<=n;i++)a[i]=rnd(-1000,1000);
        stx.build(n,a);
        stn.build(n,a);
        for(int l=1;l<=n;l++)
            for(int r=l;r<=n;r++)
            {
                int mx=-0x3f3f3f3f,mn=0x3f3f3f3f;
                for(int i=l;i<=r;i++)mx=max(mx,a[i]),mn=min(mn,a[i]);
                cnt++;
                if(stx.query(l,r)!=mx||stn.query(l,r)!=mn)bad++;
            }
    }
    printf("ST表(区间最大/最小) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[3,1,4,1,5]，max[1,5]=5 min[2,4]=1 max[3,3]=4
    n=5;
    int s[6]={0,3,1,4,1,5};
    for(int i=1;i<=5;i++)a[i]=s[i];
    stx.build(n,a);
    stn.build(n,a);
    printf("小样例: max[1,5]=%d min[2,4]=%d max[3,3]=%d\n",stx.query(1,5),stn.query(2,4),stx.query(3,3));
    return 0;
}
