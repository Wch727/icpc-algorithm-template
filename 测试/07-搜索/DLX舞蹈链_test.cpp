// DLX舞蹈链 的测试与对拍代码
// 模板本体：07-搜索/DLX舞蹈链.cpp
#include "../../07-搜索/DLX舞蹈链.cpp"

bool test(const vector<int> &a,int m)
{
    Dlx tr;
    tr.init(m);
    for(int i=0;i<(int)a.size();i++)
    {
        vector<int> c;
        for(int j=0;j<m;j++)if(a[i]>>j&1)c.push_back(j+1);
        tr.add_row(i,c);
    }
    bool want=false;
    for(int s=0;s<(1<<(int)a.size());s++)
    {
        int used=0;
        bool ok=true;
        for(int i=0;i<(int)a.size();i++)if(s>>i&1)
        {
            if(used&a[i])ok=false;
            used|=a[i];
        }
        if(ok&&used==(1<<m)-1)want=true;
    }
    bool got=tr.solve();
    if(got!=want)return false;
    if(tr.solve()!=want)return false;
    if(got)
    {
        int used=0;
        for(int x:tr.ans)
        {
            if(used&a[x])return false;
            used|=a[x];
        }
        if(used!=(1<<m)-1)return false;
    }
    return true;
}

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
