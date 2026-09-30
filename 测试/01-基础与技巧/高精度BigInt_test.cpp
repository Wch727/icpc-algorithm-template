// 高精度BigInt 的测试与对拍代码
// 模板本体：01-基础与技巧/高精度BigInt.cpp
#include "../../01-基础与技巧/高精度BigInt.cpp"

int main()
{
    srand(20240523);
    // 自测1：手算样例，含负数、零、借位
    chk("add1",(BigInt("12")+BigInt("34")).to_string(),"46");
    chk("sub1",(BigInt("12")-BigInt("34")).to_string(),"-22");
    chk("mul1",(BigInt("12")*BigInt("34")).to_string(),"408");
    chk("sub2",(BigInt("100000000000000000000")-BigInt("99999999999999999999")).to_string(),"1");
    chk("mul2",(BigInt("123456789")*BigInt("987654321")).to_string(),"121932631112635269");
    chk("add_z",(BigInt("1234")+BigInt("-1234")).to_string(),"0");
    chk("mul_z",(BigInt("-5")*BigInt("0")).to_string(),"0");
    chk("sub3",(BigInt("0")-BigInt("5000")).to_string(),"-5000");
    chk("neg",(-BigInt("7")).to_string(),"-7");
    printf("hard sample self-check OK\n");fflush(stdout);

    // 自测2：小数据与 long long 对拍
    for(int t=1;t<=20000;t++)
    {
        ll x=(ll)(rand()%100000),y=(ll)(rand()%100000);
        if(rand()%2)x=-x;
        if(rand()%2)y=-y;
        BigInt A(x),B(y);
        string got,want;
        got=(A+B).to_string();
        want=to_string(x+y);
        chk("add",got,want);
        got=(A-B).to_string();
        want=to_string(x-y);
        chk("sub",got,want);
        got=(A*B).to_string();
        want=to_string(x*y);
        chk("mul",got,want);
        if(y!=0)
        {
            got=(A/B).to_string();
            want=to_string(x/y);
            chk("div",got,want);
            got=(A%B).to_string();
            want=to_string(x%y);
            chk("mod",got,want);
        }
        if((A<B)!=(x<y)||(A>B)!=(x>y)||(A==B)!=(x==y))
        {
            printf("fail cmp %lld %lld\n",x,y);
            return 0;
        }
    }
    printf("ll differential self-check OK\n");

    // 自测3：大除大 + 位数边界
    {
        BigInt a(string(6,'9')),b(string(3,'7'));// 999999 / 777
        pair<BigInt,BigInt> pr=BigInt::divmod(a,b);
        chk("divmod_small",pr.first.to_string(),"1287");
        chk("divmod_rem",pr.second.to_string(),"0");
        BigInt a2(string(400,'9')),b2(string(200,'7'));// 400 位除 200 位
        pair<BigInt,BigInt> pr2=BigInt::divmod(a2,b2);
        chk("big_divmod",(pr2.first*b2+pr2.second).to_string(),a2.to_string());
        printf("big divmod: quotient digits=%d, a=q*b+r OK\n",pr2.first.len);
        BigInt big(string(60,'9'));
        BigInt sqv=big*big;// (10^60-1)^2 = 10^120-2*10^60+1
        string sq=sqv.to_string();
        bool ok=(sq.size()==120)&&sq[0]=='9'&&sq[119]=='1'&&sq[59]=='8';
        if(!ok)printf("fail square: len=%d head=%c tail=%c mid=%c\n",(int)sq.size(),sq[0],sq[119],sq[59]);
        chk("square_div",(sqv/big).to_string(),big.to_string());
    }
    printf("big digit self-check OK\n");

    // 自测4：洛谷 P1932 的调用方式（读入 a b，输出 和 差 积 商 余）
    cin.sync_with_stdio(false);
    cin.tie(0);
    string s1,s2;
    if(cin>>s1>>s2)
    {
        BigInt a(s1),b(s2);
        printf("%s\n",(a+b).to_string().c_str());
        printf("%s\n",(a-b).to_string().c_str());
        printf("%s\n",(a*b).to_string().c_str());
        printf("%s\n",(a/b).to_string().c_str());
        printf("%s\n",(a%b).to_string().c_str());
    }
    return 0;
}
