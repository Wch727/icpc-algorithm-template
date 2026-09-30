// KD-Tree 的测试与对拍代码
// 模板本体：02-数据结构/KD-Tree.cpp
#include "../../02-数据结构/KD-Tree.cpp"

int main()
{
    mt19937 rng(619);
    int bad=0,cnt=0;
    KDTree sample({{0,0},{3,4},{3,4}}),empty({});
    bad+=sample.nearest({0,1})!=1||sample.rectangle(3,4,3,4)!=2;
    bad+=empty.nearest({0,0})!=LLONG_MAX||empty.rectangle(-1,-1,1,1)!=0;
    for(int t=1;t<=30;t++)
    {
        int n=rng()%100+1;
        vector<KDTree::Point> a;
        for(int i=1;i<=n;i++)a.push_back({(int)(rng()%101)-50,(int)(rng()%101)-50});
        KDTree tr(a);
        for(int q=1;q<=300;q++)
        {
            KDTree::Point p={(int)(rng()%151)-75,(int)(rng()%151)-75};
            ll ans=LLONG_MAX;
            for(KDTree::Point v:a)ans=min(ans,tr.dist(p,v));
            bad+=tr.nearest(p)!=ans;
            ll x1=(int)(rng()%151)-75,x2=(int)(rng()%151)-75,y1=(int)(rng()%151)-75,y2=(int)(rng()%151)-75;
            if(x1>x2)swap(x1,x2);
            if(y1>y2)swap(y1,y2);
            int z=0;
            for(KDTree::Point v:a)z+=x1<=v.x&&v.x<=x2&&y1<=v.y&&v.y<=y2;
            bad+=tr.rectangle(x1,y1,x2,y2)!=z;
            cnt++;
        }
    }
    printf("KD-Tree 最近点/矩形计数: %s, 对拍=%d\n",bad?"FAILED":"OK",cnt);
    if(bad)return 1;
    return 0;
}
