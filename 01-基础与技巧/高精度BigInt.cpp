#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 高精度：加减乘除取模 + 比较 + 输出，竖式运算，每次 O(len^2)
// d[0] 是最低位，base 10；只用 d[0..len-1]；sign=1 正，-1 负
// 低位数组放堆上（vector），对象只占几十字节，表达式里的临时对象不会爆栈
struct BigInt
{
    vector<int> d;
    int len,sign;

    BigInt()// 空对象当 0 用：d 里先不放东西，len=0，to_string 会输出 0
    {
        len=0,sign=1;
    }

    BigInt(ll v)
    {
        len=0,sign=1;
        if(v<0)sign=-1,v=-v;
        while(v>0)
        {
            d.push_back((int)(v%10));
            v/=10;
            len++;
        }
        if(len==0)
        {
            len=1;
            d.push_back(0);// v=0 时存一个 0
        }
    }

    BigInt(const string& s)
    {
        len=0,sign=1;
        if(s.empty())
        {
            len=1,d.push_back(0);
            return;
        }
        int st=0;
        if(s[0]=='-')sign=-1,st=1;
        for(int i=(int)s.size()-1;i>=st;i--)d.push_back(s[i]-'0');
        len=(int)d.size();
        if(len==0)len=1,d.push_back(0);
        trim();
    }

    void trim()// 去掉高位 0，保证 0 的符号是正
    {
        while(len>1&&d[len-1]==0)len--;
        if(len<=0)// 空对象归一成「一个 0」
        {
            len=1;
            d.assign(1,0);
        }
        if(len==1&&d[0]==0)sign=1;
    }

    bool is_zero()const
    {
        return len<=0||(len==1&&d[0]==0);
    }

    bool abs_less(const BigInt& b)const// 比绝对值
    {
        if(len!=b.len)return len<b.len;
        for(int i=len-1;i>=0;i--)
            if(d[i]!=b.d[i])return d[i]<b.d[i];
        return false;
    }

    bool operator<(const BigInt& b)const
    {
        if(sign!=b.sign)return sign<b.sign;
        if(sign==1)return abs_less(b);
        return b.abs_less(*this);
    }

    bool operator>(const BigInt& b)const
    {
        return b<*this;
    }

    bool operator==(const BigInt& b)const
    {
        if(len!=b.len||sign!=b.sign)return false;
        for(int i=0;i<len;i++)
            if(d[i]!=b.d[i])return false;
        return true;
    }

    bool operator!=(const BigInt& b)const
    {
        return !(*this==b);
    }

    bool operator<=(const BigInt& b)const
    {
        return !(b<*this);
    }

    bool operator>=(const BigInt& b)const
    {
        return !(*this<b);
    }

    static BigInt sub_abs(const BigInt& a,const BigInt& b)// 只用绝对值，返回 |a|-|b|（要求 |a|>=|b|）
    {
        BigInt res;
        res.d.assign(max(a.len,b.len)+1,0);// 多一位防借位越界
        int borrow=0;
        for(int i=0;i<a.len||i<b.len||borrow;i++)
        {
            int cur=(i<a.len?a.d[i]:0)-borrow-(i<b.len?b.d[i]:0);
            if(cur<0)cur+=10,borrow=1;
            else borrow=0;
            res.d[i]=cur;
        }
        res.len=(int)res.d.size();
        res.trim();
        return res;
    }

    static BigInt add_abs(const BigInt& a,const BigInt& b)// 只用绝对值相加
    {
        BigInt res;
        res.d.assign(max(a.len,b.len)+1,0);
        int carry=0;
        for(int i=0;i<a.len||i<b.len||carry;i++)
        {
            int sum=carry+(i<a.len?a.d[i]:0)+(i<b.len?b.d[i]:0);
            res.d[i]=sum%10;
            carry=sum/10;
        }
        res.len=(int)res.d.size();
        res.trim();
        return res;
    }

    BigInt operator-()const// 取相反数
    {
        BigInt res=*this;
        if(!res.is_zero())res.sign=-res.sign;
        return res;
    }

