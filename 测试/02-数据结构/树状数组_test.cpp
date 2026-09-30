// 树状数组 的测试与对拍代码
// 模板本体：02-数据结构/树状数组.cpp
#include "../../02-数据结构/树状数组.cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：五种用法全部与暴力对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    // 1. 单点加 + 区间和
    BIT<ll,105> bit;
    ll b1[105];
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,60);
        bit.init(n);
        for(int i=1;i<=n;i++)b1[i]=0;
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,3),x=rnd(1,n),y=rnd(1,n);
            if(op<=2)
            {
                ll v=rnd(-20,20);
                bit.add(x,v),b1[x]+=v;
            }
            else
            {
                if(x>y)swap(x,y);
                ll s=bit.query(x,y),z=0;
                for(int i=x;i<=y;i++)z+=b1[i];
                cnt++;
                if(s!=z)bad++;
            }
        }
    }
    // 2. 差分：区间加 + 单点查
    DiffBIT<ll,105> db;
    ll b2[105];
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,60);
        db.init(n);
        for(int i=1;i<=n;i++)b2[i]=0;
        for(int q=1;q<=200;q++)
        {
            int x=rnd(1,n),y=rnd(1,n),op=rnd(1,2);
            if(x>y)swap(x,y);
            if(op==1)
            {
                ll v=rnd(-20,20);
                db.update(x,y,v);
                for(int i=x;i<=y;i++)b2[i]+=v;
            }
            else
            {
                cnt++;
                if(db.query(x)!=b2[x])bad++;
            }
        }
    }
    // 3. 求第 k 小(值域 1..n 的权值树)
    int cntb[105];
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,50);
        BIT<int,105> c;
        c.init(n);
        for(int i=1;i<=n;i++)cntb[i]=0;
        for(int q=1;q<=200;q++)
        {
            if(rnd(1,2)==1)
            {
                int x=rnd(1,n),v=rnd(1,3);
                c.add(x,v),cntb[x]+=v;
            }
            else
            {
                int tot=0;
                for(int i=1;i<=n;i++)tot+=cntb[i];
                int k=rnd(1,tot+1);//k=tot+1 时应该返回 n+1
                int expect=n+1,s=0;
                for(int i=1;i<=n;i++)
                {
                    s+=cntb[i];
                    if(s>=k){expect=i;break;}
                }
                cnt++;
                if(c.kth(k)!=expect)bad++;
            }
        }
    }
    // 4. 二维树状数组
    BIT2D<int,55> b2d;
    int br2[55][55];
    for(int t=1;t<=5;t++)
    {
        int nn=rnd(1,20),mm=rnd(1,20);
        b2d.init(nn,mm);
        for(int i=1;i<=nn;i++)
            for(int j=1;j<=mm;j++)br2[i][j]=0;
        for(int q=1;q<=200;q++)
        {
            if(rnd(1,2)==1)
            {
                int x=rnd(1,nn),y=rnd(1,mm),v=rnd(-9,9);
                b2d.add(x,y,v),br2[x][y]+=v;
            }
            else
            {
                int x1=rnd(1,nn),x2=rnd(1,nn),y1=rnd(1,mm),y2=rnd(1,mm);
                if(x1>x2)swap(x1,x2);
                if(y1>y2)swap(y1,y2);
                int s=b2d.query(x1,y1,x2,y2),z=0;
                for(int i=x1;i<=x2;i++)
                    for(int j=y1;j<=y2;j++)z+=br2[i][j];
                cnt++;
                if(s!=z)bad++;
            }
        }
    }
    // 5. 离散化逆序对
    for(int t=1;t<=10;t++)
    {
        int len=rnd(1,60);
        for(int i=1;i<=len;i++)a[i]=rnd(1,20);
        ll s=calc_inv(len),z=0;
        for(int i=1;i<=len;i++)
            for(int j=i+1;j<=len;j++)
                if(a[i]>a[j])z++;
        cnt++;
        if(s!=z)bad++;
    }
    printf("树状数组(单点改/差分/第k小/二维/逆序对) vs 暴力: %s, 校验=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[3,1,2] 逆序对为 2；权值树 {0,1,1,3} 第 2 小是 3
    a[1]=3,a[2]=1,a[3]=2;
    BIT<int,10> c;
    c.init(4);
    c.add(2,1),c.add(3,1),c.add(4,3);
    printf("小样例: 逆序对=%lld, 权值树第2小=%d, 前缀和[1,3]=%d\n",calc_inv(3),c.kth(2),c.query(1,3));
    return 0;
}
