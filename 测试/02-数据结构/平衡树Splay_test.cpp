// 平衡树Splay 的测试与对拍代码
// 模板本体：02-数据结构/平衡树Splay.cpp
#include "../../02-数据结构/平衡树Splay.cpp"

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 全树体检：sz 与 num 之和一致、父子指针互指
int chk_sum,chk_bad,chk_nodes,chk_zero;
void check(int p)
{
    if(!p)return;
    chk_nodes++;
    if(p<1||p>sp.tot)chk_bad++;
    if(sp.num[p]==0)chk_zero++;
    if(sp.ch[p][0]&&sp.fa[sp.ch[p][0]]!=p)chk_bad++;
    if(sp.ch[p][1]&&sp.fa[sp.ch[p][1]]!=p)chk_bad++;
    chk_sum+=sp.num[p];
    check(sp.ch[p][0]),check(sp.ch[p][1]);
}

// 中序展开成「带重复的升序序列」，写进 out，返回长度
int expand(int *out)
{
    int len=0,top=0,p=sp.root,st[105];
    while(top||p)
    {
        while(p)sp.push_down(p),st[++top]=p,p=sp.ch[p][0];
        p=st[top--];
        if(sp.val[p]!=-INF&&sp.val[p]!=INF)
            for(int i=1;i<=sp.num[p];i++)out[++len]=sp.val[p];   // 结点记了出现次数，展开 num 次
        p=sp.ch[p][1];
    }
    return len;
}

