#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 排序去重 O(n log n)，映射 O(log n)；保留大小关系，不保留实际距离。
const int N=1000005;
int n,id[N];
ll a[N];
vector<ll> b;

void discrete()
{
    b.clear();
    for(int i=1;i<=n;i++)b.push_back(a[i]);
    sort(b.begin(),b.end());
    b.erase(unique(b.begin(),b.end()),b.end());
    for(int i=1;i<=n;i++)
        id[i]=lower_bound(b.begin(),b.end(),a[i])-b.begin()+1;
}
// id[i] 是 1-based 排名，原数组 a 不变；排名 r 的原值为 b[r-1]。
