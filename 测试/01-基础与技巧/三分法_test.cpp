// 三分法 的测试与对拍代码
// 模板本体：01-基础与技巧/三分法.cpp
#include "../../01-基础与技巧/三分法.cpp"

template<typename F>

double brute_min(double l,double r,F f,int step=1000000)// 暴力扫描当参照
{
    double ans=1e300;
    for(int i=0;i<=step;i++)ans=min(ans,f(l+(r-l)*i/step));
    return ans;
}

int main()
{
    srand(20240517);
    // 自测1：f(x)=(x-3)^2+1 最小值点 3
    double x1=trisearch_min(-10,10,[](double x){return (x-3)*(x-3)+1;});
    printf("min at %.6f, val %.6f (want 3, 1)\n",x1,(x1-3)*(x1-3)+1);
    if(fabs(x1-3)>1e-6)printf("fail convex min\n");

    // 自测2：f(x)=-((x+2)^2)+5 最大值点 -2
    double x2=trisearch_max(-10,10,[](double x){return -(x+2)*(x+2)+5;});
    printf("max at %.6f, val %.6f (want -2, 5)\n",x2,-(x2+2)*(x2+2)+5);
    if(fabs(x2+2)>1e-6)printf("fail concave max\n");

    // 自测3：与 1e6 点暴力扫描的极值对拍（只比函数值，避免平台区误差）
    for(int t=1;t<=100;t++)
    {
        double a=(double)(rand()%200-100)/100.0;
        double b=(double)(rand()%200-100)/100.0;
        double c=(double)(rand()%200-100)/100.0;
        if(a<0)a=-a;
        a+=0.01;// 保证是凸函数
        double got=trisearch_min(-50,50,[&](double x){return a*x*x+b*x+c;});
        double want=-b/(2*a);// 理论最值点
        double bf=brute_min(-50,50,[&](double x){return a*x*x+b*x+c;});
        if(fabs(a*got*got+b*got+c-bf)>1e-3||fabs(got-want)>1e-6)
        {
            printf("fail trisearch a=%.4f b=%.4f c=%.4f got=%.8f want=%.8f\n",a,b,c,got,want);
            return 0;
        }
    }
    printf("trisearch double self-check OK\n");

    // 自测4：套洛谷 P1883 的写法，F(x)=max(a x^2+b x+c)
    n=2;
    aa[0]=2,bb[0]=0,cc[0]=0;// f0=2x^2
    aa[1]=1,bb[1]=-1,cc[1]=1;// f1=x^2-x+1，F=max(f0,f1) 在 x=0.5 处最小，值 0.75
    double l=0,r=1000;
    for(int i=1;i<=200;i++)
    {
        double lm=l+(r-l)/3.0,rm=r-(r-l)/3.0;
        if(F_val(lm)>F_val(rm))l=lm;
        else r=rm;
    }
    double bf=1e300,bfx=0;// 暴力扫描当参照
    for(int i=0;i<=1000000;i++)
    {
        double x=i/1000000.0;
        if(F_val(x)<bf)bf=F_val(x),bfx=x;
    }
    printf("P1883 x=%.6f F=%.6f (brute x=%.6f F=%.6f)\n",(l+r)/2,F_val((l+r)/2),bfx,bf);
    if(fabs(F_val((l+r)/2)-bf)>1e-6||fabs((l+r)/2-bfx)>1e-3)printf("fail P1883 template\n");
    return 0;
}
