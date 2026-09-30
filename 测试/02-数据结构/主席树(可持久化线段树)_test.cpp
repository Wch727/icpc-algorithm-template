// 主席树(可持久化线段树) 的测试与对拍代码
// 模板本体：02-数据结构/主席树(可持久化线段树).cpp
#include "../../02-数据结构/主席树(可持久化线段树).cpp"

int main()
{
    srand(20240513);

    // 1. 手测：a = 1 5 2 6 3 7 4
    n=7;
    int ini[]={0,1,5,2,6,3,7,4};
    for(int i=1;i<=n;i++)a[i]=ini[i];
    build();
    printf("range[2,5] k=3 -> %d\n",b[ct.kth(ct.root[1],ct.root[5],1,sz,3)]);
    printf("range[1,7] k=1 -> %d, k=7 -> %d\n",b[ct.kth(ct.root[0],ct.root[7],1,sz,1)],b[ct.kth(ct.root[0],ct.root[7],1,sz,7)]);
    printf("cnt[2,5] in [1,3] = %d\n",count_range(2,5,1,3));

    // 2. 随机多轮对拍：区间第 k 小 / 区间计数 与暴力比较
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        n=rand()%30+1;
        for(int i=1;i<=n;i++)a[i]=rand()%50-25;     // 有重复、有负数
        build();
        for(int q=1;q<=40;q++)
        {
            int l=rand()%n+1,r=rand()%n+1;
            if(l>r)swap(l,r);
            int kk=rand()%(r-l+1)+1;
            vector<int> tmp(a+l,a+r+1);
            sort(tmp.begin(),tmp.end());
            if(b[ct.kth(ct.root[l-1],ct.root[r],1,sz,kk)]!=tmp[kk-1]){printf("第 %d 轮 kth 错 l=%d r=%d kk=%d\n",T,l,r,kk);ok=false;break;}
            int lo=rand()%40-20,hi=rand()%40-20;
            if(lo>hi)swap(lo,hi);
            int want=0;
            for(int i=l;i<=r;i++)if(a[i]>=lo&&a[i]<=hi)want++;
            int got=count_range(l,r,lo,hi);
            if(got!=want){printf("第 %d 轮 cnt 错 l=%d r=%d [%d,%d] got=%d want=%d\n",T,l,r,lo,hi,got,want);ok=false;break;}
        }
        printf("chair round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 全重复值：第 k 小恒为该值
    n=10;
    for(int i=1;i<=n;i++)a[i]=7;
    build();
    printf("all same: sz=%d kth1=%d kth10=%d cnt(all)=%d\n",sz,
        b[ct.kth(ct.root[0],ct.root[10],1,sz,1)],b[ct.kth(ct.root[0],ct.root[10],1,sz,10)],
        count_range(1,10,7,7));

    // 4. 规模测试：n=100000 个互不相同的值
    n=100000;
    for(int i=1;i<=n;i++)a[i]=i*7%100000;      // 互不相同的排列
    build();
    int kmin=ct.kth(ct.root[0],ct.root[n],1,sz,1);
    int kmax=ct.kth(ct.root[0],ct.root[n],1,sz,n);
    printf("big: kth(1,100000,1)=%d(应=%d) kth(1,100000,100000)=%d(应=%d) nodes=%d\n",
        b[kmin],b[1],b[kmax],b[sz],ct.tot);
    printf("big cnt in [1,50000] = %d (应=50000)\n",count_range(1,n,1,50000));
    int l=30000,r=70000,kk=12345;
    vector<int> tmp(a+l,a+r+1);
    sort(tmp.begin(),tmp.end());
    printf("big kth(30000,70000,12345)=%d (应=%d)\n",b[ct.kth(ct.root[l-1],ct.root[r],1,sz,kk)],tmp[kk-1]);
    return 0;
}
