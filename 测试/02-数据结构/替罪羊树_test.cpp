// 替罪羊树 的测试与对拍代码
// 模板本体：02-数据结构/替罪羊树.cpp
#include "../../02-数据结构/替罪羊树.cpp"

int main()
{
    mt19937 rng(712);
    int bad=0,cnt=0;
    Scapegoat st;
    st.insert(3),st.insert(1),st.insert(3);
    int v=0;
    bad+=!st.kth(2,v)||v!=3;
    for(int t=1;t<=20;t++)
    {
        Scapegoat s;
        multiset<int> br;
        for(int q=1;q<=1000;q++)
        {
            int x=(int)(rng()%101)-50,op=rng()%3;
            if(op==0)s.insert(x),br.insert(x);
            if(op==1)
            {
                s.erase(x);
                multiset<int>::iterator it=br.find(x);
                if(it!=br.end())br.erase(it);
            }
            bad+=s.rank(x)!=distance(br.begin(),br.lower_bound(x))+1;
            int k=rng()%(br.size()+2),a=0;
            bool ok=s.kth(k,a),bk=k>=1&&k<=(int)br.size();
            bad+=ok!=bk;
            if(bk) { multiset<int>::iterator it=br.begin(); advance(it,k-1); bad+=a!=*it; }
            multiset<int>::iterator it=br.lower_bound(x);
            ok=s.prev(x,a),bk=it!=br.begin();
            bad+=ok!=bk;
            if(bk)bad+=a!=*--it;
            it=br.upper_bound(x),ok=s.next(x,a),bk=it!=br.end();
            bad+=ok!=bk;
            if(bk)bad+=a!=*it;
            cnt++;
        }
    }
    printf("替罪羊树: %s, 对拍=%d\n",bad?"FAILED":"OK",cnt);
    if(bad)return 1;
    return 0;
}
