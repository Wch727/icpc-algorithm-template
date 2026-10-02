// 平衡树Treap 的测试与对拍代码
// 模板本体：02-数据结构/平衡树Treap.cpp
#include "../../02-数据结构/平衡树Treap.cpp"

vector<int> bf;                            // 暴力容器（有序、去重），插入前用 binary_search 判重

bool bf_has(int x){return binary_search(bf.begin(),bf.end(),x);}
void bf_insert(int x)
{
    if(bf_has(x))return;
    bf.insert(lower_bound(bf.begin(),bf.end(),x),x);
}
void bf_erase(int x)
{
    vector<int>::iterator it=lower_bound(bf.begin(),bf.end(),x);
    if(it!=bf.end()&&*it==x)bf.erase(it);
}
int bf_rank(int x){return lower_bound(bf.begin(),bf.end(),x)-bf.begin();}
int bf_kth(int k){return (k>=1&&k<=(int)bf.size())?bf[k-1]:0;}
int bf_pre(int x)
{
    int pos=lower_bound(bf.begin(),bf.end(),x)-bf.begin()-1;
    return pos>=0?bf[pos]:-INF;
}
int bf_next(int x)
{
    int pos=upper_bound(bf.begin(),bf.end(),x)-bf.begin();
    return pos<(int)bf.size()?bf[pos]:INF;
}

int main()
{
    // 1. 手测：插入 5 3 8 1 4，看第 k 小和前驱后继
    t.clear(),root=0;
    int ini[]={5,3,8,1,4};
    for(int i=0;i<5;i++)t.insert(root,ini[i]);
    printf("kth1=%d kth3=%d kth5=%d min=%d max=%d\n",t.kth(root,1),t.kth(root,3),t.kth(root,5),t.get_min(root),t.get_max(root));
    printf("rank(4)=%d pre(4)=%d next(4)=%d pre(1)=%d next(8)=%d\n",t.get_rank(root,4),t.get_pre(root,4),t.get_next(root,4),t.get_pre(root,1),t.get_next(root,8));
    printf("inorder:"),t.print_inorder(root),printf("\n");
    t.erase(root,5);
    printf("after erase 5: kth4=%d size=%d find(5)=%d\n",t.kth(root,4),t.sz[root],(int)t.find(root,5));

    // 2. 随机多轮对拍：插入 / 删除 / 五种查询 与暴力比较
    bool ok=true;
    for(int T=1;T<=15&&ok;T++)
    {
        t.clear(),root=0,bf.clear();
        n=1000;
        for(int i=1;i<=n;i++)
        {
            x=rand()%400;
            int do_erase=(rand()%3==0);         // 先决定动作，保证两边完全同步
            if(do_erase)
            {
                t.erase(root,x);
                bf_erase(x);
            }
            else
            {
                t.insert(root,x);
                bf_insert(x);
            }
            if(t.sz[root]!=(int)bf.size())
            {
                printf("第 %d 步 size 对不上：树=%d bf=%d\n",i,t.sz[root],(int)bf.size());
                ok=false;break;
            }
            if(rand()%4==0)
            {
                if(t.get_rank(root,x)!=bf_rank(x)){printf("第 %d 步 rank 错 x=%d\n",i,x);ok=false;break;}
                int k=rand()%((int)bf.size()+2)+1;
                if(t.kth(root,k)!=bf_kth(k)){printf("第 %d 步 kth 错 k=%d\n",i,k);ok=false;break;}
                if(t.get_pre(root,x)!=bf_pre(x)){printf("第 %d 步 pre 错 x=%d\n",i,x);ok=false;break;}
                if(t.get_next(root,x)!=bf_next(x)){printf("第 %d 步 next 错 x=%d\n",i,x);ok=false;break;}
                if(t.find(root,x)!=bf_has(x)){printf("第 %d 步 find 错 x=%d\n",i,x);ok=false;break;}
            }
        }
        // 收尾再整体比一遍：中序应当等于 bf
        if(ok)
        {
            for(int i=0;i<(int)bf.size()&&ok;i++)
                if(t.kth(root,i+1)!=bf[i])ok=false;
        }
        if(ok&&!bf.empty()&&(t.get_min(root)!=bf.front()||t.get_max(root)!=bf.back()))ok=false;
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 边界：空树 / 单结点 / 删到空
    t.clear(),root=0;
    printf("empty: rank=%d kth=%d pre=%d next=%d\n",t.get_rank(root,5),t.kth(root,1),t.get_pre(root,5),t.get_next(root,5));
    t.insert(root,7);
    printf("one: rank(7)=%d kth(1)=%d pre(7)=%d next(7)=%d\n",t.get_rank(root,7),t.kth(root,1),t.get_pre(root,7),t.get_next(root,7));
    t.erase(root,7);
    printf("erase to empty: size=%d kth(1)=%d\n",t.sz[root],t.kth(root,1));

    // 4. 大量有序插入（比 BST 抗退化）：1..100000 顺序插入后查第 1 小
    t.clear(),root=0;
    for(int i=1;i<=100000;i++)t.insert(root,i);
    printf("ordered insert: size=%d kth1=%d kth100000=%d\n",t.sz[root],t.kth(root,1),t.kth(root,100000));
    return 0;
}
