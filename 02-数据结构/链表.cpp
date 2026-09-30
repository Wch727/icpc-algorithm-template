// 数组模拟双向链表 / 静态链表
// 用 pre/nxt 两个数组当指针，插入删除 O(1)，按下标随机访问 O(1)
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;

// 数组模拟双向链表：h 是头结点编号，t 是尾结点编号，0 表示空
// 结点编号就是 val（值为 i 的结点编号 i），删除时按编号删
struct Link{
    int val[N],pre[N],nxt[N];
    bool used[N];        // 结点是否还在链表里
    int h,t;
    void init()
    {
        for(int i=1;i<N;i++)used[i]=false;
        h=0,t=0;
    }
    // 空表时单独处理，避免维护哨兵
    void push_back(int id,int v)
    {
        val[id]=v,used[id]=true;
        pre[id]=t,nxt[id]=0;
        if(t)nxt[t]=id;
        else h=id;
        t=id;
    }
    int insert_right(int p,int id,int v)   // 在 p 右边插入，O(1)
    {
        val[id]=v,used[id]=true;
        nxt[id]=nxt[p],pre[id]=p;
        if(nxt[p])pre[nxt[p]]=id;
        nxt[p]=id;
        if(t==p)t=id;
        return id;
    }
    int insert_left(int p,int id,int v)    // 在 p 左边插入，O(1)
    {
        val[id]=v,used[id]=true;
        pre[id]=pre[p],nxt[id]=p;
        if(pre[p])nxt[pre[p]]=id;
        pre[p]=id;
        if(h==p)h=id;
        return id;
    }
    void erase(int id)                     // O(1)，已删过就跳过（P1160 会重复删）
    {
        if(!used[id])return;
        used[id]=false;
        if(pre[id])nxt[pre[id]]=nxt[id];
        else h=nxt[id];
        if(nxt[id])pre[nxt[id]]=pre[id];
        else t=pre[id];
    }
    bool empty(){return h==0;}
    int size()
    {
        int c=0;
        for(int p=h;p;p=nxt[p])c++;
        return c;
    }
    void print()                           // 从头到尾
    {
        for(int p=h;p;p=nxt[p])printf("%d ",val[p]);
        printf("\n");
    }
    void print_rev()                       // 从尾到头，顺便验证 pre 指针
    {
        for(int p=t;p;p=pre[p])printf("%d ",val[p]);
        printf("\n");
    }
};

Link L;

int main()
{
    // 1. 手测：依次尾插 1 2 3 4，删 2，在 4 左边插 9（结点编号 101）
    L.init();
    for(int i=1;i<=4;i++)L.push_back(i,i);
    printf("build : "),L.print();
    L.erase(2);
    printf("erase2: "),L.print();
    L.insert_left(4,101,9);
    printf("ins   : "),L.print();
    printf("rev   : "),L.print_rev();
    printf("size=%d empty=%d\n",L.size(),(int)L.empty());
    L.erase(2);                            // 重复删除应该没影响
    printf("erase2 again: size=%d\n",L.size());
    L.erase(1),L.erase(101),L.erase(3),L.erase(4);   // 删空
    printf("erase all: size=%d empty=%d\n",L.size(),(int)L.empty());
    L.push_back(7,7);
    printf("push after empty: "),L.print();

    // 2. 手测 P1160 风格：第 i 个人插到 k 的左边(p=0)或右边(p=1)
    L.init();
    L.push_back(1,1);
    int ins[][3]={{2,1,0},{3,2,1},{4,2,0},{5,3,1}};    // {编号, k, p}
    for(int i=0;i<4;i++)
    {
        int id=ins[i][0],k=ins[i][1],p=ins[i][2];
        if(p==0)L.insert_left(k,id,id);
        else L.insert_right(k,id,id);
    }
    printf("P1160: "),L.print();
    L.erase(3);
    printf("del 3: "),L.print();
    printf("rev  : "),L.print_rev();
    printf("check: %d\n",(int)L.size());

    // 3. 对拍：随机插入/删除，与 vector 暴力比较（存结点编号）
    srand(20240513);
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        L.init();
        vector<int> bf;                    // 暴力：按链表顺序存结点编号
        n=500;
        int cnt=0;
        for(int i=1;i<=n;i++)
        {
            if(!bf.empty()&&rand()%3==0)   // 1/3 概率删一个已存在的结点
            {
                int pos=rand()%bf.size();
                int id=bf[pos];
                bf.erase(bf.begin()+pos);
                L.erase(id);
            }
            else                           // 插到随机位置
            {
                cnt++;
                int v=rand()%1000;
                if(bf.empty())L.push_back(cnt,v),bf.push_back(cnt);
                else
                {
                    int pos=rand()%bf.size();
                    int id=bf[pos];
                    if(rand()&1)
                        L.insert_left(id,cnt,v),bf.insert(bf.begin()+pos,cnt);
                    else
                        L.insert_right(id,cnt,v),bf.insert(bf.begin()+pos+1,cnt);
                }
            }
            vector<int> got;               // 每次抽查一遍链表遍历结果
            for(int p=L.h;p;p=L.nxt[p])got.push_back(p);
            if(got!=bf){ok=false;break;}
            if((int)bf.size()!=L.size()){ok=false;break;}
        }
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 4. 循环链表（约瑟夫环 P1996 的骨架）：n 个人围成一圈，每数到 m 出圈
    // 环形没有头尾，用 cur（当前人）和 last（它的前驱）两个指针就够了
    int nn=7,mm=3;
    for(int i=1;i<=nn;i++)L.val[i]=i,L.nxt[i]=i%nn+1;   // i 的后继是 i+1，最后一个指向 1
    int cur=1,last=nn,remain=nn;
    printf("joseph: ");
    while(remain>0)
    {
        for(int i=1;i<mm;i++)last=cur,cur=L.nxt[cur];    // 数 m 个数，last 始终是 cur 的前驱
        printf("%d ",L.val[cur]);
        L.nxt[last]=L.nxt[cur];                          // 跨过 cur，把它摘出去
        cur=L.nxt[cur];
        remain--;
        if(remain==0)break;
    }
    printf("\n");
    return 0;
}
