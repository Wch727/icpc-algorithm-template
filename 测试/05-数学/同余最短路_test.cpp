#include "../../05-数学/同余最短路.cpp"
int main(){mt19937 r(151);for(int t=0;t<200;t++){vector<long long>c(1+r()%8);for(auto &x:c)x=1+r()%40;auto d=residue_shortest_path(c);vector<bool>ok(2001);ok[0]=true;for(int i=1;i<=2000;i++)for(auto x:c)if(x<=i&&ok[i-x])ok[i]=true;for(int i=0;i<=2000;i++)assert((d[i%d.size()]<=i)==ok[i]);}}
