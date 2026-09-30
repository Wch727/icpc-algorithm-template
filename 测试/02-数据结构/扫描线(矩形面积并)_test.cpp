// 扫描线(矩形面积并) 的测试与对拍代码
// 模板本体：02-数据结构/扫描线(矩形面积并).cpp
#include "../../02-数据结构/扫描线(矩形面积并).cpp"

void brute_fill(int nn,int W)
{
    for(int i=0;i<W;i++)for(int j=0;j<W;j++)g[i][j]=0;
    for(int i=1;i<=nn;i++)
        for(int x=rx1[i];x<rx2[i];x++)
            for(int y=ry1[i];y<ry2[i];y++)g[x][y]=1;
}

ll brute_area(int nn,int W)
{
    brute_fill(nn,W);
    ll s=0;
    for(int i=0;i<W;i++)for(int j=0;j<W;j++)s+=g[i][j];
    return s;
}

ll brute_peri(int nn,int W)
{
    brute_fill(nn,W);
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

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

int main()
{
    srand(20240513);
    bool ok=true;

    // 1. 手算样例：[0,2]x[0,2] 并 [1,3]x[1,3] -> 面积 7、周长 12；原期望 16 写错，暴力边界枚举为 12
    {
        n=2;
        rx1[1]=0,ry1[1]=0,rx2[1]=2,ry2[1]=2;
        rx1[2]=1,ry1[2]=1,rx2[2]=3,ry2[2]=3;
        printf("手算样例: 面积=%lld(应=7) 周长=%lld(应=12)\n",union_area(),union_perimeter());
        if(brute_area(n,20)!=7||brute_peri(n,20)!=12||union_area()!=7||union_perimeter()!=12)ok=false;
    }

    // 2. 单个矩形：面积 = w*h，周长 = 2(w+h)
    {
        n=1;
        rx1[1]=3,ry1[1]=4,rx2[1]=10,ry2[1]=9;
        printf("单矩形: 面积=%lld(应=35) 周长=%lld(应=24)\n",union_area(),union_perimeter());
        if(union_area()!=35||union_perimeter()!=24)ok=false;
    }

    // 共边、同坐标出入且长度相同但位置不同、重合、零面积
    int cases[4][2][4]={{{0,0,2,2},{2,0,4,2}},{{0,0,2,2},{2,2,4,4}},
        {{0,0,2,2},{0,0,2,2}},{{0,0,2,2},{2,0,2,2}}};
    for(int t=0;t<4;t++)
    {
        n=2;
        for(int i=1;i<=n;i++)
        {
            rx1[i]=cases[t][i-1][0],ry1[i]=cases[t][i-1][1];
            rx2[i]=cases[t][i-1][2],ry2[i]=cases[t][i-1][3];
        }
        if(union_area()!=brute_area(n,20)||union_perimeter()!=brute_peri(n,20))ok=false;
    }

    // 3. 随机对拍：坐标 0..9 的小矩形，面积/周长都跟暴力比
    for(int T=1;T<=1000&&ok;T++)
    {
        n=rnd(1,6);
        for(int i=1;i<=n;i++)
        {
            int a=rnd(0,8),b=rnd(0,8),c=rnd(0,8),d=rnd(0,8);
            rx1[i]=min(a,c),rx2[i]=max(a,c);
            ry1[i]=min(b,d),ry2[i]=max(b,d);
            if(rx1[i]==rx2[i])rx2[i]++;
            if(ry1[i]==ry2[i])ry2[i]++;
        }
        ll wa=brute_area(n,20),wp=brute_peri(n,20);
        ll ga=union_area(),gp=union_perimeter();
        if(ga!=wa||gp!=wp)
        {
            printf("第 %d 轮错: 面积 got=%lld want=%lld, 周长 got=%lld want=%lld\n",T,ga,wa,gp,wp);
            for(int i=1;i<=n;i++)printf("  矩形[%d,%d]x[%d,%d]\n",rx1[i],rx2[i],ry1[i],ry2[i]);
            ok=false;
        }
    }
    printf("面积/周长并 随机对拍 %s\n",ok?"passed":"FAILED");

    // 4. 规模测试：1e5 个随机矩形
    {
        n=100000;
        for(int i=1;i<=n;i++)
        {
            int x=rnd(0,1000000000),y=rnd(0,1000000000);
            int w=rnd(1,1000),h=rnd(1,1000);
            rx1[i]=x,ry1[i]=y,rx2[i]=x+w,ry2[i]=y+h;
        }
        printf("规模: 100000 个矩形 -> 面积=%lld 周长=%lld\n",union_area(),union_perimeter());
    }

    printf("结果: %s\n",ok?"OK":"FAILED");
    return 0;
}
