// 适用：有序数组两数和、满足条件的最短连续窗口；各指针只向一个方向走。
// vector/string 都为 0-indexed；指针单调保证总扫描线性，窗口计数允许为负表示多余。
#include<bits/stdc++.h>
using namespace std;

// O(n)，已排序数组中找两数之和，返回下标；无解返回 -1
// O(n)、额外空间 O(1)；a 必须非降序，target 为目标和，返回两个不同的 0-based 下标。
pair<int,int> two_sum(const vector<int> &a,int target)
{
    int l=0,r=(int)a.size()-1;
    while(l<r)
    {
        long long sum=(long long)a[l]+a[r];
        if(sum==target)return {l,r};
        if(sum<target)l++;
        else r--;
    }
    return {-1,-1};
}

// O(|s|+|t|)，最短覆盖子串，计入重复字符；返回长度，无解 -1
// O(|s|+|t|)、空间 O(256)；s 是文本、t 是需求多重集，窗口 [l,r] 两端均包含。
// unsigned char 避免高位字节作为负下标；先扩大至满足，再收缩直到缺少一个所需字符。
int min_cover(const string &s,const string &t)
{
    if(t.empty())return 0;
    int cnt[256]={0},need=t.size(),l=0,ans=INT_MAX;
    for(unsigned char c:t)cnt[c]++;
    for(int r=0;r<(int)s.size();r++)
    {
        unsigned char c=s[r];
        if(cnt[c]-->0)need--;
        while(need==0)
        {
            ans=min(ans,r-l+1);
            unsigned char d=s[l++];
            if(++cnt[d]>0)need++;
        }
    }
    return ans==INT_MAX?-1:ans;
}
