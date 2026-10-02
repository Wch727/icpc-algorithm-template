#include "../../06-动态规划/SOS子集与超集和.cpp"
int main(){mt19937 r(49);for(int t=0;t<100;t++){int n=1<<(r()%8);ll p=1+r()%1000;vector<ll>a(n),b(n),want(n);for(ll &x:a)x=int(r()%1000)-500;for(ll &x:b)x=int(r()%1000)-500;for(int s=0;s<n;s++){for(int z=s;;z=(z-1)&s){want[s]=(want[s]+(__int128)((a[z]%p+p)%p)*((b[s^z]%p+p)%p))%p;if(!z)break;}}assert(subset_convolution(a,b,p)==want);}}
