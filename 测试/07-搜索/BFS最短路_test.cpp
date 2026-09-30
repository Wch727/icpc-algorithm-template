// BFS最短路 的测试与对拍代码
// 模板本体：07-搜索/BFS最短路.cpp
#include "../../07-搜索/BFS最短路.cpp"

// ---------- 六、自测 ----------

void test_grid()
{
    n=5,m=5;
    memset(block,0,sizeof(block));
    bfs_grid(1,1);
    printf("[grid] 5x5 从 (1,1) 到 (5,5) 曼哈顿步数=%d (期望 8)\n",dis[5][5]);
    printf("[grid] (3,4)=%d (期望 5)\n",dis[3][4]);
    // 加一堵墙：这 5 格切断了所有 8 步的单调路径，只能绕远，多走 2 步
    block[2][2]=block[2][4]=block[3][1]=block[3][3]=block[4][5]=true;
    bfs_grid(1,1);
    printf("[grid] 加上 5 格墙后 (5,5)=%d (期望 10: 绕行多 2 步)\n",dis[5][5]);
    memset(block,0,sizeof(block));
}

void test_multi()
{
    n=5,m=5;
    vector<pair<int,int> > s;
    s.push_back(make_pair(1,1));
    s.push_back(make_pair(5,5));
    bfs_multi(s);
    // 马步图上没有阻挡时是切比雪夫距离 max(|dx|,|dy|)，只有一步可达的格子例外
    printf("[multi] 双源马步 BFS: (3,3)=%d (期望 4)  (1,5)=%d (期望 2)  (3,2)=%d (期望 1)\n",
        md[3][3],md[1][5],md[3][2]);
}

void test_flood()
{
    // 1 是墙，0 是空地；外面补一圈 0，把与外面连通的部分染成 2
    n=5,m=5;
    memset(g,0,sizeof(g));
    int wall[5][5]={
        {0,0,0,0,0},
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0},
        {0,0,0,0,0}};
    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)g[i+1][j+1]=wall[i][j];
    flood(0,0,2);
    printf("[flood] 封闭区域内的格子 (3,3)=%d (期望 0 未被染色)  外部空地 (1,2)=%d (期望 2)\n",g[3][3],g[1][2]);
}

void test_meteor()
{
    // 数据自造：一颗流星在 t=0 砸 (0,1)，则 (0,0)(0,1)(1,1) 都在 t=1 被砸毁
    // 起点 (0,0) 在 t=0 安全，往下走到 (1,0) 在 t=1 到场，而 (1,0) 永不被砸 -> 答案 1
    memset(tim,0x3f,sizeof(tim));
    int x=0,y=1,t0=0;//被砸毁的时刻 = 到达时刻 t0+1
    tim[x][y]=min(tim[x][y],t0+1);
    for(int i=1;i<=4;i++)
    {
        int xx=x+dx4[i],yy=y+dy4[i];
        if(xx<0||yy<0)continue;
        tim[xx][yy]=min(tim[xx][yy],t0+1);
    }
    printf("[meteor] t=0 砸 (0,1) 时最早安全时刻=%d (期望 1: 走到 (1,0))\n",bfs_meteor());
    // 起点立刻就被砸：时刻 0 已经在 (0,0) 上，tim[0][0]=0 表示不能站
    memset(tim,0x3f,sizeof(tim));
    tim[0][0]=tim[0][1]=tim[1][0]=0;
    printf("[meteor] 起点 t=0 就被砸 -> %d (期望 -1)\n",bfs_meteor());
    // 永不落流星，原地就是安全点
    memset(tim,0x3f,sizeof(tim));
    printf("[meteor] 完全不落流星 -> %d (期望 0)\n",bfs_meteor());
}

void test_puzzle()
{
    // 一步就能到目标：空格把 8 换过来
    int st[3][3]={{1,2,3},{4,5,6},{7,0,8}};
    printf("[puzzle] 123456708 -> %d 步 (期望 1)\n",bfs_puzzle(st));
    int st2[3][3]={{1,2,3},{4,0,6},{7,5,8}};
    printf("[puzzle] 123406758 -> %d 步 (期望 2)\n",bfs_puzzle(st2));
    int st3[3][3]={{1,2,3},{4,5,6},{7,8,0}};
    printf("[puzzle] 目标态自身 -> %d 步 (期望 0)\n",bfs_puzzle(st3));
    int st4[3][3]={{1,2,3},{4,5,6},{8,7,0}};
    printf("[puzzle] 只交换最后两个 -> %d 步 (期望 -1，奇偶性无解)\n",bfs_puzzle(st4));
}

int main()
{
    test_grid();
    test_multi();
    test_flood();
    test_meteor();
    test_puzzle();
    return 0;
}
