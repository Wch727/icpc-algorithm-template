// 线性基 的测试与对拍代码
// 模板本体：05-数学/线性基.cpp
#include "../../05-数学/线性基.cpp"

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        LinearBasis z; for(int i=0;i<63;i++)z.insert(1LL<<i); z.build();
        assert(z.query_kth(1ULL<<63)==LLONG_MAX&&z.query_kth(0)==-1);
    }

    int bad=0;
    mt19937_64 rnd(20250707);
    // 1) 小数组：最大异或和与 2^n 暴力枚举子集对拍；第 k 小与排序后的全集对拍
    for(int t=1;t<=500;t++)
    {
        int n=rnd()%10+1;
        ll a[12];
        LinearBasis lb;
        for(int i=1;i<=n;i++)
        {
            a[i]=rnd()%(1LL<<20);
            lb.insert(a[i]);
        }
        // 暴力枚举所有子集异或，去重后就是线性基能表示的全部值
        vector<ll> all;
        for(int s=0;s<(1<<n);s++)
        {
            ll x=0;
            for(int i=1;i<=n;i++)
                if(s>>(i-1)&1)x^=a[i];
            all.push_back(x);
        }
        sort(all.begin(),all.end());
        all.erase(unique(all.begin(),all.end()),all.end());
        if(lb.query_max()!=all.back())bad++;
        if((ll)all.size()!=(1LL<<lb.cnt))bad++;
        // zero 标记：所有 n 个向量线性相关（秩 < n）时才能异或出 0
        if((lb.zero)!=(lb.cnt<n))bad++;
        // 直接用集合大小独立判一次：秩 < n 等价于 2^n 个子集去重后少于 2^n 个
        if((lb.cnt<n)!=(all.size()<(1ULL<<n)))bad++;
        // check 与暴力集合比对，并抽一个不在集合里的数
        for(int s=0;s<(1<<n);s++)
        {
            ll x=0;
            for(int i=1;i<=n;i++)
                if(s>>(i-1)&1)x^=a[i];
            if(!lb.check(x))
            {
                bad++;
                if(bad<=5)printf("CHECK FAIL t=%d x=%lld\n",t,x);
            }
        }
        if(lb.check((1LL<<40)+12345)&&(lb.cnt<40))bad++;
        // 第 k 小：0 排第 1 位（由空集取到），之后是非零值升序，共 2^cnt 个
        vector<ll> seq;
        seq.push_back(0);
        for(int i=0;i<(int)all.size();i++)
            if(all[i]!=0)seq.push_back(all[i]);
        lb.build();
        for(int k=0;k<(int)seq.size();k++)
            if(lb.query_kth((ll)k+1)!=seq[k])
            {
                bad++;
                if(bad<=5)printf("KTH FAIL t=%d k=%d got=%lld want=%lld cnt=%d zero=%d\n",
                                 t,k+1,lb.query_kth((ll)k+1),seq[k],lb.cnt,(int)lb.zero);
            }
        if(lb.query_kth((ll)seq.size()+1)!=-1)bad++;// 越界必须返回 -1
    }
    // 2) 大数不参与暴力，只验证最大异或和可达
    LinearBasis lb2;
    ll seed=1;
    for(int i=1;i<=50;i++)
    {
        seed=seed*6364136223846793005ULL+1442695040888963407ULL;
        lb2.insert(seed>>3);
    }
    if(!lb2.check(lb2.query_max()))bad++;
    // 3) 边界：重复插入同一数会置 zero；单元素基
    LinearBasis lb3;
    lb3.insert(5),lb3.insert(5);
    if(!lb3.zero)bad++;
    if(!lb3.check(0))bad++;
    if(lb3.query_max()!=5)bad++;
    lb3.build();
    if(lb3.query_kth(1)!=0)bad++;// 最小的一定是 0
    if(lb3.query_kth(2)!=5)bad++;
    if(lb3.query_kth(3)!=-1)bad++;

    LinearBasis demo;
    int arr[5]={0,1,2,4,8};
    for(int i=1;i<=4;i++)demo.insert(arr[i]);
    printf("max xor of {1,2,4,8} = %lld (expect 15)\n",demo.query_max());
    demo.build();
    printf("kth 1..4 = ");
    for(int k=1;k<=4;k++)printf("%lld ",demo.query_kth(k));
    printf("\n(1..2^rank 枚举整个张成，第 1 个是 0)\n");
    printf("\n5 xor 3 achievable? %d (expect 1)\n",(int)demo.check(5^3));
    printf("rank of {1,2,4,8} = %d, zero reachable = %d\n",demo.cnt,(int)demo.zero);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1469 找筷子，出现奇数次的数 = 全部异或和（线性基是异或性质的推广）
// 边界：query_kth 的 k 从 1 开始；k 超过 2^cnt 返回 -1；zero 只表示 0 可达

/*
自测记录：
  1) 500 组随机小数组（n<=10，值 <2^20）：最大异或和、秩、zero 标记、check()、第 k 小全部与子集枚举对拍；
  2) 50 个随机大数插入后验证最大异或和可达；
  3) 重复插入（zero=true）与单元素基的边界。
  query_kth 语义：k 从 1 开始，第 1 小固定是 0，共 2^rank 个值，越界返回 -1。
*/
