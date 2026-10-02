#include "../../02-数据结构/左偏树(可并堆).cpp"
int main()
{
    LeftistHeap h;
    int x=h.insert(0,LLONG_MAX),y=h.insert(0,LLONG_MIN);
    x=h.merge(x,y);assert(h.top(x)==LLONG_MIN);x=h.pop(x);assert(h.top(x)==LLONG_MAX&&h.pop(x)==0);
    h.clear();
    vector<int> root(20);
    vector<multiset<ll>> s(20);
    mt19937 g(205);
    for(int t=0;t<10000;t++)
    {
        int i=g()%20,j=g()%20,op=g()%3;
        if(op==0)
        {
            ll v=int(g()%2001)-1000;
            root[i]=h.insert(root[i],v),s[i].insert(v);
        }
        else if(op==1&&i!=j)
        {
            root[i]=h.merge(root[i],root[j]),root[j]=0;
            s[i].insert(s[j].begin(),s[j].end()),s[j].clear();
        }
        else if(!s[i].empty())
        {
            assert(h.top(root[i])==*s[i].begin());
            root[i]=h.pop(root[i]),s[i].erase(s[i].begin());
        }
        for(int k=0;k<20;k++)
        {
            assert(bool(root[k])==!s[k].empty());
            if(root[k])assert(h.top(root[k])==*s[k].begin());
        }
    }
    vector<bool> seen(h.tr.size());
    auto check=[&](auto &&self,int p)->void
    {
        if(!p)return;
        assert(!seen[p]);seen[p]=true;
        int l=h.tr[p].l,r=h.tr[p].r;
        assert(h.tr[l].d>=h.tr[r].d&&h.tr[p].d==h.tr[r].d+1);
        if(l)assert(h.tr[p].val<=h.tr[l].val);
        if(r)assert(h.tr[p].val<=h.tr[r].val);
        self(self,l),self(self,r);
    };
    for(int i=0;i<20;i++)
    {
        check(check,root[i]);
        while(root[i])
        {
            assert(h.top(root[i])==*s[i].begin());
            root[i]=h.pop(root[i]),s[i].erase(s[i].begin());
        }
        assert(s[i].empty());
    }
    cout<<"LeftistHeap OK\n";
}
