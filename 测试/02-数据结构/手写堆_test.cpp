// 手写堆 的测试与对拍代码
// 模板本体：02-数据结构/手写堆.cpp
#include "../../02-数据结构/手写堆.cpp"

int main()
{
    srand(20240513);

    // 1. 小数据手测：小根堆堆顶最小、大根堆堆顶最大
    int ini[]={0,5,1,9,3,7};
    for(int i=1;i<=5;i++)mn.push(ini[i]),mx.push(ini[i]);
    printf("min-top=%d max-top=%d valid=%d%d\n",mn.top(),mx.top(),(int)mn.valid(),(int)mx.valid());

    // 2. 与 priority_queue 对拍
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        mn.clear(),mx.clear();
        priority_queue<int,vector<int>,greater<int> > q1;   // 小根堆
        priority_queue<int> q2;                             // 大根堆
        for(int i=1;i<=3000;i++)
        {
            int x=rand()%1000-500;
            mn.push(x),mx.push(x),q1.push(x),q2.push(x);
            if(mn.top()!=q1.top()||mx.top()!=q2.top()){ok=false;break;}
            if(!mn.valid()||!mx.valid()){ok=false;break;}
            if(i%3==0)
            {
                mn.pop(),mx.pop(),q1.pop(),q2.pop();
                if(mn.top()!=q1.top()||mx.top()!=q2.top()){ok=false;break;}
                if(!mn.valid()||!mx.valid()){ok=false;break;}
            }
        }
        printf("heap round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 堆排序对拍 sort
    for(int T=1;T<=10;T++)
    {
        n=rand()%200+1;
        for(int i=1;i<=n;i++)a[i]=rand()%100-50,b[i]=a[i];
        heap_sort(a,n);
        sort(b+1,b+n+1);
        bool same=true;
        for(int i=1;i<=n;i++)if(a[i]!=b[i]){same=false;break;}
        printf("sort round %d %s\n",T,same?"passed":"FAILED");
        if(!same)break;
    }

    // 4. O(n) 建堆后逐个弹出，应当升序（小根堆）
    int c[]={0,4,2,8,1,9,3};
    mn.build(c,6);
    printf("build-pop: ");
    while(!mn.empty())printf("%d ",mn.top()),mn.pop();
    printf("\n");

    // 5. 小根堆套 P3378 的用法：1 插入 x，2 输出最小值，3 删除最小值
    mn.clear();
    mn.push(3),printf("%d ",mn.top()),mn.push(1),printf("%d ",mn.top()),mn.pop(),printf("%d\n",mn.top());
    return 0;
}
