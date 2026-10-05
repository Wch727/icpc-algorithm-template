#include "../../02-数据结构/CDQ分治(三维偏序).cpp"
vector<ll> dominance(const vector<array<int,3>> &arg_a){a=arg_a;return dominance();}
int main(){mt19937 g(123);for(int z=0;z<1000;z++){int n=g()%70;vector<array<int,3>>a(n);for(auto &p:a)for(int &v:p)v=int(g()%11)-5;auto r=dominance(a);for(int i=0;i<n;i++){long long s=0;for(int j=0;j<n;j++)if(i!=j&&a[j][0]<=a[i][0]&&a[j][1]<=a[i][1]&&a[j][2]<=a[i][2])s++;assert(r[i]==s);}}cout<<"CDQ OK\n";}
