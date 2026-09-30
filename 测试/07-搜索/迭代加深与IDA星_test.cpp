// 迭代加深与IDA星 的测试与对拍代码
// 模板本体：07-搜索/迭代加深与IDA星.cpp
#include "../../07-搜索/迭代加深与IDA星.cpp"

// ========== 四、自测 ==========

void test_iddfs()
{
    id_n=3;
    id_a[1]=1,id_a[2]=3,id_a[3]=7;
    printf("[iddfs] {1,3,7} 凑 10 -> %d 个 (期望 2: 3+7)\n",solve_iddfs(10));
    printf("[iddfs] {1,3,7} 凑 6 -> %d 个 (期望 3: 7-3+1 或 3+3 不允许)\n",solve_iddfs(6));
    printf("[iddfs] {1,3,7} 凑 100 -> %d (无解期望 -1)\n",solve_iddfs(100));
}

void test_knight()
{
    mt19937 rnd(20240607);
    int ok=1,fail=0;
    long long tot=0;
    int depth_sum=0;
    // 从目标局面倒着走 1~3 步造出起点，起点到目标的真实步数一定不超过走的步数
    // 用 IDA* 求解，检查它给出的步数 <= 步数上限（能对上真解），并统计搜索规模
    for(int t=1;t<=10;t++)
    {
        ks_rand_start(rnd);
        memcpy(ks_goal,ks,sizeof(ks));
        int steps=rnd()%3+1;
        int done=0;
        for(int i=0;i<steps;i++)
        {
            int x=ks_sx,y=ks_sy;
            for(int att=0;att<50;att++)
            {
                int i2=rnd()%8+1;
                int xx=x+kdx[i2],yy=y+kdy[i2];
                if(xx<0||xx>=KB||yy<0||yy>=KB)continue;
                swap(ks[x][y],ks[xx][yy]),ks_sx=xx,ks_sy=yy;
                done++;
                break;
            }
        }
        ks_nodes=0;
        int got=ks_solve(steps+1);//深度上限放宽 1，够它找到更短的路
        tot+=ks_nodes;
        int good=(got>=0&&got<=done&&got>0);
        depth_sum+=got;
        printf("  第 %d 组: 随机走了 %d 步, IDA* 得 %d 步, 访问 %lld 个结点\n",t,done,got,ks_nodes);
        if(!good)
        {
            ok=0,fail++;
            if(fail<=3)printf("  !! 第 %d 组失败: 走了 %d 步却解不出\n",t,done);
        }
    }
    printf("[knight] 10 组「随机走几步再倒推」的自洽检验 %s (共访问 %lld 结点, 总步数 %d)\n",
        ok?"全部通过":"失败",tot,depth_sum);
    // 手算例子（洛谷 P2324 风格的 5x5 局面）：目标局面自身应该是 0 步
    int goal[KB][KB]={
        { 1, 1, 1, 1, 1},
        { 0, 1, 1, 1, 1},
        { 0, 0,-1, 1, 1},
        { 0, 0, 0, 0, 1},
        { 0, 0, 0, 0, 0}};
    memcpy(ks_goal,goal,sizeof(goal));
    memcpy(ks,goal,sizeof(goal));
    ks_sx=2,ks_sy=2;
    printf("[knight] 目标局面自身 -> %d 步 (期望 0)\n",ks_solve(3));
}

void test_rings()
{
    mt19937 rnd(998244353);
    int ok=1,fail=0,tested=0;
    for(int t=1;t<=200&&tested<25;t++)
    {
        cr_n=rnd()%3+2;//2..4，n 太大 IDA* 会慢
        for(int i=1;i<=cr_n;i++)
        {
            cr_s[i]=rnd()&3;
            // t[i] 必须和 i 不同，否则「一次改善两个」的估价不成立
            int v=rnd()%(cr_n-1);
            if(v>=i)v++;
            cr_t[i]=v+1;
        }
        int ex=cr_brute();
        if(ex<0||ex>6)continue;//太深的局面 IDA* 会很慢，只对拍浅的
        int got=cr_solve(ex+1);
        tested++;
        if(got!=ex)
        {
            ok=0,fail++;
            if(fail<=3)printf("  !! 第 %d 组: IDA*=%d 暴力=%d\n",t,got,ex);
        }
    }
    printf("[rings] %d 组随机旋钮与暴力 BFS 对拍 %s\n",tested,ok?"全部通过":"失败");
    // 手算小例：n=2, t={2,1}（1 号旋钮联动 2 号），初始 {1,2}
    cr_n=2,cr_t[1]=2,cr_t[2]=1,cr_s[1]=1,cr_s[2]=2;
    printf("[rings] n=2 t={2,1} 初始{1,2} -> %d 步, 暴力=%d\n",cr_solve(12),cr_brute());
}

int main()
{
    test_iddfs();
    test_knight();
    test_rings();
    return 0;
}
