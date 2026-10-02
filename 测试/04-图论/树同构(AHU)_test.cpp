#include "../../04-图论/树同构(AHU).cpp"
bool brute(const vector<vector<int>> &a,const vector<vector<int>> &b)
{
    int n=a.size()-1;
    vector<int> p(n);iota(p.begin(),p.end(),1);
    vector<vector<bool>> edge(n+1,vector<bool>(n+1));
    for(int u=1;u<=n;u++)for(int v:b[u])edge[u][v]=true;
    do
    {
        bool ok=true;
        for(int u=1;u<=n&&ok;u++)
        {
            if(a[u].size()!=b[p[u-1]].size()){ok=false;break;}
            for(int v:a[u])if(!edge[p[u-1]][p[v-1]]){ok=false;break;}
        }
        if(ok)return true;
    }while(next_permutation(p.begin(),p.end()));
    return false;
}
int main()
{
    assert(same_tree({{}},{{}}));
    assert(!same_tree({{}},{{},{}}));
    mt19937 g(204);
    for(int n=1;n<=7;n++)for(int t=0;t<70;t++)
    {
        vector<vector<int>> a(n+1),b(n+1),c(n+1);
        for(int u=2;u<=n;u++)
        {
            int v=1+g()%(u-1);a[u].push_back(v),a[v].push_back(u);
            v=1+g()%(u-1);b[u].push_back(v),b[v].push_back(u);
        }
        assert(same_tree(a,b)==brute(a,b));
        vector<int> p(n+1);iota(p.begin(),p.end(),0);shuffle(p.begin()+1,p.end(),g);
        for(int u=1;u<=n;u++)for(int v:a[u])c[p[u]].push_back(p[v]);
        for(auto &v:c)shuffle(v.begin(),v.end(),g);
        assert(same_tree(a,c));
    }
    int n=100000;
    vector<vector<int>> a(n+1),b(n+1);
    for(int u=2;u<=n;u++)a[u].push_back(u-1),a[u-1].push_back(u),b[u].push_back(u-1),b[u-1].push_back(u);
    assert(same_tree(a,b)&&tree_centroids(a).size()==2);
    cout<<"AHU OK\n";
}
