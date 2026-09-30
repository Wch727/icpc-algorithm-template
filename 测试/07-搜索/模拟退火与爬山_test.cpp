// 模拟退火与爬山 的测试与对拍代码
// 模板本体：07-搜索/模拟退火与爬山.cpp
#include "../../07-搜索/模拟退火与爬山.cpp"

int main()
{
    srand(19260817);
    bool ok=true;
    for(int t=0;t<=40;t++)
    {
        double x=t==0?2:(rand()%2001-1000)/100.0;
        double y=t==0?-3:(rand()%2001-1000)/100.0;
        Optimizer tr(x,y,19260817+t);
        Point start={10,-10},p=tr.anneal(start),q=tr.climb(p),h=tr.climb(start);
        // 暴力网格枚举，另用已知解析最优值 0 检查精度
        double want=1e100;
        for(int i=-40;i<=40;i++)
            for(int j=-40;j<=40;j++)want=min(want,tr.value({i/4.0,j/4.0}));
        if(tr.value(p)>tr.value(start)||tr.value(q)>want+1e-8||tr.value(q)>1e-12||tr.value(h)>1e-12)ok=false;
    }
    printf("模拟退火与爬山 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
