// 扫描线：离散化 + 线段树，求矩形面积并 / 周长并
// 复杂度 O(n log n)，n 是矩形个数
// 线段树维护 y 轴上的「被覆盖总长度」len：
//   cov[p]：这段区间被整段覆盖了几次（不往下推，是标记永久化写法）
//   len[p]：这段区间内被覆盖的长度；cov[p]>0 时就是整段长度
// 求面积并：每条竖直扫描线处 area += len[1]*(y[i+1]-y[i])
// 求周长并：竖边贡献 2*num[1]（num 是覆盖区间的段数），
//           横边贡献 |len_i - len_{i+1}|（相邻扫描线之间的变化量）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;

int ys[N<<1],ycnt;//所有 y 坐标离散化
ll cov[N<<3],len[N<<3],num[N<<3];//覆盖次数、覆盖长度、覆盖段数

struct Ev{
    int x,y1,y2,typ;//typ=1 左边界(入)，typ=-1 右边界(出)
    bool operator<(const Ev &o)const{return x<o.x;}//按 x 升序扫描
};

vector<Ev> ev;

void push_up(int p,int l,int r)
{
    if(cov[p]>0)//整段被覆盖，长度就是整段
    {
        len[p]=ys[r+1]-ys[l];
        num[p]=1;
        return;
    }
    if(l==r)//叶子且没被覆盖
    {
        len[p]=0,num[p]=0;
        return;
    }
    int lp=p<<1,rp=(p<<1)|1;
    len[p]=len[lp]+len[rp];
    // 左孩子右端被覆盖 且 右孩子左端被覆盖 -> 两段能接起来
    num[p]=num[lp]+num[rp]-(cov[lp]>0&&cov[rp]>0?1:0);
}

void update(int L,int R,ll v,int l,int r,int p)
{
    if(L<=l&&r<=R)//(l,r)<=(L,R)
    {
        cov[p]+=v;
        push_up(p,l,r);
        return;
    }
    int mid=(l+r)>>1;
    if(L<=mid)update(L,R,v,l,mid,p<<1);
    if(R>mid)update(L,R,v,mid+1,r,(p<<1)|1);
    push_up(p,l,r);
}

// 用当前 ev 求面积并，ycnt 是 y 坐标个数（实际线段下标 1..ycnt-1）
ll union_area()
{
    if(ev.empty())return 0;
    for(int i=1;i<=(ycnt<<2);i++)cov[i]=len[i]=num[i]=0;
    int m=ycnt-1;
    ll area=0;
    for(size_t i=0;i<ev.size();i++)
    {
        int l=lower_bound(ys+1,ys+ycnt+1,ev[i].y1)-ys;
        int r=lower_bound(ys+1,ys+ycnt+1,ev[i].y2)-ys-1;//区间 [y1,y2) 对应线段 [l,r]
        update(l,r,ev[i].typ,1,m,1);
        if(i+1<ev.size())area+=len[1]*(ll)(ev[i+1].x-ev[i].x);
    }
    return area;
}

// 同时返回面积并和周长并
pair<ll,ll> union_area_peri()
{
    sort(ev.begin(),ev.end());
    if(ev.empty())return make_pair(0,0);
    for(int i=1;i<=(ycnt<<2);i++)cov[i]=len[i]=num[i]=0;
    int m=ycnt-1;
    ll area=0,peri=0,last=0;
    for(size_t i=0;i<ev.size();i++)
    {
        int l=lower_bound(ys+1,ys+ycnt+1,ev[i].y1)-ys;
        int r=lower_bound(ys+1,ys+ycnt+1,ev[i].y2)-ys-1;//区间 [y1,y2) 对应线段 [l,r]
        update(l,r,ev[i].typ,1,m,1);
        if(i+1<ev.size())
        {
            ll d=ev[i+1].x-ev[i].x;
            area+=len[1]*d;             // 竖着扫过去的一条条竖条
            peri+=2*num[1]*d;           // 竖边：num[1] 段覆盖，每段左右各一条
        }
        if(i)peri+=llabs(len[1]-last);  // 横边：与上一条扫描线的长度之差
        last=len[1];
    }
    return make_pair(area,peri);
}

const int TESTN=2005;
int rx1[TESTN],ry1[TESTN],rx2[TESTN],ry2[TESTN];
int g[30][30];

// 小坐标暴力：格点打标记数格子
ll brute_area(int n,int W)
{
    for(int i=0;i<W;i++)for(int j=0;j<W;j++)g[i][j]=0;
    for(int i=1;i<=n;i++)
        for(int x=rx1[i];x<rx2[i];x++)
            for(int y=ry1[i];y<ry2[i];y++)g[x][y]=1;
    ll s=0;
    for(int i=0;i<W;i++)for(int j=0;j<W;j++)s+=g[i][j];
    return s;
}

