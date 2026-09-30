// BSGS与扩展BSGS 的测试与对拍代码
// 模板本体：05-数学/BSGS与扩展BSGS.cpp
#include "../../05-数学/BSGS与扩展BSGS.cpp"

// 暴力遍历余数直到重复，不调用模板中的幂函数

ll brute(ll a,ll b,int p)
{
    vector<int> vis(p);
    ll cur=1%p;
    for(int x=0;!vis[cur];x++,cur=cur*a%p)
    {
        if(cur==b)return x;
        vis[cur]=1;
    }
    return -1;
}

int main()
{
    mt19937 rnd(931);
    int bad=0;
    bad+=bsgs(2,8,13)!=3;
    bad+=exbsgs(2,0,8)!=3;
    bad+=exbsgs(0,0,8)!=1;
    // 小模数穷举，含零底数、无解和指数为零
    for(int p=1;p<=35;p++)
        for(int a=0;a<p;a++)
            for(int b=0;b<p;b++)
            {
                ll want=brute(a,b,p);
                bad+=exbsgs(a,b,p)!=want;
                bad+=exbsgs(a-p,b-p,p)!=want;
                if(gcd(a,p)==1)bad+=bsgs(a,b,p)!=want;
            }
    for(int t=1;t<=1000;t++)
    {
        int p=rnd()%150+1,a=rnd()%p,b=rnd()%p;
        ll want=brute(a,b,p),got=exbsgs(a,b,p);
        bad+=got!=want;
        if(gcd(a,p)==1)bad+=bsgs(a,b,p)!=want;
    }
    printf("BSGS 与扩展 BSGS：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
