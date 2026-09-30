// 李超线段树：插入直线 y=kx+b，查询某个 x 处的最大值
// 插入 O(log V)，查询 O(log V)，V 是定义域大小；每次插入只留一条「当前最优」直线
// 定义域写法两种：
//   1) 连续整数域 [XLO,XHI]（结构体内部按区间递归，不建满整棵树）
//   2) 离散化定义域：只关心给定的若干 x 坐标，把它们压缩成 1..m
// 坑：斜率/截距要精确比较，不能直接用 double（模板里用分数 + __int128 交叉相乘）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const int N=200005;

struct Line{
    ll A,B,C;//y=(A*x+B)/C，C>0，A,B,C 都为 0 表示这条线不存在
    Line(ll A_=0,ll B_=0,ll C_=1){A=A_,B=B_,C=C_;if(C<0)A=-A,B=-B,C=-C;ll g=__gcd(__gcd(abs(A),abs(B)),C);if(g)A/=g,B/=g,C/=g;}
};

// 在 x 处 a 是否严格优于 b（x 是整数，乘 C 后是精确的整数比较）
bool better(const Line &a,const Line &b,ll x)
{
    if(a.C==0)return false;             // a 不存在
    if(b.C==0)return true;              // b 不存在
    __int128 va=(__int128)a.A*x*b.C+(__int128)a.B*b.C;
    __int128 vb=(__int128)b.A*x*a.C+(__int128)b.B*a.C;
    return va>vb;
}

lll calc(const Line &l,ll x){return (lll)l.A*x+l.B;}

// 连续定义域版：域是 [XLO,XHI]，每次插入最多走 O(log V) 层
struct LiChao{
    Line s[N<<2];
    ll XLO,XHI;
    void init(ll lo,ll hi)
    {
        XLO=lo,XHI=hi;
        for(int i=1;i<=(N<<2);i++)s[i]=Line();
    }
    void insert(Line x){insert(x,XLO,XHI,1);}
    void insert(Line x,ll l,ll r,int p)
    {
        if(x.C==0)return;
        ll mid=(l+r)>>1;
        bool bl=better(x,s[p],l),bm=better(x,s[p],mid);//在左端点和中点谁更优
        if(bm)swap(s[p],x);                 // 中点上更优的留在当前结点
        if(l==r)return;
        if(bl!=bm)insert(x,l,mid,p<<1);     // 交点落在左半边
        else insert(x,mid+1,r,(p<<1)|1);    // 交点落在右半边
    }
    ll query(ll x){return query(x,XLO,XHI,1);}
    ll query(ll x,ll l,ll r,int p)
    {
        lll res=s[p].C?calc(s[p],x):(lll)LLONG_MIN;
        if(l==r)return (ll)res;
        ll mid=(l+r)>>1;
        lll t=(x<=mid)?query(x,l,mid,p<<1):query(x,mid+1,r,(p<<1)|1);
        return (ll)max(res,t);
    }
};

// 离散定义域版：xs[1..m] 是允许查询的 x（升序去重），插入的是这些点上的最优值
struct LiChaoDis{
    Line s[N<<2];
    ll *xs;int m;
    void init(ll *xs_,int m_)
    {
        xs=xs_,m=m_;
        for(int i=1;i<=(N<<2);i++)s[i]=Line();
    }
    void insert(Line x){insert(x,1,m,1);}
    void insert(Line x,int l,int r,int p)
    {
        if(x.C==0)return;
        int mid=(l+r)>>1;
        bool bl=better(x,s[p],xs[l]),bm=better(x,s[p],xs[mid]);
        if(bm)swap(s[p],x);
        if(l==r)return;
        if(bl!=bm)insert(x,l,mid,p<<1);
        else insert(x,mid+1,r,(p<<1)|1);
    }
    // 只允许查 xs 里的坐标
    ll query(int id){return query(id,1,m,1);}
    ll query(int id,int l,int r,int p)
    {
        lll res=s[p].C?calc(s[p],xs[id]):(lll)LLONG_MIN;
        if(l==r)return (ll)res;
        int mid=(l+r)>>1;
        lll t=(id<=mid)?query(id,l,mid,p<<1):query(id,mid+1,r,(p<<1)|1);
        return (ll)max(res,t);
    }
};

