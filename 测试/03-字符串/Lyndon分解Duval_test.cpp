#include "../../03-字符串/Lyndon分解Duval.cpp"
int main(){mt19937 g(79);for(int t=0;t<500;t++){string s;for(int n=g()%70;n--;)s+=char('a'+g()%4);auto v=lyndon_factorization(s);string prev;int end=0;for(auto [l,r]:v){assert(l==end);end=r;string x=s.substr(l,r-l);if(!prev.empty())assert(prev>=x);for(int k=1;k<(int)x.size();k++)assert(x<x.substr(k)+x.substr(0,k));prev=x;}assert(end==(int)s.size());}}
