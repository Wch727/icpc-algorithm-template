// 莫队算法 的测试与对拍代码
// 模板本体：02-数据结构/莫队算法.cpp
#include "../../02-数据结构/莫队算法.cpp"

int ql[N],qr[N];                 // 自测用：保留原始询问左右端点（solve 会把 q 排序）

int main()
{
    srand(20240513);

    // 1. 手测：a = 1 2 1 3 2 4，询问 [1,3] [2,5] [1,6]
    n=6;
    int ini[]={0,1,2,1,3,2,4};
    for(int i=1;i<=n;i++)a[i]=ini[i];
    m=3;
    int qs[][3]={{1,3,0},{2,5,1},{1,6,2}};
    for(int i=0;i<m;i++)q[i+1].l=qs[i][0],q[i+1].r=qs[i][1],q[i+1].id=qs[i][2];
    solve();
    printf("hand(莫队): %d %d %d\n",ans[0],ans[1],ans[2]);
    printf("hand(暴力): ");
    for(int i=0;i<m;i++)
    {
        set<int> st;
        for(int j=qs[i][0];j<=qs[i][1];j++)st.insert(a[j]);
        printf("%d ",(int)st.size());
    }
    printf("\n");

    // 2. 随机多轮对拍：与 O(n) 暴力比较
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        n=rand()%60+1;
        for(int i=1;i<=n;i++)a[i]=rand()%10;     // 值域故意开小，重复多
        m=rand()%40+1;
        for(int i=1;i<=m;i++)
        {
            int l=rand()%n+1,r=rand()%n+1;
            if(l>r)swap(l,r);
            q[i].l=l,q[i].r=r,q[i].id=i;
            ql[i]=l,qr[i]=r;                     // solve 会把 q 排序，先留一份原始询问
        }
        solve();
        for(int i=1;i<=m;i++)
        {
            set<int> st;
            for(int j=ql[i];j<=qr[i];j++)st.insert(a[j]);
            if((int)st.size()!=ans[i]){ok=false;break;}
        }
        printf("mo round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 大规模：n=m=100000，随机询问，验证不超时
    n=100000;
    for(int i=1;i<=n;i++)a[i]=rand()%1000;
    m=100000;
    for(int i=1;i<=m;i++)
    {
        int l=rand()%n+1,r=rand()%n+1;
        if(l>r)swap(l,r);
        q[i].l=l,q[i].r=r,q[i].id=i;
        ql[i]=l,qr[i]=r;
    }
    solve();
    // 抽查 20 个询问
    bool big=true;
    for(int i=1;i<=20;i++)
    {
        int id=rand()%m+1;
        set<int> st;
        for(int j=ql[id];j<=qr[id];j++)st.insert(a[j]);
        if((int)st.size()!=ans[id]){big=false;break;}
    }
    printf("big test %s, q1=(%d,%d) ans=%d\n",big?"passed":"FAILED",ql[1],qr[1],ans[1]);

    // 4. 另一种查询：区间内出现奇数次的数的种类数（改 add/del 即可）
    // 这里只演示写法：维护 cnt 的奇偶，改动都是 O(1)
    return 0;
}
