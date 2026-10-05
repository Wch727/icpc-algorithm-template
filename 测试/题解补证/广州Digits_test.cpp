#define GUANGZHOU_DIGITS_NO_MAIN
#include "../../_台账/补证/实现/广州Digits.cpp"
using namespace std;
int brute(const vector<int>& a){
    int ans=0,n=a.size();
    for(int mask=0;mask<(1<<(n-1));mask++){
        string s; int sum=0;
        for(int i=0;i<n;i++){
            sum+=a[i];
            if(i==n-1 || (mask>>i&1)){s+=to_string(sum);sum=0;}
        }
        string t=s; reverse(t.begin(),t.end()); ans+=s==t;
    }
    return ans;
}
int main(int argc,char**argv){
    if(argc>1 && std::string(argv[1])=="--solve"){
        int t;std::cin>>t;
        while(t--){int n;std::cin>>n;std::vector<int>a(n);for(int &x:a)std::cin>>x;
            std::cout<<guangzhou_digits::solve(a)<<'\n';}
        return 0;
    }
    int cases=0;
    for(int n=1;n<=6;n++){
        int total=1; for(int i=0;i<n;i++) total*=3;
        for(int mask=0;mask<total;mask++){
            vector<int>a(n); int x=mask;
            for(int &v:a){v=x%3;x/=3;}
            assert(guangzhou_digits::solve(a)==brute(a)); cases++;
        }
    }
    vector<vector<int>> edge={{123456,654321},{10,0,1},{100,1,0,0},{0},{666666},
        {1,1,1,1,1,1},{1,1,4,5,1,4,1},{1,2,3,1,1,1,2,1,1}};
    mt19937 rng(20261005);
    for(int n=1;n<=12;n++){
        vector<int>a(n); for(int &v:a) v=rng()%666667;
        edge.push_back(a);
    }
    int alphabet[]={0,1,10,11,100,101,100001,666666};
    for(int t=0;t<20;t++){
        vector<int>a(8);for(int &v:a)v=alphabet[rng()%8];edge.push_back(a);
    }
    for(auto a:edge){assert(guangzhou_digits::solve(a)==brute(a)); cases++;}
    cout<<"independent partitions PASS "<<cases<<"\n";
    for(int type=0;type<6;type++){
        vector<int>a(150);
        for(int i=0;i<150;i++) a[i]=type==0?0:type==1?1:type==2?666666:
            type==3?rng()%666667:type==4?(i%2?100001:0):111111;
        auto start=chrono::steady_clock::now(); int ans=guangzhou_digits::solve(a);
        if(type==0){int p=1;for(int i=1;i<150;i++)p=guangzhou_digits::add(p,p);assert(ans==p);}
        cout<<"n150 type "<<type<<" answer "<<ans<<" seconds "
            <<chrono::duration<double>(chrono::steady_clock::now()-start).count()<<"\n";
    }
}
