// Duval：O(n) 把字符串拆成字典序非增的 Lyndon 串，返回 0-indexed 半开区间。
// Lyndon 串严格小于自身任何非平凡循环移位；用于周期/最小表示、Lyndon 相关分段。
// 按 unsigned char 比较，空串返回空；最小表示法已有模板，此处不重复。
// 应用：各前缀最小后缀、周期分段；前缀最小后缀需跟踪 Duval 扫描状态，不能只取分解首块。
#include<bits/stdc++.h>
using namespace std;
string s;
vector<pair<int,int>> lyndon_factorization()
{
    int n=s.size(),i=0;vector<pair<int,int>> ans;
    while(i<n){int j=i+1,k=i;while(j<n&&(unsigned char)s[k]<=(unsigned char)s[j]){if((unsigned char)s[k]<(unsigned char)s[j])k=i;else k++;j++;}int len=j-k;while(i<=k)ans.push_back({i,i+len}),i+=len;}
    return ans;
}
