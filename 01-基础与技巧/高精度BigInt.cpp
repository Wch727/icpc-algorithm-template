#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 按区复制：非负加法 0+1，用 add_abs；普通 a+b 复制 0+1+2+3。
// 仅乘法：0+4 或 0+5；除法/取模：0+2+4+6；比较：0+7。
// 各区放在同一个 struct BigInt 内，不需要的区整块省略。
struct BigInt
{
    // [0 公共：存储、构造、去零、绝对值比较、输出]
    vector<int> d;
    int len,sign; // d[0] 为最低位，十进制；sign=1 正，-1 负，零统一为正

    BigInt():d(1,0),len(1),sign(1){}

    BigInt(ll v)
    {
        len=0,sign=1;
        unsigned long long u=v;
        if(v<0)sign=-1,u=0-u;
        while(u>0)
        {
            d.push_back((int)(u%10));
            u/=10;
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

    bool is_zero()const{return len<=0||(len==1&&d[0]==0);}

    bool abs_less(const BigInt& b)const// 比绝对值
    {
        if(len!=b.len)return len<b.len;
        for(int i=len-1;i>=0;i--)
            if(d[i]!=b.d[i])return d[i]<b.d[i];
        return false;
    }

    string to_string()const// 输出用
    {
        string s;
        if(len<=0)return "0";// 空对象当 0
        if(sign==-1&&!is_zero())s+='-';
        for(int i=len-1;i>=0;i--)s+=(char)('0'+d[i]);
        return s;
    }

    // [1 绝对值加法：仅需 0，O(n)]
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

    // [2 绝对值减法：仅需 0，要求 |a|>=|b|，O(n)]
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

    // [3 带符号加减：需 0+1+2，O(n)]
    BigInt operator-()const// 取相反数
    {
        BigInt res=*this;
        if(!res.is_zero())res.sign=-res.sign;
        return res;
    }

    BigInt operator+(const BigInt& b)const
    {
        BigInt res;
        if(sign==b.sign)
        {
            res=add_abs(*this,b);
            if(!res.is_zero())res.sign=sign;
        }
        else
        {
            bool less=abs_less(b);
            res=less?sub_abs(b,*this):sub_abs(*this,b);
            if(!res.is_zero())res.sign=less?b.sign:sign;
        }
        return res;
    }

    BigInt operator-(const BigInt& b)const
    {
        BigInt t=b;// a-b = a+(-b)，符号分情况走加法
        if(!t.is_zero())t.sign=-b.sign;
        return *this+t;
    }

    // [4 高精乘 int：仅需 0，O(n)]
    BigInt operator*(int v)const// 高精乘低精，除法试商时用
    {
        BigInt res;
        if(v==0)return res;// 已经是 0
        res.d.clear();
        res.sign=sign;
        ll u=v;
        if(u<0)res.sign=-res.sign,u=-u;
        ll carry=0;
        int i=0;
        for(;i<len||carry;i++)
        {
            ll cur=carry+(i<len?(ll)d[i]*u:0);
            res.d.push_back((int)(cur%10));
            carry=cur/10;
        }
        res.len=i>0?i:1;
        res.trim();
        return res;
    }

    // [5 高精乘高精：仅需 0，O(nm)]
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
                res.d[i+j]=cur%10;
                carry=cur/10;
            }
        }
        res.len=n;
        res.sign=sign*b.sign;
        res.trim();
        return res;
    }

    // [6 除法与取模：需 0+2+4，无需加法区和高精乘高精区，O(nm)]
    static pair<BigInt,BigInt> divmod_abs(const BigInt& a,const BigInt& b)// 只用绝对值，返回 {商,余数}
    {
        if(a.abs_less(b))return make_pair(BigInt(0),a);
        BigInt q(0),r(0);
        q.d.assign(a.len,0);
        q.len=a.len;
        for(int i=a.len-1;i>=0;i--)// 逐位试商，每位二分 0..9
        {
            r.d.insert(r.d.begin(),a.d[i]); // r=r*10+当前位，无需加法区
            r.len=(int)r.d.size();
            r.trim();
            int lo=0,hi=9,dig=0;
            while(lo<=hi)
            {
                int mid=(lo+hi)>>1;
                if(!r.abs_less(b*mid))dig=mid,lo=mid+1;
                else hi=mid-1;
            }
            q.d[i]=dig;
            r=sub_abs(r,b*dig);
        }
        q.trim();
        r.trim();
        return make_pair(q,r);
    }

    static pair<BigInt,BigInt> divmod(const BigInt& a,const BigInt& b)// 返回 {商,余数}，符号同 C++
    {
        assert(!b.is_zero()); // 除数必须非零
        BigInt x=a,y=b;
        int sa=x.sign,sb=y.sign;
        x.sign=1,y.sign=1;
        pair<BigInt,BigInt> pr=divmod_abs(x,y);
        BigInt q=pr.first,r=pr.second;
        if(!q.is_zero())q.sign=sa*sb;
        if(!r.is_zero())r.sign=sa;// 余数跟被除数同号
        return make_pair(q,r);
    }

    BigInt operator/(const BigInt& b)const{return divmod(*this,b).first;}

    BigInt operator%(const BigInt& b)const{return divmod(*this,b).second;}

    // [7 比较重载：仅需 0，按需选用]
    bool operator<(const BigInt& b)const
    {
        if(sign!=b.sign)return sign<b.sign;
        if(sign==1)return abs_less(b);
        return b.abs_less(*this);
    }

    bool operator>(const BigInt& b)const{return b<*this;}

    bool operator==(const BigInt& b)const
    {
        if(len!=b.len||sign!=b.sign)return false;
        for(int i=0;i<len;i++)
            if(d[i]!=b.d[i])return false;
        return true;
    }

    bool operator!=(const BigInt& b)const{return !(*this==b);}

    bool operator<=(const BigInt& b)const{return !(b<*this);}

    bool operator>=(const BigInt& b)const{return !(*this<b);}
};
