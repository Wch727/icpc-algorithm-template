// 反悔贪心 的测试与对拍代码
// 模板本体：01-基础与技巧/反悔贪心.cpp
#include "../../01-基础与技巧/反悔贪心.cpp"

// 暴力：枚举所有子集（n<=15），和堆贪心对拍

ll brute_subset(const vector<Item>& it,ll lim)
{
    ll best=0;
    for(int mask=0;mask<(1<<(int)it.size());mask++)
    {
        ll val=0,cost=0;
        for(int i=0;i<(int)it.size();i++)
            if(mask>>i&1)val+=it[i].val,cost+=it[i].cost;
        if(cost<=lim)best=max(best,val);
    }
    return best;
}

int main()
{
    srand(20240603);
    // 自测1：工作调度 与 暴力（按截止时间排完，枚举 2^n 个子集判可行性）对拍
    for(int t=1;t<=2000;t++)
    {
        m=rand()%10+1;
        vector<Job> v;
        for(int i=0;i<m;i++)v.push_back({(int)(rand()%m+1),(int)(rand()%50+1)});
        ll got=solve_job_schedule(v);
        ll want=0;
        for(int mask=0;mask<(1<<m);mask++)
        {
            ll val=0;
            int ok=1;
            vector<int> dl;
            for(int i=0;i<m;i++)
                if(mask>>i&1)val+=v[i].p,dl.push_back(v[i].d);
            sort(dl.begin(),dl.end());
            for(int i=0;i<(int)dl.size();i++)
                if(dl[i]<i+1)ok=0;// 第 i+1 件工作最早也得在第 i+1 天做完
            if(ok)want=max(want,val);
        }
        if(got!=want)
        {
            printf("fail job schedule t=%d got=%lld want=%lld\n",t,got,want);
            return 0;
        }
    }
    printf("job schedule greedy self-check OK\n");

    // 自测2：不限次数的最优解 == k 给足时的 DP（两条独立路线必须一致）
    for(int t=1;t<=3000;t++)
    {
        m=rand()%14+1;
        vector<ll> p;
        for(int i=0;i<m;i++)p.push_back(rand()%21);
        ll f1=stock_unlimited_best(p),f3=dp_stock_k(p,m);
        if(f1!=f3)
        {
            printf("fail stock unlimited t=%d best=%lld dp=%lld p=",t,f1,f3);
            for(ll x:p)printf("%lld ",x);
            printf("\n");
            return 0;
        }
    }
    printf("stock unlimited self-check OK\n");

    // 自测3：错题本——堆配对贪心必须 >= 正解，而且要能举出严格大于的例子
    // 它比正解大正好说明「同一买点被复用」，这就是这题不能用配对的证据
    {
        int found=0;
        for(int t=1;t<=2000;t++)
        {
            m=rand()%10+1;
            vector<ll> p;
            for(int i=0;i<m;i++)p.push_back(rand()%21);
            if(solve_stock_wrong(p)>stock_unlimited_best(p))
            {
                found=1;
                break;
            }
        }
        if(!found)
        {
            printf("fail wrong-version demo did not overshoot\n");
            return 0;
        }
        vector<ll> p={1,2,3};
        printf("错题本 [1,2,3]: 堆配对=%lld 正解=%lld\n",solve_stock_wrong(p),stock_unlimited_best(p));
    }
    printf("stock wrong-version self-check OK\n");

    // 自测4：只有 1 次交易时，只有 DP 对；堆配对算出来的数会偏大
    {
        vector<ll> p={1,2,3};
        ll wrong_ans=solve_stock_wrong(p),dp1=dp_stock_k(p,1),dp3=dp_stock_k(p,3);
        printf("stock [1,2,3]: 堆配对=%lld dp(k=1)=%lld dp(k=3)=%lld\n",wrong_ans,dp1,dp3);
        if(wrong_ans!=3||dp1!=2||dp3!=2)
        {
            printf("fail stock k comparison\n");
            return 0;
        }
    }

    // 自测5：k 次限制的 DP 与暴力枚举不相交区间对拍（n 很小，直接四重循环）
    for(int t=1;t<=500;t++)
    {
        m=rand()%10+1;
        vector<ll> p;
        for(int i=0;i<m;i++)p.push_back(rand()%21);
        int kk=rand()%3+1;
        ll got=dp_stock_k(p,kk);
        // 暴力：枚举所有不相交的至多 kk 个区间
        ll best=0;
        if(kk==1)
        {
            for(int i=0;i<m;i++)
                for(int j=i+1;j<m;j++)best=max(best,p[j]-p[i]);
        }
        else
        {
            for(int i=0;i<m;i++)
                for(int j=i+1;j<m;j++)
                    for(int x=j+1;x<m;x++)// 第二段从 j+1 天之后开买
                        for(int y=x+1;y<m;y++)best=max(best,p[j]-p[i]+p[y]-p[x]);
        }
        if(got<best)
        {
            printf("fail dp_stock_k t=%d got=%lld want=%lld\n",t,got,best);
            return 0;
        }
    }
    printf("dp_stock_k self-check OK\n");

    // 自测6：选物品的启发式 与 2^n 精确解对拍
    // 只要求「不超过精确解」+「至少几次能撞上精确解」，不要求次次最优（0/1 背包是 NP-hard）
    {
        int hit=0,total=0;
        for(int t=1;t<=1500;t++)
        {
            m=rand()%12+1;
            vector<Item> it;
            for(int i=0;i<m;i++)it.push_back({(ll)(rand()%30+1),(ll)(rand()%20+1)});
            ll lim=rand()%40+1;
            ll got=solve_pick_greedy(it,lim);
            ll want=brute_subset(it,lim);
            total++;
            if(got>want)
            {
                printf("fail pick greedy t=%d got=%lld want=%lld lim=%lld\n",t,got,want,lim);
                return 0;
            }
            if(got==want)hit++;
        }
        printf("pick greedy self-check OK: %d/%d 次撞上精确解\n",hit,total);
    }

    // 自测7：套题演示——P2949 风格的小数据（截止时间 + 收益）
    {
        vector<Job> v={{1,5},{2,10},{1,7},{3,20},{2,8}};
        // 手工最优：第1天做 7，第2天做 10，第3天做 20，合计 38
        // （收益 5 和 8 那两件只能丢一件，因为截止时间都卡在第 1、2 天）
        printf("P2949 风格 ans=%lld (want 38)\n",solve_job_schedule(v));
    }

    // 自测8：买股票样例——价格 [7,1,5,3,6,4]
    {
        vector<ll> p={7,1,5,3,6,4};
        printf("stock 不限次数 ans=%lld (want 7: 1买5卖 + 3买6卖)\n",stock_unlimited_best(p));
        printf("stock 堆配对(错解) ans=%lld (会偏大)\n",solve_stock_wrong(p));
        printf("stock k=1 (DP) ans=%lld (want 5: 1买6卖)\n",dp_stock_k(p,1));
        printf("stock k=2 (DP) ans=%lld (want 7)\n",dp_stock_k(p,2));
    }

    // 自测9：n=2e5 规模下堆贪心的耗时
    {
        m=200000;
        vector<Job> v;
        for(int i=0;i<m;i++)v.push_back({(int)(rand()%m+1),(int)(rand()%1000000+1)});
        clock_t st=clock();
        ll ans=solve_job_schedule(v);
        printf("n=%d job schedule ans=%lld time=%.3fs\n",m,ans,(double)(clock()-st)/CLOCKS_PER_SEC);
    }
    return 0;
}
