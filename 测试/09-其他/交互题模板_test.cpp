#include "../../09-其他/交互题模板.cpp"
#include "../helpers/交互假评测器.hpp"

// 用流缓冲模拟评测机，直接运行模板 solve，而不是复制二分算法测试。
struct ReplyBuf:streambuf
{
    ostringstream &out;
    string data="1 1\n";
    size_t used=0;
    int value,calls=0;
    ReplyBuf(ostringstream &out,int value):out(out),value(value){setg(data.data(),data.data(),data.data()+data.size());}
    int_type underflow()override
    {
        string text=out.str();
        istringstream query(text.substr(used));
        used=text.size();
        char type;
        int k,x;
        assert((query>>type>>k>>x)&&type=='?'&&k==1);
        assert(++calls<=30);
        data=to_string(value>x)+"\n";
        setg(data.data(),data.data(),data.data()+data.size());
        return traits_type::to_int_type(*gptr());
    }
};

int main()
{
    judge::init("5 3 5 1 9 3 7");
    assert(judge::reply("? 3 5")==0);
    assert(judge::reply("? 3 4")==1);
    assert(judge::check_answer("! 5"));
    for(string s:{"","?","? 1","? x 0","? 0 0","? 1 0 extra"})assert(judge::reply(s)==INT_MIN);
    for(string s:{"","!","! x","! 5 extra","! 7"})assert(!judge::check_answer(s));
    judge::limit=judge::qcnt;
    assert(judge::reply("? 1 0")==INT_MIN);
    judge::init("2 1 5");
    assert(!judge::check_answer("! 5"));
    mt19937 rng(20261001);
    vector<int> values={0,1,2,999999999,1000000000};
    for(int i=0;i<100;i++)values.push_back(rng()%1000000001);
    for(int value:values)
    {
        ostringstream out;
        ReplyBuf input(out,value);
        auto oldin=cin.rdbuf(&input),oldout=cout.rdbuf(out.rdbuf());
        cin.clear();
        solve();
        cin.rdbuf(oldin);cout.rdbuf(oldout);cin.clear();
        string result=out.str(),final="! "+to_string(value)+"\n";
        assert(result.size()>=final.size()&&result.substr(result.size()-final.size())==final);
    }
    puts("交互实际 solve、值域端点与协议格式：OK");
}
