#include<bits/stdc++.h>
using namespace std;

// 无符号整数多重集：插入、删一个副本、查询 max(x^y)，O(B)，B=32 含最高位。
// 修改为较小 B 时所有输入必须小于 2^B；空集合不能查最大异或。
// 删除不回收节点，空间 O(插入过的不同数*B)；要输出 y，用 x^max_xor(x)。
struct BinaryTrie
{
    struct Node{int ch[2]={0,0},cnt=0;};
    vector<Node> tr=vector<Node>(1);
    int B;
    BinaryTrie(int bits=32):B(bits){assert(1<=B&&B<=32);}
    void clear(){tr.assign(1,Node{});}
    void insert(uint32_t x)
    {
        assert(B==32||(x>>B)==0);
        int p=0;
        ++tr[p].cnt;
        for(int i=B-1;i>=0;i--)
        {
            int b=x>>i&1;
            if(!tr[p].ch[b])
            {
                tr[p].ch[b]=tr.size();
                tr.push_back(Node{});
            }
            p=tr[p].ch[b],++tr[p].cnt;
        }
    }
    bool erase(uint32_t x)
    {
        assert(B==32||(x>>B)==0);
        int p=0;
        for(int i=B-1;i>=0;i--)
        {
            p=tr[p].ch[x>>i&1];
            if(!p||!tr[p].cnt)return false;
        }
        p=0,--tr[p].cnt;
        for(int i=B-1;i>=0;i--)p=tr[p].ch[x>>i&1],--tr[p].cnt;
        return true;
    }
    uint32_t max_xor(uint32_t x)
    {
        assert(tr[0].cnt>0&&(B==32||(x>>B)==0));
        uint32_t ans=0;
        int p=0;
        for(int i=B-1;i>=0;i--)
        {
            int b=x>>i&1,q=tr[p].ch[b^1];
            if(q&&tr[q].cnt)ans|=uint32_t(1)<<i,p=q;
            else p=tr[p].ch[b];
        }
        return ans;
    }
};
