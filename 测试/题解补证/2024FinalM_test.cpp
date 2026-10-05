#define FINAL_M_NO_MAIN
#include "../../_台账/补证/实现/2024FinalM.cpp"

// Independent oracle: directly test strong connectivity of every induced subset.
int brute(const vector<int>&a,const vector<int>&b){
    int n=(int)a.size()-1,answer=0;
    for(int mask=1;mask<(1<<n);mask++){
        int source=__builtin_ctz((unsigned)mask);bool good=true;
        for(int rev=0;rev<2;rev++){
            int seen=1<<source;vector<int>q{source};
            for(size_t i=0;i<q.size();i++)for(int v=0;v<n;v++)if((mask>>v&1)&&!(seen>>v&1)){
                int u=q[i];
                bool edge=rev?(a[v+1]<=u+1&&u+1<=b[v+1]):(a[u+1]<=v+1&&v+1<=b[u+1]);
                if(edge){seen|=1<<v;q.push_back(v);}
            }
            if(seen!=mask)good=false;
        }
        answer+=good;
    }
    return answer;
}
void check(const vector<int>&a,const vector<int>&b,int expected=-1){
    int got=final_m::solve(a,b).answer,want=expected<0?brute(a,b):expected;
    if(got!=want){
        cerr<<"FAIL n="<<a.size()-1<<" got="<<got<<" want="<<want<<'\n';
        for(size_t i=1;i<a.size();i++)cerr<<a[i]<<' '<<b[i]<<'\n';
        exit(1);
    }
}
int main(int argc,char**argv){
    if(argc>1&&string(argv[1])=="--solve"){
        int n;cin>>n;vector<int>a(n+1),b(n+1);for(int i=1;i<=n;i++)cin>>a[i]>>b[i];
        cout<<final_m::solve(a,b).answer<<'\n';return 0;
    }
    check({0,1,2,3,2,4},{0,2,5,4,5,5},12);
    for(int n=1;n<=4;n++){
        vector<int>a(n+1),b(n+1);int cases=0;
        function<void(int)> dfs=[&](int i){
            if(i>n){check(a,b);++cases;return;}
            for(a[i]=1;a[i]<=i;a[i]++)for(b[i]=i;b[i]<=n;b[i]++)dfs(i+1);
        };dfs(1);cout<<"all interval graphs n="<<n<<" cases="<<cases<<" PASS\n";
    }
    mt19937 rng(20241024);
    for(int n=5;n<=10;n++)for(int t=0;t<12;t++){
        vector<int>a(n+1),b(n+1);
        for(int i=1;i<=n;i++){a[i]=1+rng()%i;b[i]=i+rng()%(n-i+1);}check(a,b);
    }
    cout<<"independent induced-subset SCC checks PASS\n";
    for(int mode=0;mode<7;mode++){
        int n=50;vector<int>a(n+1),b(n+1);
        for(int i=1;i<=n;i++){
            if(mode==0)a[i]=b[i]=i;
            else if(mode==1)a[i]=1,b[i]=n;
            else if(mode==2)a[i]=i,b[i]=n;
            else if(mode==3)a[i]=(i-1)/10*10+1,b[i]=a[i]+9;
            else {a[i]=1+rng()%i;b[i]=i+rng()%(n-i+1);}
        }
        auto begin=chrono::steady_clock::now();auto result=final_m::solve(a,b);
        double sec=chrono::duration<double>(chrono::steady_clock::now()-begin).count();
        if(mode==0||mode==2)check(a,b,50);
        if(mode==1){int p=1;for(int i=0;i<n;i++)p=final_m::add(p,p);check(a,b,final_m::sub(p,1));}
        if(mode==3)check(a,b,5*((1<<10)-1));
        cout<<"n50 mode="<<mode<<" answer="<<result.answer<<" seconds="<<fixed<<setprecision(4)<<sec
            <<" transitions="<<result.transitions<<" nonzero="<<result.nonzero<<'\n';
    }
}
