#include "../../04-图论/存图(vector邻接表).cpp"
int main()
{
    n=4;m=5;
    int u[]={0,1,1,2,3,3},v[]={0,2,3,4,1,4},w[]={0,7,2,5,1,9};
    for(int i=1;i<=m;i++)add_edge(u[i],v[i],w[i]);
    int count=0;
    for(int x=1;x<=n;x++)count+=adj[x].size();
    assert(count==m&&adj[1][0].to==2&&adj[1][0].w==7);
    for(int x=1;x<=n;x++)adj[x].clear();
    for(int i=1;i<=m;i++)add_undirected(u[i],v[i],w[i]);
    count=0;
    for(int x=1;x<=n;x++)count+=adj[x].size();
    assert(count==2*m);cout<<"vector graph OK\n";
}