// 把 y=ka*x+kb 变成 Line（ka、kb 是整数）
Line mk_line(ll ka,ll kb){return Line(ka,kb,1);}

const ll XLO=-1000000000LL,XHI=1000000000LL;
LiChao lichao;
LiChaoDis lcd;
Line all[N];
ll xs[N],qs[N];
int alln;

// 暴力：枚举所有直线在 x 处取最大
ll brute_max(ll x)
{
    ll res=LLONG_MIN;
    for(int i=1;i<=alln;i++)res=max(res,all[i].A*x+all[i].B);
    return res;
}

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

int main()
{
    srand(20240513);
    bool ok=true;

    // 1. 小样例：直线 y=0, y=x, y=2x-3, y=-3x+100
    {
        lichao.init(XLO,XHI);
        lichao.insert(mk_line(0,0));
        lichao.insert(mk_line(1,0));
        lichao.insert(mk_line(2,-3));
        lichao.insert(mk_line(-3,100));
        printf("小样例: x=-10 -> %lld(应=130)  x=0 -> %lld(应=100)  x=10 -> %lld(应=70)  x=100 -> %lld(应=17)\n",
            lichao.query(-10),lichao.query(0),lichao.query(10),lichao.query(100));
        if(lichao.query(-10)!=130||lichao.query(0)!=100||lichao.query(10)!=70||lichao.query(100)!=17)ok=false;
    }

    // 2. 随机对拍：连续域，与暴力枚举所有直线比较
    for(int T=1;T<=20&&ok;T++)
    {
        alln=rnd(1,20);
        lichao.init(XLO,XHI);
        for(int i=1;i<=alln;i++)
        {
            all[i]=mk_line(rnd(-1000,1000),rnd(-100000,100000));
            lichao.insert(all[i]);
        }
        for(int q=1;q<=300&&ok;q++)
        {
            ll x=rnd((int)XLO,(int)XHI);
            ll got=lichao.query(x),want=brute_max(x);
            if(got!=want)
            {
                printf("第 %d 轮错: x=%lld got=%lld want=%lld\n",T,x,got,want);
                ok=false;
            }
        }
    }
    printf("李超树(连续域) 随机对拍 %s\n",ok?"passed":"FAILED");

    // 3. 定义域离散化：只在给定的 200 个 x 上查，答案要跟暴力一致
    for(int T=1;T<=10&&ok;T++)
    {
        int m=0;
        while(m<200)
        {
            int x=rnd(-100000,100000);
            xs[++m]=x;
        }
        sort(xs+1,xs+m+1);
        m=unique(xs+1,xs+m+1)-xs-1;
        alln=rnd(1,15);
        lcd.init(xs,m);
        for(int i=1;i<=alln;i++)
        {
            all[i]=mk_line(rnd(-500,500),rnd(-50000,50000));
            lcd.insert(all[i]);
        }
        for(int i=1;i<=m&&ok;i++)
        {
            ll got=lcd.query(i),want=brute_max(xs[i]);
            if(got!=want)
            {
                printf("离散第 %d 轮错: x=%lld got=%lld want=%lld\n",T,xs[i],got,want);
                ok=false;
            }
        }
    }
    printf("李超树(离散定义域) 随机对拍 %s\n",ok?"passed":"FAILED");

    // 4. 斜率相同的直线：只留截距最大的那条；再加两条近乎重合的线
    {
        lichao.init(XLO,XHI);
        lichao.insert(mk_line(5,1));
        lichao.insert(mk_line(5,100));
        lichao.insert(mk_line(5,-100));
        lichao.insert(mk_line(1,0));
        ll g=lichao.query(50);
        printf("同斜率: x=50 -> %lld(应=350)\n",g);
        if(g!=350)ok=false;
    }

    // 5. 规模测试：1e5 条直线 + 1e5 次查询
    {
        lichao.init(XLO,XHI);
        for(int i=1;i<=100000;i++)lichao.insert(mk_line(rnd(-1000000,1000000),rnd(-1000000000,1000000000)));
        ll s=0;
        for(int i=1;i<=100000;i++)s+=lichao.query(rnd((int)XLO,(int)XHI));
        printf("规模: 1e5 条线 1e5 次查询 -> 询问和 %lld\n",s);
    }

    printf("结果: %s\n",ok?"OK":"FAILED");
    return 0;
}
