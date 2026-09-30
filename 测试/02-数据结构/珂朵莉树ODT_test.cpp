// 珂朵莉树ODT 的测试与对拍代码
// 模板本体：02-数据结构/珂朵莉树ODT.cpp
#include "../../02-数据结构/珂朵莉树ODT.cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

ll a[105],br[105];

// 自测：赋值/加/和/计数/第k小全部与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=30;t++)
    {
        n=rnd(1,40);
        odt.clear();
        for(int i=1;i<=n;i++)a[i]=rnd(1,5),br[i]=a[i],odt.insert(Node(i,i,a[i]));
        for(int q=1;q<=200;q++)
        {
            int op=rnd(1,5),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                ll v=rnd(1,5);
                assign(l,r,v);
                for(int i=l;i<=r;i++)br[i]=v;
            }
            else if(op==2)
            {
                ll v=rnd(-5,5);
                add(l,r,v);
                for(int i=l;i<=r;i++)br[i]+=v;
            }
            else if(op==3)
            {
                ll x=query_sum(l,r),z=0;
                for(int i=l;i<=r;i++)z+=br[i];
                cnt++;
                if(x!=z)bad++;
            }
            else if(op==4)
            {
                ll v=rnd(1,5);
                int x=query_cnt(l,r,v),z=0;
                for(int i=l;i<=r;i++)
                    if(br[i]==v)z++;
                cnt++;
                if(x!=z)bad++;
            }
            else
            {
                int k=rnd(1,r-l+1);
                vector<ll> tmp;
                for(int i=l;i<=r;i++)tmp.push_back(br[i]);
                sort(tmp.begin(),tmp.end());
                cnt++;
                if(query_kth(l,r,k)!=tmp[k-1])bad++;
            }
        }
    }
    printf("珂朵莉树ODT vs 暴力: %s, 校验=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：[1,5] 全赋 2，再给 [2,4] 加 10，求和/计数/第2小
    n=5;
    odt.clear();
    odt.insert(Node(1,n,1));
    assign(1,5,2);
    add(2,4,10);
    printf("小样例: sum[1,5]=%lld cnt(1,5,2)=%d kth(1,5,2)=%lld\n",query_sum(1,5),query_cnt(1,5,2),query_kth(1,5,2));
    return 0;
}
