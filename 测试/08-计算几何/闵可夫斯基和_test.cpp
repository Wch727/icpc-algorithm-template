// 闵可夫斯基和 的测试与对拍代码
// 模板本体：08-计算几何/闵可夫斯基和.cpp
#include "../../08-计算几何/闵可夫斯基和.cpp"

int main()
{
    srand(20260930);
    bool ok=same(minkowski({{0,0},{1,0},{1,1},{0,1}},{{0,0},{1,0},{1,1},{0,1}}),{{0,0},{2,0},{2,2},{0,2}});
    ok&=minkowski({},{{0,0}}).empty();
    for(int t=1;t<=400;t++)
    {
        vector<Point> a,b;
        for(int i=0,n=rand()%10+1;i<n;i++)a.push_back({rand()%11-5,t%4?rand()%11-5:0});
        for(int i=0,n=rand()%10+1;i<n;i++)b.push_back({rand()%11-5,t%5?rand()%11-5:0});
        if(!same(minkowski(hull(a),hull(b)),brute(a,b)))ok=false;
    }
    printf("闵可夫斯基和: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
