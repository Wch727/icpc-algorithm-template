// 插头DP(轮廓线) 的测试与对拍代码
// 模板本体：06-动态规划/插头DP(轮廓线).cpp
#include "../../06-动态规划/插头DP(轮廓线).cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

ll brute_domino(int n,int m)// 暴力铺砖：从左往右第一个空格子枚举两种放法
{
    int tot=n*m;
    vector<int> used(tot,0);
    function<ll(int)> dfs=[&](int pos)->ll
    {
        while(pos<tot&&used[pos])pos++;
        if(pos==tot)return 1;
        int r=pos/m,c=pos%m;
        ll ans=0;
        if(c+1<m&&!used[pos+1])
        {
            used[pos]=used[pos+1]=1;
            ans+=dfs(pos+1);
            used[pos]=used[pos+1]=0;
        }
        if(r+1<n&&!used[pos+m])
        {
            used[pos]=used[pos+m]=1;
            ans+=dfs(pos+1);
            used[pos]=used[pos+m]=0;
        }
        return ans;
    };
    return dfs(0);
}

// 暴力：从 (0,0) 出发枚举哈密顿回路，条数除以 2（正反两个方向各算一次）
ll hc_cnt;
int tot_cell;
void dfs_hc(int x,int y,int step)
{
    if(step==tot_cell)
    {
        if((x==1&&y==0)||(x==0&&y==1))hc_cnt++;// 能一步回到起点就成环
        return;
    }
    int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
    for(int d=0;d<4;d++)
    {
        int nx=x+dx[d],ny=y+dy[d];
        if(nx<0||nx>=n||ny<0||ny>=m)continue;
        if(vis[nx][ny])continue;
        vis[nx][ny]=1;
        dfs_hc(nx,ny,step+1);
        vis[nx][ny]=0;
    }
}

ll brute_plug(int nn,int mm)
{
    n=nn,m=mm,tot_cell=n*m,hc_cnt=0;
    memset(vis,0,sizeof(vis));
    vis[0][0]=1;
    dfs_hc(0,0,1);
    return hc_cnt/2;
}

int main()
{
    srand(20240622);
    printf("==== 固定样例 ====\n");
    printf("哈密顿回路 2x2 : %lld (期望 1)\n",plug_dp(2,2));
    // 原期望写错：2x3 有 6 格且有 1 个回路，3x4 的暴力回路数为 2
    printf("哈密顿回路 2x3 : %lld (期望 1)\n",plug_dp(2,3));
    printf("哈密顿回路 2x4 : %lld (期望 1)\n",plug_dp(2,4));
    printf("哈密顿回路 3x4 : %lld (期望 2)\n",plug_dp(3,4));
    printf("哈密顿回路 4x4 : %lld (期望 6)\n",plug_dp(4,4));
    printf("铺砖方案 2x3 : %lld (期望 3)\n",domino_tiling(2,3));
    printf("铺砖方案 4x4 : %lld (期望 36)\n",domino_tiling(4,4));

    printf("==== 与暴力对拍 ====\n");
    int bad=0;
    int cs[9][2]={{2,2},{2,3},{2,4},{3,4},{4,4},{2,6},{3,6},{1,4},{3,3}};
    for(int c=0;c<9;c++)
    {
        int a=cs[c][0],b=cs[c][1];
        ll cur=plug_dp(a,b),ref=brute_plug(a,b);
        printf("哈密顿回路 %dx%d : 插头dp %lld 暴力 %lld %s\n",a,b,cur,ref,cur==ref?"OK":"FAILED");
        if(cur!=ref)bad=1;
    }
    for(int a=1;a<=5;a++)
        for(int b=1;b<=5;b++)
            if(a*b<=25)
            {
                ll cur=domino_tiling(a,b),ref=brute_domino(a,b);
                if(cur!=ref){printf("FAILED! 铺砖 %dx%d : 插头dp %lld 暴力 %lld\n",a,b,cur,ref);bad=1;}
            }
    if(!bad)printf("stress OK (9 组哈密顿回路 + 铺砖 1x1~5x5 全部与暴力一致)\n");
    return 0;
}

/*
轮廓线 dp 说明：
- 轮廓线是"处理过的格子"和"没处理的格子"的分界线，长度 m+1
- 逐格转移：第 j 个格子的左插头在位置 j，上插头在位置 j+1
- 每个插头 2 bit：0 无、1 左括号、2 右括号；平面性保证轮廓线上的插头是匹配括号序列
- 四个分支：0 个插头（留空或造新块）/ 1 个插头（右延或下延）/ 2 个插头（三种括号组合）
- 回路闭合只允许发生在最后一格且轮廓线干净，否则会提前形成小环
- 复杂度 O(n*m*状态数)，n,m<=12 可用
*/
