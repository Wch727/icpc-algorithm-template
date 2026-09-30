#include<bits/stdc++.h>
using namespace std;

// O(n)，已排序数组中找两数之和，返回下标；无解返回 -1
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
