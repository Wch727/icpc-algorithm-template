// 博弈论(Nim与SG) 的测试与对拍代码
// 模板本体：05-数学/博弈论(Nim与SG).cpp
#include "../../05-数学/博弈论(Nim与SG).cpp"

// 暴力博弈：每步从任意一堆取 1..k 个（k=0 表示不限制，即普通 Nim）
// 返回当前局面先手是否必胜；memo 用按 k 分开的记忆化
map<pair<int,vector<int>>,bool> memo;

bool brute(vector<int> v,int k)
{
    sort(v.begin(),v.end());
    auto key=make_pair(k,v);
    if(memo.count(key))return memo[key];
    bool win=false;
    for(int i=0;i<(int)v.size();i++)
        for(int take=1;take<=v[i];take++)
        {
            if(k&&take>k)break;// 限步游戏：一步最多取 k 个
            vector<int> u=v;
            u[i]-=take;
            sort(u.begin(),u.end());
            if(!brute(u,k))win=true;// 存在一步让对手必败
        }
    return memo[key]=win;
}

int main()
{
    int bad=0;
    // 1) Nim：小数据与暴力博弈对拍（Nim 每步可取任意多个，brute 也是）
    mt19937_64 rnd(20250606);
    for(int t=1;t<=500;t++)
    {
        int k=rnd()%3+1;
        vector<int> v;
        int arr[5];
        for(int i=1;i<=k;i++)
        {
            arr[i]=rnd()%5;
            v.push_back(arr[i]);
        }
        bool want=brute(v,0);// k=0 表示不限制，普通 Nim
        bool got=nim_win(k,arr);
        if(got!=want)bad++;
        // 必胜步验证：走完异或和必须为 0
        pair<int,int> mv=nim_first(k,arr);
        if(got)
        {
            if(mv.first==0)bad++;
            else
            {
                int b[5];
                for(int i=1;i<=k;i++)b[i]=arr[i];
                b[mv.first]=mv.second;
                if(nim_win(k,b))bad++;
            }
        }
    }
    // 2) SG：单堆取 1..k 个的 SG 值与暴力必胜判定一致
    for(int k=1;k<=5;k++)
    {
        sg_init();
        for(int x=0;x<=60;x++)
        {
            int g=get_sg(x,k);
            // 单堆限步局面：SG=0 必败，SG!=0 必胜
            vector<int> v;
            v.push_back(x);
            bool want=brute(v,k);
            if((g!=0)!=want)
            {
                bad++;
                if(bad<=8)printf("P2FAIL k=%d x=%d sg=%d want=%d\n",k,x,g,(int)want);
            }
        }
    }
    // 3) 多堆 SG 异或与暴力对拍
    for(int t=1;t<=300;t++)
    {
        int k=rnd()%3+1;
        vector<int> v;
        for(int i=1;i<=k;i++)
        {
            int p=rnd()%7;
            v.push_back(p);
        }
        sg_init();
        int s=0;
        for(int i=0;i<k;i++)s^=get_sg(v[i],3);// 每堆可取 1..3
        // 暴力：同一个限步游戏（每堆每次取 1..3），直接递归必胜判定
        bool want=brute(v,3);
        if((s!=0)!=want)
        {
            bad++;
            if(bad<=8)printf("P3FAIL k=%d s=%d want=%d\n",k,s,(int)want);
        }
    }
    int arr[4]={0,3,4,5};
    printf("nim(3,4,5) first player %s\n",nim_win(3,arr)?"wins":"loses");
    pair<int,int> mv=nim_first(3,arr);
    printf("winning move: pile %d -> %d\n",mv.first,mv.second);
    sg_init();
    printf("sg(0..8) with take 1..3 : ");
    for(int i=0;i<=8;i++)printf("%d ",get_sg(i,3));
    printf("\nbrute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P4702 取石子，n 堆总和为奇数 Alice 胜，偶数 Bob 胜
// 边界：全 0 局面异或和为 0，先手必败；nim_first 无必胜步返回 (0,0)

/*
自测记录：
  1) Nim 500 组（1..3 堆，每堆 <5）与记忆化暴力博弈对拍，并验证必胜步走完异或和为 0；
  2) 单堆限步 1..k（k=1..5，x<=60）的 SG 值与暴力必胜判定对拍；
  3) 多堆限步 1..3 的 SG 异或与暴力必胜判定对拍 300 组。
  注意：get_sg 里的 vis 必须每次调用独立，用 static 会被递归内层调用冲掉。
*/
