#include "../../05-数学/自适应Simpson积分.cpp"
int main()
{
    bool ok;
    auto near=[](double a,double b){assert(fabs(a-b)<1e-8);};
    near(simpson([](double x){return x*x*x*x;},0,1,1e-10,ok),0.2);assert(ok);
    near(simpson([](double x){return x*x*x*x;},1,0,1e-10,ok),-0.2);assert(ok);
    near(simpson([](double x){return sin(x);},0,acos(-1),1e-10,ok),2);assert(ok);
    near(simpson([](double x){return exp(x);},-2,3,1e-10,ok),exp(3)-exp(-2));assert(ok);
    near(simpson([](double){return 3.0;},-5,4,1e-10,ok),27);assert(ok);
    assert(simpson([](double){assert(false);return 1.0;},2,2,1e-10,ok)==0&&ok);
    simpson([](double x){return exp(x);},0,1,1e-15,ok,0);assert(!ok);
    simpson([](double){return 1.0;},1,nextafter(1.0,2.0),1e-10,ok);assert(!ok);
    cout<<"Simpson OK\n";
}
