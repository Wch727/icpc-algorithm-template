// @code common
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// @code modular_add
inline int add_mod(int a,int b,int mod){ll c=1LL*a+b;return c>=mod?c-mod:c;}

// @code unrolled_sum
ll sum_unrolled(int *a,int n)
{
    ll s=0;
    int i=1;
    for(;i+3<=n;i+=4)
    {
        s+=a[i],s+=a[i+1],s+=a[i+2],s+=a[i+3];
    }
    for(;i<=n;i++)s+=a[i];
    return s;
}
