// 闵可夫斯基和 的测试与对拍代码
// 模板本体：08-计算几何/闵可夫斯基和.cpp
#include "../../08-计算几何/闵可夫斯基和.cpp"

vector<Point> brute(vector<Point> a,vector<Point> b)
{
    vector<Point> c;
    for(Point x:a)for(Point y:b)c.push_back(x+y);
    return hull(c);
}

bool same(vector<Point> a,vector<Point> b)
{
    sort(a.begin(),a.end()),sort(b.begin(),b.end());
    return a==b;
}

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
