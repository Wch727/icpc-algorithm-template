// 可持久化Treap(fhq) 的测试与对拍代码
// 模板本体：02-数据结构/可持久化Treap(fhq).cpp
#include "../../02-数据结构/可持久化Treap(fhq).cpp"

int main()
{
    mt19937 rng(521);
    bool ok=true;
    Treap s;
    int p=0;
    for(int i=1;i<=5;i++)p=s.insert(p,i-1,i);
    int z=s.update(p,2,4,10,true);
    if(s.range_sum(p,1,5)!=15||s.range_sum(z,1,5)!=45||s.range_sum(z,2,2)!=14)ok=false;
    for(int T=1;T<=20;T++)
    {
        Treap tr;
        vector<int> root={0};
        vector<vector<ll>> br(1);
        for(int q=1;q<=250;q++)
        {
            int id=rng()%root.size(),rt=root[id],op=rng()%4;
            vector<ll> a=br[id];
            int n=a.size();
            if(!n||op==0)
            {
                int k=rng()%(n+1);
                ll v=(int)(rng()%101)-50;
                rt=tr.insert(rt,k,v),a.insert(a.begin()+k,v);
            }
            else if(op==1)
            {
                int k=rng()%n;
                rt=tr.erase(rt,k+1),a.erase(a.begin()+k);
            }
            else
            {
                int l=rng()%n,r=rng()%n;
                if(l>r)swap(l,r);
                ll v=op==2?(int)(rng()%21)-10:0;
                rt=tr.update(rt,l+1,r+1,v,op==3);
                for(int i=l;i<=r;i++)a[i]+=v;
                if(op==3)reverse(a.begin()+l,a.begin()+r+1);
            }
            root.push_back(rt),br.push_back(a);
            for(int k=0;k<(int)root.size();k++)
            {
                n=br[k].size();
                if(tr.tr[root[k]].sz!=n)ok=false;
                if(!n)continue;
                int l=rng()%n,r=rng()%n;
                if(l>r)swap(l,r);
                ll sum=0;
                for(int i=l;i<=r;i++)sum+=br[k][i];
                if(tr.range_sum(root[k],l+1,r+1)!=sum)ok=false;
            }
        }
    }
    printf("可持久化Treap 历史版本/区间操作 对拍: %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
