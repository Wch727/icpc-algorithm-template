// 点分治 的测试与对拍代码
// 模板本体：04-图论/点分治.cpp
#include "../../04-图论/点分治.cpp"

int main()
{
    mt19937 rnd(7123);
    int bad=0;
    Centroid a(5,2);
    for(int i=1;i<5;i++)a.add_edge(i,i+1);
    bad+=a.run()!=3;
    for(int t=1;t<=300;t++)
    {
        int n=rnd()%35+1,k=rnd()%(n+3);
        Centroid tr(n,k);
        for(int i=2;i<=n;i++)tr.add_edge(i,rnd()%(i-1)+1);
        ll want=0;
        for(int s=1;s<=n;s++)
        {
            vector<int> d(n+1,-1),q(1,s);
            d[s]=0;
            for(int i=0;i<(int)q.size();i++)for(int v:tr.adj[q[i]])if(d[v]<0)d[v]=d[q[i]]+1,q.push_back(v);
            for(int v=s+1;v<=n;v++)want+=d[v]==k;
        }
        bad+=tr.run()!=want;
        bad+=tr.run()!=want;// 重复调用不能遗留桶和删除标记
    }
    Centroid tr(100000,1);
    for(int i=1;i<100000;i++)tr.add_edge(i,i+1);
    bad+=tr.run()!=99999;
    Centroid star(100000,2);
    for(int i=2;i<=100000;i++)star.add_edge(1,i);
    bad+=star.run()!=99999LL*99998/2;
    printf("点分治：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
