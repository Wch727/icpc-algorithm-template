#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 单调栈：每个元素最多入栈出栈一次，O(n)
int n;
int a[N];
int nge[N];// 右边第一个 >a[i] 的下标，没有记 0
int nge2[N];// 右边第一个 >=a[i] 的下标，没有记 0
int nse[N];// 右边第一个 <a[i] 的下标，没有记 0
int nle[N];// 左边第一个 <=a[i] 的下标，没有记 0
int st[N],top;// 手写数组栈，比 stack<int> 快
int bf1[N],bf2[N],bf3[N],bf4[N];// 暴力对拍用

void get_nge()// 下一个更大元素：栈内下标对应值单调递减
{
    top=0;
    memset(nge,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]<a[i])nge[st[top]]=i,top--;
        st[++top]=i;
    }
}

void get_nge2()// 下一个大于等于：弹栈条件改成 <=，保证相等不互相吃掉
{
    top=0;
    memset(nge2,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]<=a[i])nge2[st[top]]=i,top--;
        st[++top]=i;
    }
}

void get_nse()// 下一个更小元素
{
    top=0;
    memset(nse,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]>a[i])nse[st[top]]=i,top--;
        st[++top]=i;
    }
}

void get_nle()// 从左往右扫，得到左边第一个 <=a[i]
{
    top=0;
    memset(nle,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]>a[i])top--;// 弹掉左边比它大的，栈里全 <=a[i]
        if(top>0)nle[i]=st[top];// 栈顶就是离它最近的那个
        st[++top]=i;
    }
}

ll count_see()// 洛谷 P2866：每头牛向右能看到的牛数之和
{
    // 反过来数：连续一段严格矮于它的才会被它挡住视线
    // 维护一个「严格递减」的栈（存身高）：把 <=a[i] 的全弹掉，剩下的都能看见第 i 头牛
    top=0;
    ll ans=0;
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]<=a[i])top--;// 矮的或一样高的都出栈
        ans+=top;
        st[++top]=i;
    }
    return ans;
}