int main()
{
    srand(19260817);

    // 1. 文艺平衡树手测：1 2 3 4 5，翻转 [2,4] 应得 1 4 3 2 5
    sp.init(5);
    printf("初始序列:"),sp.print(sp.root),printf("\n");
    sp.reverse(2,4);
    printf("翻转[2,4]后:"),sp.print(sp.root),printf("\n");
    sp.reverse(1,5);
    printf("再翻转[1,5]后:"),sp.print(sp.root),printf("\n");
    sp.reverse(2,2);                                 // 单点翻转不变
    printf("翻转[2,2]后:"),sp.print(sp.root),printf("\n");
    sp.reverse(1,3),sp.reverse(1,3);                 // 翻两次还原
    printf("翻转[1,3]两次后:"),sp.print(sp.root),printf("\n");

    // 2. 普通平衡树手测
    sp.clear();
    int ini[]={5,3,8,1,4,3};
    for(int i=0;i<6;i++)sp.insert(ini[i]);
    printf("hand: size=%d rank(4)=%d kth(1)=%d kth(6)=%d pre(5)=%d next(5)=%d\n",
        sp.sz[sp.root],sp.get_rank(4),sp.kth(1),sp.kth(6),sp.get_pre(5),sp.get_next(5));
    sp.erase(3);
    printf("after erase 3: size=%d rank(3)=%d kth(2)=%d\n",sp.sz[sp.root],sp.get_rank(3),sp.kth(2));

    // 3. 普通平衡树与暴力对拍
    bool ok=true;
    for(int T=1;T<=12&&ok;T++)
    {
        sp.clear(),bflen=0;
        n=rnd(1,40);
        for(int i=1;i<=n;i++){x=rnd(-50,50);sp.insert(x);bf_insert(x);}
        for(int q=1;q<=300&&ok;q++)
        {
            int op=rnd(1,3);
            x=rnd(-60,60);
            if(op==1)sp.insert(x),bf_insert(x);
            else if(op==2)sp.erase(x),bf_erase(x);
            if(sp.sz[sp.root]!=bflen)
            {
                printf("第 %d 轮第 %d 步 size 错: splay=%d bf=%d\n",T,q,sp.sz[sp.root],bflen);
                ok=false;break;
            }
            int r1=sp.get_rank(x),r2=bf_rank(x);
            if(r1!=r2){printf("第 %d 轮第 %d 步 rank 错 x=%d got=%d want=%d\n",T,q,x,r1,r2);ok=false;break;}
            int k=rnd(1,bflen+1);
            int g1=(bflen?sp.kth(k):0),g2=(k<=bflen?bf_kth(k):0);
            if(g1!=g2){printf("第 %d 轮第 %d 步 kth 错 k=%d got=%d want=%d\n",T,q,k,g1,g2);ok=false;break;}
            if(sp.get_pre(x)!=bf_pre(x)){printf("第 %d 轮第 %d 步 pre 错 x=%d\n",T,q,x);ok=false;break;}
            if(sp.get_next(x)!=bf_next(x)){printf("第 %d 轮第 %d 步 next 错 x=%d\n",T,q,x);ok=false;break;}
            if((sp.find_node(x)!=0)!=bf_has(x)){printf("第 %d 轮第 %d 步 find 错 x=%d\n",T,q,x);ok=false;break;}
            if(q%20==0)                              // 周期性体检
            {
                chk_sum=chk_bad=chk_nodes=chk_zero=0;
                check(sp.root);
                if(chk_bad||chk_zero||chk_sum!=bflen||sp.sz[sp.root]!=chk_sum)
                {
                    printf("第 %d 轮第 %d 步 体检失败: bad=%d sum=%d sz[root]=%d 结点数=%d 零num=%d opsz=%d\n",
                        T,q,chk_bad,chk_sum,sp.sz[sp.root],chk_nodes,chk_zero,bflen);
                    ok=false;break;
                }
            }
        }
        // 收尾：中序展开（含重复）必须逐项等于暴力数组
        if(ok)
        {
            static int out[205];
            int len=expand(out);
            if(len!=bflen){printf("第 %d 轮收尾: 长度 splay=%d bf=%d\n",T,len,bflen);ok=false;}
            for(int i=1;i<=len&&ok;i++)
                if(out[i]!=bf[i]){printf("第 %d 轮收尾: 第 %d 个 got=%d want=%d\n",T,i,out[i],bf[i]);ok=false;}
        }
        printf("splay round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 4. 文艺平衡树与暴力对拍：随机区间翻转 + 单点查询
    for(int T=1;T<=10&&ok;T++)
    {
        n=rnd(1,60);
        for(int i=1;i<=n;i++)a[i]=i;
        sp.init(n);
        for(int q=1;q<=150&&ok;q++)
        {
            int l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(rnd(1,3)<=2)
            {
                sp.reverse(l,r);
                reverse(a+l,a+r+1);
            }
            else
            {
                int p=rnd(1,n);
                int got=sp.val[sp.get_pos(p+1)];
                if(got!=a[p]){printf("第 %d 轮翻转第 %d 步错: 位置 %d got=%d want=%d\n",T,q,p,got,a[p]);ok=false;}
            }
        }
        if(ok)                                       // 整体展开再比一遍
        {
            static int out[205];
            int len=expand(out);
            if(len!=n)ok=false;
            for(int i=1;i<=len&&ok;i++)if(out[i]!=a[i])ok=false;
        }
        printf("flip round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 5. 顺序插入 1..100000（考退化）
    sp.clear();
    for(int i=1;i<=100000;i++)sp.insert(i);
    printf("big: size=%d kth1=%d kth100000=%d rank(50000)=%d\n",
        sp.sz[sp.root],sp.kth(1),sp.kth(100000),sp.get_rank(50000));

    // 6. 大规模文艺平衡树：n=50000 翻转 2000 次，抽查位置
    n=50000;
    for(int i=1;i<=n;i++)a[i]=i;
    sp.init(n);
    for(int i=1;i<=2000;i++)
    {
        int l=rnd(1,n),r=rnd(1,n);
        if(l>r)swap(l,r);
        sp.reverse(l,r);
        reverse(a+l,a+r+1);
    }
    int bad=0;
    for(int i=1;i<=200;i++)
    {
        int p=rnd(1,n);
        int q=sp.get_pos(p+1);
        sp.splay(q);                                 // 旋到根，把路径上的 rev 全部放下去
        if(sp.val[q]!=a[p])bad++;
    }
    printf("big flip: 抽查 200 个位置，错 %d 个\n",bad);
    if(bad)ok=false;

    printf("Splay 自测: %s\n",ok?"OK":"FAILED");
    return 0;
}
