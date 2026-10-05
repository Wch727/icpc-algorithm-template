#include <bits/stdc++.h>
namespace guangzhou_digits {
constexpr int mod=1000000009;
int add(int a,int b){ a+=b; return a>=mod?a-mod:a; }
int sub(int a,int b){ a-=b; return a<0?a+mod:a; }
// 残串左右两种方向；相同数值、不同长度（例如 0/00）必须区分。
struct Piece { std::string s,t; int base; std::vector<int> ps,pt; };
struct Residual { std::string p; std::vector<int> tail; int key; };
int solve(const std::vector<int>& a){
    int n=a.size(),N=n+2;
    std::vector<int> pre(n+1);
    for(int i=0;i<n;i++) pre[i+1]=pre[i]+a[i];
    std::vector<std::vector<int>> id(n,std::vector<int>(n));
    std::vector<Piece> pieces; int states=0;
    std::vector<std::unordered_map<int,std::pair<int,int>>> left(n),right(n);
    for(int i=0;i<n;i++) for(int j=i;j<n;j++){
        std::string s=std::to_string(pre[j+1]-pre[i]),t=s;
        std::reverse(t.begin(),t.end());
        id[i][j]=pieces.size(); pieces.push_back({s,t,states});
        states+=s.size();
        int sum=pre[j+1]-pre[i];
        auto [x,newL]=left[i].emplace(sum,std::make_pair(j+1,j+1));
        if(!newL) x->second.second=j+1;
        auto [y,newR]=right[j].emplace(sum,std::make_pair(i,i));
        if(!newR) y->second.second=i;
    }
    std::vector<std::vector<int>> indexL(pieces.size()),indexR(pieces.size());
    std::vector<std::vector<int>> boundaryL(N),boundaryR(N);
    std::vector<std::unordered_map<std::string,int>> mapL(N),mapR(N);
    std::vector<Residual> residualL,residualR;
    auto intern=[](auto& maps,auto& boundaries,auto& residual,int b,std::string p){
        auto it=maps[b].find(p);
        if(it!=maps[b].end()) return it->second;
        int z=residual.size(); maps[b][p]=z; boundaries[b].push_back(z);
        residual.push_back({p,{}}); return z;
    };
    for(int i=0;i<n;i++) for(int j=i;j<n;j++){
        int z=id[i][j],d=pieces[z].s.size();
        indexL[z].resize(d+1);indexR[z].resize(d+1);
        for(int k=1;k<=d;k++){
            indexL[z][k]=intern(mapL,boundaryL,residualL,j+1,pieces[z].s.substr(d-k));
            indexR[z][k]=intern(mapR,boundaryR,residualR,i,pieces[z].t.substr(d-k));
        }
    }
    auto tails=[](auto& maps,auto& boundaries,auto& residual){
        for(int b=0;b<(int)boundaries.size();b++) for(int z:boundaries[b]){
            auto &s=residual[z];int k=s.p.size();s.tail.resize(k+1);
            for(int h=1;h<=k;h++) s.tail[h]=maps[b].at(s.p.substr(k-h));
        }
    };
    tails(mapL,boundaryL,residualL);tails(mapR,boundaryR,residualR);
    std::vector<int> L((size_t)residualL.size()*N),R((size_t)residualR.size()*N);
    auto row=[&](std::vector<int>& v,int z,int k){
        int slot=&v==&L?indexL[z][k]:indexR[z][k];
        return v.data()+(size_t)slot*N;
    };
    auto palindrome=[](const std::string& s,int k){
        int p=s.size()-k;
        for(int x=0;x<k/2;x++) if(s[p+x]!=s[p+k-1-x]) return 0;
        return 1;
    };
    for(int i=0;i<n;i++) for(int j=i;j<n;j++){
        int z=id[i][j];
        for(int k=1;k<=(int)pieces[z].s.size();k++){
            row(L,z,k)[j+1]=palindrome(pieces[z].s,k);
            row(R,z,k)[i]=palindrome(pieces[z].t,k);
        }
    }
    std::vector<std::vector<int>> E(N,std::vector<int>(N)),EL=E,ER=E;
    for(int i=0;i<=n;i++) E[i][i]=EL[i][i]=ER[i][i]=1;
    auto getL=[&](int i,int l,int r,int k){
        int* p=row(L,id[i][l-1],k);
        return r<l?p[l]:sub(p[r+1],p[r]);
    };
    auto getR=[&](int l,int r,int j,int k){
        int* p=row(R,id[r+1][j],k);
        return l>r?p[r+1]:sub(p[l],p[l+1]);
    };
    auto key=[](const std::string& s,int len){
        long long v=0; for(int i=0;i<len;i++) v=v*10+s[i]-'0';
        return (long long)len*1000000000+v;
    };
    std::unordered_map<long long,int> keys;
    auto internKey=[&](long long v){
        auto [it,fresh]=keys.emplace(v,keys.size());return it->second;
    };
    for(auto &p:pieces){
        int d=p.s.size();p.ps.resize(d+1);p.pt.resize(d+1);
        for(int h=1;h<=d;h++){
            p.ps[h]=internKey(key(p.s,h));p.pt[h]=internKey(key(p.t,h));
        }
    }
    for(auto &s:residualL)s.key=internKey(key(s.p,s.p.size()));
    for(auto &s:residualR)s.key=internKey(key(s.p,s.p.size()));
    std::vector<int> BL(keys.size()),BR(keys.size()),stampL(keys.size()),stampR(keys.size());
    int epoch=0;
    auto bucketAdd=[&](auto& vals,auto& stamps,int key,int v){
        if(stamps[key]!=epoch)stamps[key]=epoch,vals[key]=0;
        vals[key]=add(vals[key],v);
    };
    for(int len=1;len<=n;len++) for(int l=0;l+len<=n;l++){
        int r=l+len-1;
        ++epoch;
        int empty=0;
        for(int j=l;j<=r;j++){
            int z=id[l][j],d=pieces[z].s.size();
            empty=add(empty,getL(l,j+1,r,d));
            for(int h=1;h<d;h++){
                bucketAdd(BL,stampL,pieces[z].ps[h],getL(l,j+1,r,d-h));
            }
            z=id[j][r]; d=pieces[z].t.size();
            for(int h=1;h<d;h++){
                bucketAdd(BR,stampR,pieces[z].pt[h],getR(l,j-1,r,d-h));
            }
        }
        E[l][r+1]=empty;
        EL[l][r+1]=add(EL[l][r],empty);
        ER[r+1][l]=add(ER[r+1][l+1],empty);
        for(int slot:boundaryL[l]){
                const auto &state=residualL[slot]; const std::string &p=state.p;
                int k=p.size();
                int val=stampR[state.key]==epoch?BR[state.key]:0;
                int target=0,power=1;
                for(int h=1;h<=k;h++){
                    target+=(p[h-1]-'0')*power; power*=10;
                    if(h>1 && p[h-1]=='0') continue;
                    if(target<a[r] || target>pre[r+1]-pre[l]) continue;
                    auto range=right[r].find(target);
                    if(range==right[r].end()) continue;
                    int lo=std::max(l,range->second.first),hi=range->second.second;
                    if(lo>hi) continue;
                    int* q=k==h?EL[l].data():L.data()+(size_t)state.tail[k-h]*N;
                    val=add(val,sub(q[hi],lo?q[lo-1]:0));
                }
                int* q=L.data()+(size_t)slot*N; q[r+1]=add(q[r],val);
        }
        for(int slot:boundaryR[r+1]){
                const auto &state=residualR[slot]; const std::string &p=state.p;
                int k=p.size();
                int val=stampL[state.key]==epoch?BL[state.key]:0;
                int target=0;
                for(int h=1;h<=k;h++){
                    target=target*10+p[h-1]-'0';
                    if(h>1 && p[0]=='0') continue;
                    if(target<a[l] || target>pre[r+1]-pre[l]) continue;
                    auto range=left[l].find(target);
                    if(range==left[l].end()) continue;
                    int lo=range->second.first,hi=std::min(r+1,range->second.second);
                    if(lo>hi) continue;
                    int* q=k==h?ER[r+1].data():R.data()+(size_t)state.tail[k-h]*N;
                    val=add(val,sub(q[lo],q[hi+1]));
                }
                int* q=R.data()+(size_t)slot*N; q[l]=add(q[l+1],val);
        }
    }
    return E[0][n];
}
}
#ifndef GUANGZHOU_DIGITS_NO_MAIN
int main(){
    std::ios::sync_with_stdio(false);std::cin.tie(nullptr);
    int t;std::cin>>t;
    while(t--){int n;std::cin>>n;std::vector<int>a(n);
        for(int &x:a)std::cin>>x;
        std::cout<<guangzhou_digits::solve(a)<<'\n';}
}
#endif
