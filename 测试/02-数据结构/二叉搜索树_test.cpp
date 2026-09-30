// 二叉搜索树 的测试与对拍代码
// 模板本体：02-数据结构/二叉搜索树.cpp
#include "../../02-数据结构/二叉搜索树.cpp"

int main()
{
    srand(20240513);

    // 1. 手测：按 P5076 的用法插入 1 4 2 5 3
    t.clear(),root=0;
    int ini[]={1,4,2,5,3};
    for(int i=0;i<5;i++)root=t.insert(root,ini[i]);
    printf("rank(3)=%d kth(1)=%d kth(5)=%d\n",t.get_rank(root,3),t.kth(root,1),t.kth(root,5));
    printf("pre(3)=%d next(3)=%d pre(1)=%d next(5)=%d\n",t.get_pre(root,3),t.get_next(root,3),t.get_pre(root,1),t.get_next(root,5));
    root=t.erase(root,3);
    printf("after erase 3: rank(4)=%d kth(4)=%d find(3)=%d size=%d\n",t.get_rank(root,4),t.kth(root,4),(int)t.find(root,3),t.sz[root]);

    // 2. 随机对拍：插入 / 删除 / 五种查询 与有序数组暴力比较
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        t.clear(),root=0,bf.clear();
        n=300;
        for(int i=1;i<=n;i++)
        {
            x=rand()%200;
            if(rand()%4==0)                 // 1/4 概率删除
            {
                root=t.erase(root,x);
                bf_erase(x);
            }
            else
            {
                root=t.insert(root,x);
                bf_insert(x);
            }
            if(rand()%3==0)
            {
                if(t.get_rank(root,x)!=bf_rank(x)){ok=false;break;}
                int k=rand()%((int)bf.size()+2)+1;
                if(t.kth(root,k)!=bf_kth(k)){ok=false;break;}
                if(t.get_pre(root,x)!=bf_pre(x)){ok=false;break;}
                if(t.get_next(root,x)!=bf_next(x)){ok=false;break;}
                if(t.find(root,x)!=(bool)binary_search(bf.begin(),bf.end(),x)){ok=false;break;}
            }
        }
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 边界：空树 / 单结点 / 删除到空
    t.clear(),root=0,bf.clear();
    printf("empty: rank=%d kth=%d pre=%d next=%d\n",t.get_rank(root,5),t.kth(root,1),t.get_pre(root,5),t.get_next(root,5));
    root=t.insert(root,7);
    printf("one: rank(7)=%d rank(9)=%d kth(1)=%d pre(7)=%d next(7)=%d\n",t.get_rank(root,7),t.get_rank(root,9),t.kth(root,1),t.get_pre(root,7),t.get_next(root,7));
    root=t.erase(root,7);
    printf("erase to empty: size=%d kth(1)=%d\n",t.sz[root],t.kth(root,1));

    // 样例（P5076）：插入 1 4 2 5 3 后 rank(3)=1(即第 2 小)，kth(1)=1
    return 0;
}
