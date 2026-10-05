#include "../../01-基础与技巧/反悔贪心.cpp"
int task_schedule(vector<Task> input){tasks=move(input);return task_schedule();}

int main()
{
    mt19937 rng(20261001);
    assert(task_schedule({{4,4},{2,5},{3,6}})==2);
    assert(task_schedule({})==0);
    assert(task_schedule({{10,0},{10,3}})==0);
    for(int z=0;z<1000;z++)
    {
        int n=rng()%10+1;
        vector<Task> tasks;
        for(int i=0;i<n;i++)tasks.push_back({(ll)(rng()%10+1),(ll)(rng()%25)});
        int want=0;
        for(int mask=0;mask<(1<<n);mask++)
        {
            vector<Task> selected;
            for(int i=0;i<n;i++)if(mask>>i&1)selected.push_back(tasks[i]);
            sort(selected.begin(),selected.end(),[](const Task &a,const Task &b){return a.d<b.d;});
            ll time=0;
            bool ok=true;
            for(Task x:selected)
            {
                time+=x.t;
                if(time>x.d)ok=false;
            }
            if(ok)want=max(want,(int)selected.size());
        }
        assert(task_schedule(tasks)==want);
    }
    puts("不同耗时调度与子集枚举对拍：OK");
}