    BigInt operator+(const BigInt& b)const
    {
        if(sign==b.sign)
        {
            // 同号：绝对值相加，符号不变（两个 0 也是正的）
            BigInt res=add_abs(*this,b);
            if(!res.is_zero())res.sign=sign;
            return res;
        }
        if(sign==-1)
        {
            // (-|a|) + |b| = |b| - |a|
            BigInt x=*this,y=b;
            x.sign=1,y.sign=1;
            if(x.abs_less(y))
            {
                BigInt res=sub_abs(y,x);
                if(!res.is_zero())res.sign=1;
                return res;
            }
            BigInt res=sub_abs(x,y);
            if(!res.is_zero())res.sign=-1;
            return res;
        }
        // |a| + (-|b|) = |a| - |b|
        BigInt x=*this,y=b;
        x.sign=1,y.sign=1;
        if(x.abs_less(y))
        {
            BigInt res=sub_abs(y,x);
            if(!res.is_zero())res.sign=-1;
            return res;
        }
        BigInt res=sub_abs(x,y);
        if(!res.is_zero())res.sign=1;
        return res;
    }

    BigInt operator-(const BigInt& b)const
    {
        BigInt t=b;// a-b = a+(-b)，符号分情况走加法
        if(!t.is_zero())t.sign=-b.sign;
        return *this+t;
    }

    BigInt operator*(int v)const// 高精乘低精，除法试商时用
    {
        BigInt res;
        if(v==0)return res;// 已经是 0
        res.sign=sign;
        if(v<0)res.sign=-res.sign,v=-v;
        ll carry=0;
        int i=0;
        for(;i<len||carry;i++)
        {
            ll cur=carry+(i<len?(ll)d[i]*v:0);
            res.d.push_back((int)(cur%10));
            carry=cur/10;
        }
        res.len=i>0?i:1;
        res.trim();
        return res;
    }

    BigInt operator*(const BigInt& b)const// 高精乘高精
    {
        BigInt res;
        if(is_zero()||b.is_zero())return res;
        int n=len+b.len+1;// 多一位放最高进位
        res.d.assign(n,0);
        for(int i=0;i<len;i++)
        {
            int carry=0;
            for(int j=0;j<b.len||carry;j++)
            {
                int cur=res.d[i+j]+carry+(j<b.len?d[i]*b.d[j]:0);
                if(i+j<n)res.d[i+j]=cur%10;// 越界那位必定是 0，丢掉
                carry=cur/10;
            }
        }
        res.len=n;
        res.sign=sign*b.sign;
        res.trim();
        return res;
    }

    static pair<BigInt,BigInt> divmod_abs(const BigInt& a,const BigInt& b)// 只用绝对值，返回 {商,余数}
    {
        if(a.abs_less(b))return make_pair(BigInt(0),a);
        BigInt q(0),r(0);
        q.d.assign(a.len,0);
        q.len=a.len;
        for(int i=a.len-1;i>=0;i--)// 逐位试商，每位二分 0..9
        {
            r=r*10+a.d[i];
            int lo=0,hi=9,dig=0;
            while(lo<=hi)
            {
                int mid=(lo+hi)>>1;
                if(!r.abs_less(b*mid))dig=mid,lo=mid+1;
                else hi=mid-1;
            }
            q.d[i]=dig;
            r=r-b*dig;
        }
        q.trim();
        r.trim();
        return make_pair(q,r);
    }

    static pair<BigInt,BigInt> divmod(const BigInt& a,const BigInt& b)// 返回 {商,余数}，符号同 C++
    {
        if(b.is_zero())return make_pair(BigInt(0),a);// 除 0 未定义，直接返回被除数
        BigInt x=a,y=b;
        int sa=x.sign,sb=y.sign;
        x.sign=1,y.sign=1;
        pair<BigInt,BigInt> pr=divmod_abs(x,y);
        BigInt q=pr.first,r=pr.second;
        if(!q.is_zero())q.sign=sa*sb;
        if(!r.is_zero())r.sign=sa;// 余数跟被除数同号
        return make_pair(q,r);
    }

    BigInt operator/(const BigInt& b)const
    {
        return divmod(*this,b).first;
    }

    BigInt operator%(const BigInt& b)const
    {
        return divmod(*this,b).second;
    }

    string to_string()const// 输出用
    {
        string s;
        if(len<=0)return "0";// 空对象当 0
        if(sign==-1&&!is_zero())s+='-';
        for(int i=len-1;i>=0;i--)s+=(char)('0'+d[i]);
        return s;
    }
};

void chk(const char* name,const string& got,const string& want)
{
    if(got!=want)printf("fail %s: got %s want %s\n",name,got.c_str(),want.c_str());
}

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
