// 并查集 的测试与对拍代码
// 模板本体：02-数据结构/并查集.cpp
#include "../../02-数据结构/并查集.cpp"

int id[305];//暴力：直接记每个点当前属于哪个连通块

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

int brute_same(int x,int y)
{
    return id[x]==id[y];
}

void brute_union(int x,int y)
{
    int a=id[x],b=id[y];
    if(a==b)return;
    for(int i=1;i<=n;i++)
        if(id[i]==b)id[i]=a;
}

// 自测：随机合并/查询，与暴力连通块编号对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=3;t++)
    {
        n=300;
        d.init(n);
        for(int i=1;i<=n;i++)id[i]=i;
        for(int q=1;q<=5000;q++)
        {
            int op=rnd(1,2),x=rnd(1,n),y=rnd(1,n);
            if(op==1)d.unionn(x,y),brute_union(x,y);
            else
            {
                cnt++;
                if(d.same(x,y)!=brute_same(x,y))bad++;
            }
        }
    }
    printf("并查集 vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：n=5，合并 (1,2)(3,4)(2,3) 后 {1,2,3,4} 连通
    n=5;
    d.init(n);
    d.unionn(1,2),d.unionn(3,4),d.unionn(2,3);
    printf("小样例: same(1,4)=%d same(1,5)=%d size(1)=%d\n",d.same(1,4),d.same(1,5),d.size(1));
    return 0;
}
