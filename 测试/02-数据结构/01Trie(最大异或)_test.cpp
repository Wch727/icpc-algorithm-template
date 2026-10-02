#include "../../02-数据结构/01Trie(最大异或).cpp"
int main()
{
    BinaryTrie t;
    assert(!t.erase(0));
    t.insert(0),t.insert(UINT32_MAX),t.insert(UINT32_MAX);
    assert(t.max_xor(0)==UINT32_MAX&&t.erase(UINT32_MAX)&&t.max_xor(0)==UINT32_MAX);
    assert(t.erase(UINT32_MAX)&&!t.erase(UINT32_MAX)&&t.max_xor(UINT32_MAX)==UINT32_MAX);
    t.clear();
    multiset<uint32_t> s;
    mt19937 g(201);
    for(int i=0;i<12000;i++)
    {
        uint32_t x=g()%3?g()%128:g();
        int op=g()%3;
        if(op==0||s.empty())t.insert(x),s.insert(x);
        else if(op==1)
        {
            auto it=s.find(x);
            assert(t.erase(x)==(it!=s.end()));
            if(it!=s.end())s.erase(it);
        }
        else
        {
            uint32_t ans=0;
            for(uint32_t v:s)ans=max(ans,x^v);
            assert(t.max_xor(x)==ans);
        }
        assert(t.tr[0].cnt==(int)s.size());
    }
    BinaryTrie small(1);small.insert(0),small.insert(1);
    assert(small.max_xor(0)==1&&small.erase(1)&&small.max_xor(0)==0);
    cout<<"BinaryTrie OK\n";
}
