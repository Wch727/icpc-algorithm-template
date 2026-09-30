#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;
string s;

// 回文自动机：结点 0 是偶根 len=0，结点 1 是奇根 len=-1，奇根的 link 指向自己
// nxt 转移、link 后缀回文链、len 该回文串长度、siz 该回文串出现次数
int tot,last;
int len[N],link[N],siz[N],cnt[N],id[N];
int nxt[N][26];
char str[N];// 1-indexed 原串，str[0] 放用不到的哨兵 '#'
char pstr[N][27];// 便于对拍的缓存：该结点代表的回文串内容（只存长度 <=26 的）
int plen[N];

// 清空 PAM，多组数据用
void pam_init()
{
    tot=1,last=0;
    len[0]=0,link[0]=1;
    len[1]=-1,link[1]=1;
    siz[0]=siz[1]=0,plen[0]=plen[1]=0;
    for(int j=0;j<26;j++)nxt[0][j]=nxt[1][j]=0;
}

// 从 p 出发沿 link 跳，找到能接上位置 i 的最长回文后缀
int get_fail(int p,int i)
{
    while(str[i-len[p]-1]!=str[i])p=link[p];
    return p;
}

// 在末尾加入第 i 个字符（1-indexed，str[i] 已赋值）
void pam_extend(int i)
{
    int c=str[i]-'a';
    int p=get_fail(last,i);
    if(!nxt[p][c])
    {
        int cur=++tot;
        len[cur]=len[p]+2,siz[cur]=1;
        for(int j=0;j<26;j++)nxt[cur][j]=0;
        if(len[cur]==1)link[cur]=0;// 单字符回文的后缀链接是偶根
        else link[cur]=nxt[get_fail(link[p],i)][c];
        nxt[p][c]=cur;
        if(len[cur]<=26)// 缓存串内容，仅用于自测
        {
            plen[cur]=len[cur];
            for(int k=0;k<len[cur];k++)pstr[cur][k]=str[i-len[cur]+1+k];
            pstr[cur][len[cur]]=0;
        }
    }
    else siz[nxt[p][c]]++;// 该回文串已经存在，出现次数 +1
    last=nxt[p][c];
}

// 按 len 基数排序得到拓扑序，再沿 link 累加出现次数
// 注意：len[i]+1 的取值范围是 0..n+1（奇根 -1 与最长回文 n），桶要开到 n+2
void pam_build()
{
    int mx=n+2;
    for(int i=0;i<=mx;i++)cnt[i]=0;
    for(int i=0;i<=tot;i++)cnt[len[i]+1]++;
    for(int i=1;i<=mx;i++)cnt[i]+=cnt[i-1];
    for(int i=tot;i>=0;i--)id[cnt[len[i]+1]--]=i;
    for(int i=tot;i>=0;i--)
    {
        int u=id[i];
        if(u>=2)siz[link[u]]+=siz[u];// 只有真结点（非两个根）才向下累加
    }
}

// 本质不同回文子串个数：去掉两个根
int count_pal()
{
    return tot-1;
}

// 最长回文子串长度：所有结点 len 的最大值
int longest_pal()
{
    int ans=0;
    for(int i=2;i<=tot;i++)ans=max(ans,len[i]);
    return ans;
}

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);// 只用 a,b 制造大量回文
    return r;
}

int main()
{
    srand(12345);

    // 基础自测：例子 abacaba，本质不同回文子串 a,b,c,aba,aca,bacab,abacaba 共 7 个
    string base="abacaba";
    n=base.length();
    pam_init();
    str[0]='#';
    for(int i=1;i<=n;i++)str[i]=base[i-1],pam_extend(i);
    pam_build();
    printf("s=%s  diff_pal=%d  longest_pal=%d\n",base.c_str(),count_pal(),longest_pal());
    for(int i=2;i<=tot&&i<=10;i++)printf("  node %d: %s len=%d occur=%d link=%d\n",i,pstr[i],len[i],siz[i],link[i]);

    int bad=0;
    for(int rd=1;rd<=800;rd++)
    {
        int ls=rand()%14+1;
        string a=rand_str(ls);
        n=ls;
        pam_init();
        str[0]='#';
        for(int i=1;i<=ls;i++)str[i]=a[i-1],pam_extend(i);
        pam_build();

        // 暴力：枚举所有子串，筛出回文的并统计出现次数
        map<string,int> want;
        for(int i=0;i<ls;i++)
            for(int j=i;j<ls;j++)
            {
                string cur=a.substr(i,j-i+1),rv=cur;
                reverse(rv.begin(),rv.end());
                if(cur==rv)want[cur]++;
            }
        if(count_pal()!=(int)want.size()){bad++;if(bad<=3)printf("diff_pal mismatch a=%s got=%d want=%d\n",a.c_str(),count_pal(),(int)want.size());}

        int mx=0;
        for(map<string,int>::iterator it=want.begin();it!=want.end();++it)mx=max(mx,(int)it->first.length());
        if(longest_pal()!=mx){bad++;if(bad<=3)printf("longest mismatch a=%s got=%d want=%d\n",a.c_str(),longest_pal(),mx);}

        for(int i=2;i<=tot;i++)
        {
            string cur=pstr[i];
            string rv=cur;
            reverse(rv.begin(),rv.end());
            // 校验 1：结点存的是回文，且出现次数与暴力一致
            if(cur!=rv||want.find(cur)==want.end()||want[cur]!=siz[i])
            {
                bad++;
                if(bad<=3)printf("occur mismatch a=%s cur=%s got=%d want=%d\n",a.c_str(),cur.c_str(),siz[i],want.count(cur)?want[cur]:-1);
            }
            // 校验 2：同一长度下结点互不相同（本质不同）
            for(int j=2;j<i;j++)
                if(len[j]==len[i]&&strcmp(pstr[j],pstr[i])==0)
                {
                    bad++;
                    if(bad<=3)printf("dup node a=%s cur=%s\n",a.c_str(),cur.c_str());
                }
            // 校验 3：link 的串是自己的最长回文真后缀
            if(link[i]>=0&&len[link[i]]>0)
            {
                string lk=pstr[link[i]];
                if(len[link[i]]>=len[i]||cur.compare(len[i]-len[link[i]],len[link[i]],lk)!=0)
                {
                    bad++;
                    if(bad<=3)printf("link content bad a=%s node=%d cur=%s link=%s\n",a.c_str(),i,cur.c_str(),lk.c_str());
                }
            }
        }
    }
    printf("random PAM bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
