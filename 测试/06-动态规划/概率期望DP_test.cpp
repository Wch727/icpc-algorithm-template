// 概率期望DP 的测试与对拍代码
// 模板本体：06-动态规划/概率期望DP.cpp
#include "../../06-动态规划/概率期望DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

double rndreal()// 生成 [0,1) 的随机实数
{
    return (double)rand()/RAND_MAX;
}

int main()
{
    srand(20240701);
    printf("==== 固定样例 ====\n");
    // 原期望写错：0 处后退停留，独立概率推进得到 20、11.9753、40.9375
    printf("逆推 p=0.50 n=4 : %.4f (期望 20.0000 = n*(n+1))\n",exp_hit(4,0.5));
    printf("逆推 p=0.60 n=4 : %.4f (期望 11.9753)\n",exp_hit(4,0.6));
    printf("正推 p=0.60 n=4 : %.4f (与上一行一致)\n",exp_forward_walk(4,0.6));
    printf("逆推 p=1.00 n=4 : %.4f (期望 4.0000)\n",exp_hit(4,1.0));
    printf("逆推 p=0.40 n=4 : %.4f (期望 40.9375)\n",exp_hit(4,0.4));
    printf("正推 2x2 网格 a=0.4 b=0.3 : %.6f\n",exp_forward_grid(2,2,0.4,0.3));
    printf("抽卡集齐 4 种 : %.4f (期望 8.3333 = 4*H4)\n",coupon_expected(4));

    printf("==== 随机对拍 ====\n");
    int bad=0;
    for(int tt=1;tt<=30;tt++)
    {
        int nn=rndint(2,8);
        double pp=0.40+0.55*rndreal();// 覆盖后退概率更大的分支
        pp=llround(pp*100)/100.0;
        if(pp<0.40)pp=0.40;
        if(pp>0.95)pp=0.95;
        double cur=exp_hit(nn,pp),ref=exp_forward_walk(nn,pp);
        if(fabs(cur-ref)>1e-6)
        {
            bad++;
            printf("FAILED! 逆推 vs 正推 轮%d n=%d p=%.4f 逆推=%.6f 正推=%.6f\n",tt,nn,pp,cur,ref);
            break;
        }
    }
    for(int tt=1;tt<=20&&!bad;tt++)// 正推网格：小概率且状态少时结果应当在合理范围
    {
        int nn=rndint(1,3),mm=rndint(1,3);
        double a=0.2+0.3*rndreal(),b=0.2+0.3*rndreal();
        double cur=exp_forward_grid(nn,mm,a,b);
        if(!(cur>=0)||cur!=cur||cur>1e7){bad++;printf("FAILED! 正推网格 轮%d 结果异常 %.6f\n",tt,cur);break;}
    }
    if(!bad)printf("stress OK (30 组逆推 vs 时间推进正推 + 20 组正推网格 全部通过)\n");
    return 0;
}

/*
概率期望 dp 两种方向：
1. 逆推：E[终态]=0，E[i]=1+sum_j P(i->j)*E[j]，从后往前算。
   注意方程里如果 E[i] 自己也出现（原地不动/后退），要移项解方程，
   模板里 p=0.5 时 E[i]=n(n+1)-i(i+1) 可以直接用，p!=0.5 用高斯消元或三对角递推。
2. 正推：先按时间推概率分布 P_t[i]，再用 E[T]=sum_{t>=0} P(T>t) 求期望；
   适合状态数小、步数上限明确的题（比如本题网格只有 9 个状态）。
- 期望的线性性：E[X+Y]=E[X]+E[Y]，把总期望拆成每条边/每种卡片的贡献往往更好写。
*/
