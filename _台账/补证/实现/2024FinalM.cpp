#include <bits/stdc++.h>
using namespace std;
namespace final_m {
constexpr int MOD=998244353;
int add(int x,int y){x+=y;if(x>=MOD)x-=MOD;return x;}
int sub(int x,int y){x-=y;if(x<0)x+=MOD;return x;}
struct Block{int a,b,value;};
struct Result{int answer;long long transitions,nonzero;};
Result solve(const vector<int>&a,const vector<int>&b){
    int n=(int)a.size()-1,D=n+2;
    vector<vector<Block>> blocks(D*D);
    vector<int> pref((size_t)D*D*D*D),bad((size_t)D*D*D);
    vector<int> cumulative(D*D),single(D*D),count((size_t)D*D*D),pow2(D,1);
    auto pp=[D](int u,int v,int t,int w){return ((size_t)(u*D+v)*D+t)*D+w;};
    auto gp=[D](int u,int v,int w){return (u*D+v)*D+w;};
    auto cp=[D](int t,int u,int v){return (t*D+u)*D+v;};
    for(int i=1;i<=n;i++)pow2[i]=add(pow2[i-1],pow2[i-1]);
    for(int i=1;i<=n;i++)for(int u=1;u<=n;u++)for(int v=1;v<=n;v++)
        count[cp(i,u,v)]=count[cp(i-1,u,v)]+int(a[i]>=u&&b[i]<=v);
    int answer=0;long long work=0,nonzero=0;
    for(int l=n;l>=1;l--){
        fill(pref.begin(),pref.end(),0);
        for(int r=l;r<=n;r++){
            fill(bad.begin(),bad.end(),0);
            fill(cumulative.begin(),cumulative.end(),0);
            for(int u=1;u<=l;u++)for(int v=r;v<=n;v++){
                int invalid=0;
                for(int k=l+1;k<=r;k++)for(auto z:blocks[k*D+r]){
                    if(z.a<u||z.b>v)continue;
                    ++work;
                    int q=sub(add(pref[pp(u,v,z.a-1,v)],pref[pp(u,v,k-1,k-1)]),
                              pref[pp(u,v,z.a-1,k-1)]);
                    if(!q)continue;
                    int value=(long long)q*z.value%MOD;
                    int &cell=bad[gp(u,v,z.b)];cell=add(cell,value);
                    invalid=add(invalid,value);
                }
                int total=0;
                if(a[l]>=u&&b[l]<=v&&a[r]>=u&&b[r]<=v){
                    int optional=r==l?0:count[cp(r-1,u,v)]-count[cp(l,u,v)];
                    total=pow2[optional];
                }
                cumulative[u*D+v]=sub(total,invalid);
            }
            auto &now=blocks[l*D+r];
            fill(single.begin(),single.end(),0);
            for(int u=1;u<=l;u++)for(int v=r;v<=n;v++){
                int value=sub(sub(add(cumulative[u*D+v],cumulative[(u+1)*D+v-1]),
                                  cumulative[(u+1)*D+v]),cumulative[u*D+v-1]);
                if(value){now.push_back({u,v,value});++nonzero;}
                single[u*D+v]=value;answer=add(answer,value);
            }
            for(int u=l-1;u>=1;u--)for(int v=r;v<=n;v++)
                single[u*D+v]=add(single[u*D+v],single[(u+1)*D+v]);
            for(int u=1;u<=l;u++)for(int v=r;v<=n;v++){
                int running=0;
                for(int w=0;w<=v;w++){
                    if(w>=r)running=add(running,add(bad[gp(u,v,w)],single[u*D+w]));
                    pref[pp(u,v,r,w)]=add(pref[pp(u,v,r-1,w)],running);
                }
            }
        }
    }
    return {answer,work,nonzero};
}
}

#ifndef FINAL_M_NO_MAIN
int main(){
    ios::sync_with_stdio(false);cin.tie(nullptr);
    int n;if(!(cin>>n))return 0;
    vector<int>a(n+1),b(n+1);
    for(int i=1;i<=n;i++)cin>>a[i]>>b[i];
    cout<<final_m::solve(a,b).answer<<'\n';
}
#endif
