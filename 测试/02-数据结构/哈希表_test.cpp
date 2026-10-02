// 哈希表 的测试与对拍代码
// 模板本体：02-数据结构/哈希表.cpp
#include "../../02-数据结构/哈希表.cpp"

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        ho.init();
        for(int i=0;i<N;i++)assert(ho.insert(i));
        assert(!ho.insert(N)); assert(!ho.count(N)); assert(ho.count(0));
        ho.init();
    }

    srand(20240513);

    hc.init(),ho.init();
    // 1. 小数据手测（含负数、0、重复值）
    int a[]={1,0,-1,-1,5,1000000007,-1000000007,3};
    for(int i=0;i<8;i++)
    {
        hc.insert(a[i]);
        ho.insert(a[i]);
    }
    printf("chain: 1=%d -1=%d 7=%d 1e9+7=%d -1e9-7=%d\n",
        (int)hc.find(1),(int)hc.find(-1),(int)hc.find(7),(int)hc.find(1000000007),(int)hc.find(-1000000007));
    printf("open : 1=%d -1=%d 7=%d 1e9+7=%d -1e9-7=%d\n",
        (int)ho.count(1),(int)ho.count(-1),(int)ho.count(7),(int)ho.count(1000000007),(int)ho.count(-1000000007));

    // 2. 随机对拍：两个哈希表都对照 set
    for(int T=1;T<=5;T++)
    {
        hc.init(),ho.init();
        set<int> st;
        n=5000;
        for(int i=1;i<=n;i++)
        {
            // 故意造大量重复和负数
            x=rand()%3000-1500;
            hc.insert(x),ho.insert(x),st.insert(x);
        }
        bool ok=true;
        for(int q=1;q<=5000;q++)
        {
            x=rand()%6000-3000;
            bool want=(st.count(x)>0);
            if(hc.find(x)!=want||ho.count(x)!=want){ok=false;break;}
        }
        printf("round %d %s\n",T,ok?"passed":"FAILED");
        if(!ok)break;
    }

    // 3. 简单应用：P4305 去重（按输入顺序输出第一次出现的数）
    // 这里直接把哈希表当判重器用
    hc.init();
    int b[]={1,2,1,3,2,4};
    for(int i=0;i<6;i++)
    {
        if(!hc.find(b[i]))
        {
            hc.insert(b[i]);
            printf("%d ",b[i]);
        }
    }
    printf("\n");
    return 0;
}
