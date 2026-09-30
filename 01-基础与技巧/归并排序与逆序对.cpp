#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 归并排序 + 归并求逆序对，O(n log n)
int n;
int a[N],tmp[N];
ll inv;// 逆序对数会超过 int，必须 long long

void merge_sort(int l,int r)// 稳定排序，值相同的相对顺序不变
{
    if(l>=r)return;
    int m=(l+r)>>1;
    merge_sort(l,m);
    merge_sort(m+1,r);
    int i=l,j=m+1,cnt=l;
    while(i<=m&&j<=r)
    {
        if(a[i]<=a[j])tmp[cnt++]=a[i++];// 取等号，相等不算逆序
        else
        {
            inv+=m-i+1;// a[i..m] 都比 a[j] 大
            tmp[cnt++]=a[j++];
        }
    }
    while(i<=m)tmp[cnt++]=a[i++];
    while(j<=r)tmp[cnt++]=a[j++];
    for(int i=l;i<=r;i++)a[i]=tmp[i];
}

ll count_inv(int l,int r)// 只要逆序对数，返回答案
{
    if(l>=r)return 0;
    int m=(l+r)>>1;
    ll res=count_inv(l,m)+count_inv(m+1,r);
    int i=l,j=m+1,cnt=l;
    while(i<=m&&j<=r)
    {
        if(a[i]<=a[j])tmp[cnt++]=a[i++];
        else res+=m-i+1,tmp[cnt++]=a[j++];
    }
    while(i<=m)tmp[cnt++]=a[i++];
    while(j<=r)tmp[cnt++]=a[j++];
    for(int i=l;i<=r;i++)a[i]=tmp[i];
    return res;
}

int main()
{
    srand(20240521);
    // 自测1：排序与 sort 对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%60+1;
        for(int i=1;i<=n;i++)a[i]=rand()%7;
        vector<int> b(n+1);// vector 走堆，避免自测里大数组爆栈
        for(int i=1;i<=n;i++)b[i]=a[i];
        sort(b.begin()+1,b.end());
        inv=0;
        merge_sort(1,n);
        for(int i=1;i<=n;i++)
            if(a[i]!=b[i])
            {
                printf("fail merge_sort t=%d i=%d\n",t,i);
                return 0;
            }
    }
    printf("merge_sort self-check OK\n");

    // 自测2：逆序对与 O(n^2) 暴力对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%50+1;
        for(int i=1;i<=n;i++)a[i]=rand()%5;// 有重复，验证相等不计入
        ll bs=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(a[i]>a[j])bs++;
        inv=0;
        merge_sort(1,n);
        if(inv!=bs)
        {
            printf("fail inv t=%d got=%lld want=%lld\n",t,inv,bs);
            return 0;
        }
        for(int i=1;i<=n;i++)a[i]=(int)(rand()%21-10);
        bs=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(a[i]>a[j])bs++;
        ll got=count_inv(1,n);
        if(got!=bs)
        {
            printf("fail count_inv t=%d got=%lld want=%lld\n",t,got,bs);
            return 0;
        }
    }
    printf("inversion self-check OK\n");

    // 自测3：洛谷 P1908 样例
    n=6;
    int v[7]={0,5,4,2,6,3,1};
    for(int i=1;i<=n;i++)a[i]=v[i];
    inv=0;
    merge_sort(1,n);
    printf("P1908 inv=%lld (want 11)\n",inv);
    for(int i=1;i<=n;i++)printf("%d ",a[i]);
    printf("\n");

    // 自测4：n=1e6 速度
    n=1000000;
    for(int i=1;i<=n;i++)a[i]=rand();
    inv=0;
    clock_t st=clock();
    merge_sort(1,n);
    printf("n=%d inv=%lld time=%.3fs\n",n,inv,(double)(clock()-st)/CLOCKS_PER_SEC);
    return 0;
}
