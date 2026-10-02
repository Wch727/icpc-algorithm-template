#include "../../08-计算几何/三维几何基础.cpp"
#include "../../01-基础与技巧/位运算技巧.cpp"
int main()
{
    Point p={2,3,5};
    auto x=rotate_axis(p,0,1),y=rotate_axis(p,1,1),z=rotate_axis(p,2,1);
    assert(x.x==2&&x.y==-5&&x.z==3);
    assert(y.x==5&&y.y==3&&y.z==-2);
    assert(z.x==-3&&z.y==2&&z.z==5);
    for(int axis=0;axis<3;axis++)for(int k=-12;k<=12;k++)
    {
        Point a=rotate_axis(p,axis,k),b=rotate_axis(a,axis,-k);
        assert(len(b-p)==0&&len(a)==len(p));
    }
    for(int k=0;k<=8;k++)for(ull j=0;j<12;j++)for(ull v=0;v<160;v++)
    {
        ull length=1ULL<<k,b=j*length;unsigned __int128 brute=0;
        for(ull i=0;i<length;i++)brute+=(b+i)^v;
        assert(xor_block_sum(b,length,v)==brute);
    }
    ull length=1ULL<<63;
    assert(xor_block_sum(0,length,ULLONG_MAX)==(unsigned __int128)length*ULLONG_MAX-(unsigned __int128)length*(length-1)/2);
    assert(xor_block_sum(length,length,0)==(unsigned __int128)length*length+(unsigned __int128)length*(length-1)/2);
    cout<<"PASS: axis rotation, inverse rotation, XOR blocks and 128-bit totals\n";
}
