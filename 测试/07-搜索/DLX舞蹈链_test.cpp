// DLX舞蹈链 的测试与对拍代码
// 模板本体：07-搜索/DLX舞蹈链.cpp
#include "../../07-搜索/DLX舞蹈链.cpp"

int main()
{
    srand(19260817);
    bool ok=test({3,12,5,10},4)&&test({},0)&&test({1},2);
    for(int t=1;t<=100;t++)
    {
        int m=rand()%7+1,n=rand()%12;
        vector<int> a(n);
        for(int &x:a)x=rand()%((1<<m)-1)+1;
        if(!test(a,m))ok=false;
    }
    printf("DLX精确覆盖 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
