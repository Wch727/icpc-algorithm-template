// 回文自动机PAM 的测试与对拍代码
// 模板本体：03-字符串/回文自动机PAM.cpp
#include "../../03-字符串/回文自动机PAM.cpp"

string node_string(int u){return len[u]>0?string(str+pos[u]-len[u]+1,len[u]):string();}

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
    for(int i=2;i<=tot&&i<=10;i++)printf("  node %d: %s len=%d occur=%d link=%d\n",i,node_string(i).c_str(),len[i],siz[i],link[i]);

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
            string cur=node_string(i);
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
                if(len[j]==len[i]&&node_string(j)==node_string(i))
                {
                    bad++;
                    if(bad<=3)printf("dup node a=%s cur=%s\n",a.c_str(),cur.c_str());
                }
            // 校验 3：link 的串是自己的最长回文真后缀
            if(link[i]>=0&&len[link[i]]>0)
            {
                string lk=node_string(link[i]);
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
