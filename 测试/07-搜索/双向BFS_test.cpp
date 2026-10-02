// 双向BFS 的测试与对拍代码
// 模板本体：07-搜索/双向BFS.cpp
#include "../../07-搜索/双向BFS.cpp"

// ---------- 三、自测 ----------

mt19937 rnd(20240607);

int rand_int(int l,int r)//[l,r]
{
    return l+(int)(rnd()%(unsigned)(r-l+1));
}

// 独立单向 BFS，直接改对应数字，不调用模板的转移函数
int bfs_one_num(ull s,ull t,int d)
{
    int save=dig;
    dig=d;
    unordered_map<ull,int> dis;
    queue<ull> q;
    dis[s]=0,q.push(s);
    while(!q.empty())
    {
        ull u=q.front();q.pop();
        if(u==t){dig=save;return dis[u];}
        for(int k=0;k<d;k++)
        {
            ull base=1ULL<<(4*k),v=(u/base)%16;
            ull w=u-v*base+((v+1)%10)*base;
            if(dis.count(w))continue;
            dis[w]=dis[u]+1;
            q.push(w);
        }
    }
    dig=save;
    return -1;
}

// 单向 BFS，只用来给双向 BFS 对答案（数组开全局，别放 1MB 的栈上）
int one_d[N][N];

int bfs_one(int sx,int sy,int ex,int ey)
{
    // 这一行不能少：起点或终点在墙上直接不可达，否则 BFS 会从墙里出发
    if(mp[sx][sy]=='#'||mp[ex][ey]=='#')return -1;
    int (*d)[N]=one_d;
    memset(d,-1,sizeof(one_d));
    queue<pair<int,int> > q;
    d[sx][sy]=0,q.push(make_pair(sx,sy));
    while(!q.empty())
    {
        int x=q.front().first,y=q.front().second;q.pop();
        if(x==ex&&y==ey)return d[x][y];
        for(int i=1;i<=4;i++)
        {
            int xx=x+dx4[i],yy=y+dy4[i];
            if(xx<1||xx>n||yy<1||yy>m)continue;
            if(mp[xx][yy]=='#')continue;
            if(d[xx][yy]!=-1)continue;
            d[xx][yy]=d[x][y]+1;
            q.push(make_pair(xx,yy));
        }
    }
    return -1;
}

int g_fail;

void check(int id,int got,int want,const char *msg)
{
    if(got!=want)
    {
        g_fail++;
        printf("  第 %d 组失败: %s 得到 %d 期望 %d\n",id,msg,got,want);
    }
}

void test_grid()
{
    // 固定小迷宫：两堵竖墙只留缺口，最短路要绕行
    n=7,m=7;
    const char *s[7]={
        ".......",
        ".#####.",
        ".#...#.",
        ".#.#.#.",
        ".#.#.#.",
        "...#...",
        "......."};
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)mp[i][j]=s[i-1][j-1];
    int a=bibfs_grid(1,1,n,m),b=bfs_one(1,1,n,m);
    check(1,a,b,"迷宫 (1,1)->(7,7)");
    printf("[grid] 7x7 迷宫 (1,1)->(7,7) 双向=%d 单向=%d %s\n",a,b,a==b?"OK":"FAILED");

    // 起点和终点重合
    check(2,bibfs_grid(3,3,3,3),0,"原地不动");
    // 终点是墙 -> 不可达
    mp[5][5]='#';
    check(3,bibfs_grid(1,1,5,5),-1,"终点是墙");
    mp[5][5]='.';
    // 终点被墙完全围死 -> 不可达
    mp[6][5]=mp[5][6]=mp[5][4]=mp[4][5]='#';
    check(4,bibfs_grid(1,1,5,5),-1,"终点被围死");
    mp[6][5]=mp[5][6]=mp[5][4]=mp[4][5]='.';
}

void test_grid_stress()
{
    // 随机小迷宫大规模对拍（墙的比例别太高，否则一半的组都是起点/终点在墙上）
    int bad=0,reach=0,ok=0;
    for(int t=1;t<=300;t++)
    {
        n=rand_int(2,12),m=rand_int(2,12);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)mp[i][j]=(rand_int(1,100)<=20)?'#':'.';
        int sx=rand_int(1,n),sy=rand_int(1,m),ex=rand_int(1,n),ey=rand_int(1,m);
        int a=bibfs_grid(sx,sy,ex,ey),b=bfs_one(sx,sy,ex,ey);
        if(a!=b)bad++;
        else ok++;
        if(a!=-1)reach++;//统计有多少组真的有解，避免全在比 -1
    }
    g_fail+=bad;
    printf("[grid] 随机迷宫 300 组对拍 %s (相同 %d 不同 %d，其中有解 %d 组)\n",
        bad?"FAILED":"OK",ok,bad,reach);
}

int main()
{
    test_grid();
    test_grid_stress();

    // 数字变换：固定小例子（十进制 a 拆成 state 时个位在下标 0，这里直接给 state）
    dig=2;
    // 原期望及编码写错：数字按每位 4 bit 编码，00 到 99 要 18 步
    check(100,bibfs_num(0,0x99),18,"00->99");
    check(101,bibfs_num(0x12,0x34),4,"12->34");
    printf("[num] 2 位数 00->99 需 %d 步 (期望 18)  12->34 需 %d 步 (期望 4)\n",
        bibfs_num(0,0x99),bibfs_num(0x12,0x34));

    // 和单向 BFS + 公式对拍。
    // 每一位的 +1 是模 10 的循环，所以第 i 位要 (y_i-x_i+10)%10 步，各步独立。
    int bad=0;
    for(int t=1;t<=60;t++)
    {
        dig=rand_int(1,4);
        int a=rand_int(0,9),b=rand_int(0,9);
        for(int k=1;k<dig;k++)a=a*10+rand_int(0,9),b=b*10+rand_int(0,9);
        ull x=0,y=0;
        int ta=a,tb=b;
        for(int k=0;k<dig;k++)x|=(ull)(ta%10)<<(4*k),y|=(ull)(tb%10)<<(4*k),ta/=10,tb/=10;
        int want=0;
        ta=a,tb=b;
        for(int k=0;k<dig;k++)
        {
            want+=(tb%10-ta%10+10)%10;
            ta/=10,tb/=10;
        }
        int r1=bibfs_num(x,y);
        int r2=bfs_one_num(x,y,dig);
        if(r1!=want||r2!=want)
        {
            bad++;
            if(bad<=3)printf("  第 %d 组: a=%d b=%d dig=%d 双向=%d 单向=%d 期望=%d\n",t,a,b,dig,r1,r2,want);
        }
    }
    g_fail+=bad;
    printf("[num] 随机数字变换 60 组对拍(单向 BFS + 公式) %s (不同 %d 组)\n",bad?"FAILED":"OK",bad);

    printf("[双向BFS] 全部通过 = %d\n",g_fail==0?1:0);
    return 0;
}
