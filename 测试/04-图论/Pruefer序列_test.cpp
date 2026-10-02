#include "../../04-图论/Pruefer序列.cpp"
int main(){mt19937 r(149);for(int t=0;t<300;t++){int n=2+r()%100;vector<int>code(n-2);for(int &x:code)x=r()%n;auto g=prufer_decode(code);assert(prufer_encode(g)==code);vector<int>cnt(n,1);for(int x:code)cnt[x]++;for(int i=0;i<n;i++)assert((int)g[i].size()==cnt[i]);}}