// 数格子边界：一个被覆盖的格子和一个没被覆盖的邻居之间贡献一条单位边
ll brute_peri(int n,int W)
{
    for(int i=0;i<W;i++)for(int j=0;j<W;j++)g[i][j]=0;
    for(int i=1;i<=n;i++)
        for(int x=rx1[i];x<rx2[i];x++)
            for(int y=ry1[i];y<ry2[i];y++)g[x][y]=1;
    ll c=0;
    for(int i=0;i<W;i++)
        for(int j=0;j<W;j++)
        {
            if(!g[i][j])continue;
            if(i==0||!g[i-1][j])c++;
            if(i==W-1||!g[i+1][j])c++;
            if(j==0||!g[i][j-1])c++;
            if(j==W-1||!g[i][j+1])c++;
        }
    return c;
}

void build_ev(int n)
{
    ev.clear();
    ycnt=0;
    for(int i=1;i<=n;i++)
    {
        ev.push_back(Ev{ry1[i],ry2[i],1,rx1[i]});
        ev.push_back(Ev{ry1[i],ry2[i],-1,rx2[i]});
        ys[++ycnt]=ry1[i],ys[++ycnt]=ry2[i];
    }
    sort(ys+1,ys+ycnt+1);
    ycnt=unique(ys+1,ys+ycnt+1)-ys-1;
}

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

int main()
{
    srand(20240513);
    bool ok=true;

    // 1. 手算样例：[0,2]x[0,2] 和 [1,3]x[1,3] -> 面积 7，周长 16
    {
        int n=2;
        rx1[1]=0,ry1[1]=0,rx2[1]=2,ry2[1]=2;
        rx1[2]=1,ry1[2]=1,rx2[2]=3,ry2[2]=3;
        build_ev(n);
        pair<ll,ll> r=union_area_peri();
        printf("手算样例: 面积=%lld(应=7) 周长=%lld(应=16)\n",r.first,r.second);
        if(r.first!=7||r.second!=16)ok=false;
    }

    // 2. 随机对拍：坐标 0..9 的小矩形，面积/周长都跟暴力比
    for(int T=1;T<=50&&ok;T++)
    {
        int n=rnd(1,6);
        for(int i=1;i<=n;i++)
        {
            int a=rnd(0,8),b=rnd(0,8),c=rnd(0,8),d=rnd(0,8);
            rx1[i]=min(a,c),rx2[i]=max(a,c);
            ry1[i]=min(b,d),ry2[i]=max(b,d);
            if(rx1[i]==rx2[i])rx2[i]++;
            if(ry1[i]==ry2[i])ry2[i]++;
        }
        build_ev(n);
        pair<ll,ll> got=union_area_peri();
        ll wa=brute_area(n,20),wp=brute_peri(n,20);
        if(got.first!=wa||got.second!=wp)
        {
            printf("第 %d 轮错: 面积 got=%lld want=%lld, 周长 got=%lld want=%lld\n",
                T,got.first,wa,got.second,wp);
            for(int i=1;i<=n;i++)printf("  矩形[%d,%d]x[%d,%d]\n",rx1[i],rx2[i],ry1[i],ry2[i]);
            ok=false;
        }
    }
    printf("面积/周长并 随机对拍 %s\n",ok?"passed":"FAILED");

    // 3. 单个矩形：面积 = w*h，周长 = 2(w+h)
    {
        int n=1;
        rx1[1]=3,ry1[1]=4,rx2[1]=10,ry2[1]=9;
        build_ev(n);
        pair<ll,ll> r=union_area_peri();
        printf("单矩形: 面积=%lld(应=35) 周长=%lld(应=24)\n",r.first,r.second);
    }

    // 4. 规模测试：1e5 个随机矩形
    {
        ev.clear();
        ycnt=0;
        for(int i=1;i<=100000;i++)
        {
            int x=rnd(0,1000000000),y=rnd(0,1000000000);
            int w=rnd(1,1000),h=rnd(1,1000);
            rx1[i]=x,ry1[i]=y,rx2[i]=x+w,ry2[i]=y+h;
        }
        build_ev(100000);
        pair<ll,ll> r=union_area_peri();
        printf("规模: 100000 个矩形 -> 面积=%lld 周长=%lld\n",r.first,r.second);
    }

    printf("结果: %s\n",ok?"OK":"FAILED");
    return 0;
}
