// 链接剖分LCT 的测试与对拍代码
// 模板本体：02-数据结构/链接剖分LCT.cpp
#include "../../02-数据结构/链接剖分LCT.cpp"

int main()
{
    mt19937 rng(571);
    int bad=0,cnt=0;
    LCT sample(3);
    sample.set_val(1,2),sample.set_val(2,3),sample.set_val(3,5);
    sample.link(1,2),sample.link(2,3);
    ll sum=0;
    bad+=!sample.query(1,3,sum)||sum!=10;
    bad+=sample.cut(1,3)||!sample.cut(2,3)||sample.connected(1,3);
    for(int t=1;t<=20;t++)
    {
        int n=15;
        LCT tr(n);
        vector<vector<int>> g(n+1,vector<int>(n+1));
        vector<ll> a(n+1);
        for(int q=1;q<=1200;q++)
        {
            int x=rng()%n+1,y=rng()%n+1,op=rng()%4;
            vector<int> p=path(g,x,y);
            if(op==0)
            {
                bool ok=p.empty();
                bad+=tr.link(x,y)!=ok;
                if(ok)g[x][y]=g[y][x]=1;
            }
            else if(op==1)
            {
                bool ok=g[x][y];
                bad+=tr.cut(x,y)!=ok;
                if(ok)g[x][y]=g[y][x]=0;
            }
            else if(op==2)a[x]=(int)(rng()%101)-50,tr.set_val(x,a[x]);
            else
            {
                ll z=0;
                for(int u:p)z+=a[u];
                bool ok=tr.query(x,y,sum);
                bad+=ok!=!p.empty();
                if(ok)bad+=sum!=z;
                cnt++;
            }
            bad+=tr.connected(x,y)!=!path(g,x,y).empty();
        }
    }
    printf("LCT 路径和/link/cut: %s, 查询=%d\n",bad?"FAILED":"OK",cnt);
    if(bad)return 1;
    return 0;
}
