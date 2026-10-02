#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N=1000005;
int n,a[N],st[N],top,ans[N];

// O(n)，右边第一个严格更大的下标，不存在为 0。
void right_greater()
{
    top=0;
    fill(ans+1,ans+n+1,0);
    for(int i=1;i<=n;i++)
    {
        while(top&&a[st[top]]<a[i])ans[st[top--]]=i;
        st[++top]=i;
    }
}

// O(n)，左边第一个严格更小的下标，不存在为 0。
void left_less()
{
    top=0;
    for(int i=1;i<=n;i++)
    {
        while(top&&a[st[top]]>=a[i])top--;
        ans[i]=top?st[top]:0;
        st[++top]=i;
    }
}
// 右侧写法：弹栈时给旧元素赋答案；左侧写法：弹栈后给当前元素赋答案。
// 修改比较方向可求较大/较小；是否包含等号由题意决定。

// 柱状图最大矩形：a[1..n] 为非负高度，柱宽为 1，O(n)。
// 弹出 j 时，右边界 i、左边界为弹出后的栈顶 L，可覆盖 [L+1,i-1]。
ll histogram()
{
    top=0;
    ll res=0;
    for(int i=1;i<=n+1;i++)
    {
        while(top&&(i==n+1||a[st[top]]>=a[i]))
        {
            int j=st[top--],L=top?st[top]:0;
            res=max(res,1LL*a[j]*(i-L-1));
        }
        if(i<=n)st[++top]=i;
    }
    return res;
}
// n+1 只负责清空栈，不访问 a[n+1]；等高柱合并后由更靠右的柱继续扩展。

// 所有非空子数组的最小值之和，O(n)，乘积/总和须在 ll 范围内。
// j 左侧第一个 < a[j] 的位置为 L，右侧第一个 <= a[j] 的位置为 R。
// 左端点有 j-L 种，右端点有 R-j 种，贡献 a[j]*(j-L)*(R-j)。
ll sum_min()
{
    top=0;
    ll res=0;
    for(int i=1;i<=n+1;i++)
    {
        while(top&&(i==n+1||a[st[top]]>=a[i]))
        {
            int j=st[top--],L=top?st[top]:0;
            res+=1LL*a[j]*(j-L)*(i-j);
        }
        if(i<=n)st[++top]=i;
    }
    return res;
}
// 一侧严格、一侧非严格，把相同最小值归给最右位置，避免重复或遗漏。
// 求最大值之和：>= 改为 <=；子数组极差之和 = 最大值之和 - 最小值之和。
