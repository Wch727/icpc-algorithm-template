#include "../../03-字符串/串联重复分组枚举.cpp"
int main(){mt19937 r(157);for(int t=0;t<400;t++){string s;for(int n=r()%55;n--;)s+=char('a'+r()%3);set<pair<int,int>> want,got;for(int p=1;2*p<=(int)s.size();p++)for(int i=0;i+2*p<=(int)s.size();i++)if(s.substr(i,p)==s.substr(i+p,p))want.insert({p,i});for(auto [p,l,h]:tandem_repeats(s))for(int i=l;i<=h;i++)assert(got.insert({p,i}).second);assert(got==want);}}
