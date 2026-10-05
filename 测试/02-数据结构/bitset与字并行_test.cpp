// bitset与字并行 的测试与对拍代码
// 模板本体：02-数据结构/bitset与字并行.cpp
#include "../../02-数据结构/bitset与字并行.cpp"
bitset<N> subset_sum(const vector<int> &arg_a){a=arg_a;return subset_sum();}
void closure(vector<bitset<N> > &arg_g){g=arg_g;closure();arg_g=g;}
namespace masks {
#include "../../01-基础与技巧/位运算技巧.cpp"
}
using masks::count_subset;

int main()
{
    srand(19260817);
    assert(count_subset(UINT_MAX,UINT_MAX)==(1ULL<<32));
    assert(subset_sum({N,1})[1]&&!subset_sum({N,1})[N-1]);
    bool ok=subset_sum({2,3})[5]&&!subset_sum({2,3})[4];
    for(int t=1;t<=60;t++)
    {
        int n=rand()%12;
        vector<int> a(n);
        for(int &x:a)x=rand()%20;
        bitset<N> f=subset_sum(a),want;
        for(int s=0;s<(1<<n);s++)
        {
            int sum=0;
            for(int i=0;i<n;i++)if(s>>i&1)sum+=a[i];
            want[sum]=1;
        }
        if(f!=want)ok=false;
        bitset<N> b,c;
        for(int i=0;i<N;i++)b[i]=rand()%2,c[i]=rand()%2;
        bitset<N> un=b|c,in=b&c,dif=b&~c;
        for(int i=0;i<N;i++)if(un[i]!=(b[i]||c[i])||in[i]!=(b[i]&&c[i])||dif[i]!=(b[i]&&!c[i]))ok=false;
        n=rand()%15+1;
        vector<bitset<N> > g(n);
        vector<vector<int> > br(n,vector<int>(n));
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                br[i][j]=rand()%4==0;
                g[i][j]=br[i][j];
            }
        closure(g);
        // 每个起点独立 BFS
        for(int s=0;s<n;s++)
        {
            vector<int> vis(n),q(1,s);
            vis[s]=1;
            for(int k=0;k<(int)q.size();k++)
                for(int v=0;v<n;v++)if(br[q[k]][v]&&!vis[v])vis[v]=1,q.push_back(v);
            for(int v=0;v<n;v++)if(g[s][v]!=(bool)vis[v])ok=false;
        }
        unsigned mask=rand()%256,allow=rand()%256;
        unsigned long long cnt=0;
        for(unsigned s=0;s<256;s++)if((s&mask)==s&&(s&allow)==s)cnt++;
        if(count_subset(mask,allow)!=cnt)ok=false;
    }
    printf("bitset优化技巧 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
