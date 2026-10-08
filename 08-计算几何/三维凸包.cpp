// 增量三维凸包，O(n²)；返回朝外三角面、维数、表面积、体积。允许重复/共面点。
// 低维退化：dim=0/1 面为空，dim=2 返回平面凸包的一份三角剖分，area 是单面面积、volume=0。
// 用固定初始四面体内部点定向；EPS 是尺度相关容差，浮点接近退化须按题调整。
#include<bits/stdc++.h>
using namespace std;
struct Point3
{
    long double x, y, z;
    Point3 operator+(Point3 b) const{return {x + b.x, y + b.y, z + b.z};}
    Point3 operator-(Point3 b) const{return {x - b.x, y - b.y, z - b.z};}
    Point3 operator*(long double t) const{return {x * t, y * t, z * t};}
};
Point3 cross(Point3 a,Point3 b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
long double dot(Point3 a,Point3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
long double norm3(Point3 a){return sqrtl(dot(a,a));}
struct Hull3D{int dim=0;vector<array<int,3>> faces;long double area=0,volume=0;};
Hull3D convex_hull_3d(const vector<Point3> &p,long double eps=1e-10L)
{
    Hull3D out;int n=p.size();if(n<2)return out;
    int a= 0, b= 0, c= 0, d= 0;
    for(int i= 1; i < n; i++)
        if(norm3(p[i] - p[a]) > norm3(p[b] - p[a]))
            b= i;
    if(norm3(p[b]-p[a])<=eps)return out;out.dim=1;
    for(int i= 0; i < n; i++)
        if(norm3(cross(p[b] - p[a], p[i] - p[a])) >
           norm3(cross(p[b] - p[a], p[c] - p[a])))
            c= i;
    Point3 normal=cross(p[b]-p[a],p[c]-p[a]);if(norm3(normal)<=eps)return out;out.dim=2;
    normal= normal * (1 / norm3(normal));
    for(int i= 0; i < n; i++)
        if(fabsl(dot(normal, p[i] - p[a])) > fabsl(dot(normal, p[d] - p[a])))
            d= i;
    if(fabsl(dot(normal,p[d]-p[a]))<=eps)
    {
        Point3 ex=(p[b]-p[a])*(1/norm3(p[b]-p[a])),ey=cross(normal,ex);vector<tuple<long double,long double,int>> v;
        for(int i= 0; i < n; i++)
            v.push_back({dot(p[i] - p[a], ex), dot(p[i] - p[a], ey), i});
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end(),
                       [&](auto x, auto y)
                       {
                           return fabsl(get<0>(x) - get<0>(y)) <= eps &&
                                  fabsl(get<1>(x) - get<1>(y)) <= eps;
                       }),
                v.end());
        auto turn= [&](int i, int j, int k)
        {
            auto [x, y, id]= v[i];
            auto [X, Y, jd]= v[j];
            auto [u, w, kd]= v[k];
            return (X - x) * (w - y) - (Y - y) * (u - x);
        };
        vector<int> h;
        for(int i= 0; i < (int)v.size(); i++)
        {
            while(h.size() > 1 && turn(h[h.size() - 2], h.back(), i) <= eps)
                h.pop_back();
            h.push_back(i);
        }
        int lower= h.size();
        for(int i= (int)v.size() - 2; i >= 0; i--)
        {
            while((int)h.size() > lower &&
                  turn(h[h.size() - 2], h.back(), i) <= eps)
                h.pop_back();
            h.push_back(i);
        }
        h.pop_back();
        for(int i= 1; i + 1 < (int)h.size(); i++)
            out.faces.push_back(
                {get<2>(v[h[0]]), get<2>(v[h[i]]), get<2>(v[h[i + 1]])});
    }
    else
    {
        out.dim=3;Point3 inner=(p[a]+p[b]+p[c]+p[d])*.25L;
        auto face= [&](int u, int v, int w)
        {
            if(dot(cross(p[v] - p[u], p[w] - p[u]), inner - p[u]) > 0)
                swap(v, w);
            return array<int, 3>{u, v, w};
        };
        out.faces={face(a,b,c),face(a,b,d),face(a,c,d),face(b,c,d)};
        for(int i= 0; i < n; i++)
            if(i != a && i != b && i != c && i != d)
            {
                vector<array<int, 3>> keep;
                map<pair<int, int>, int> horizon;
                for(auto f : out.faces)
                {
                    auto [u, v, w]= f;
                    Point3 no= cross(p[v] - p[u], p[w] - p[u]);
                    if(dot(no, p[i] - p[u]) > eps * norm3(no))
                    {
                        for(auto [x, y] : {pair(u, v), pair(v, w), pair(w, u)})
                        {
                            auto it= horizon.find({y, x});
                            if(it != horizon.end())
                                horizon.erase(it);
                            else
                                horizon[{x, y}]= 1;
                        }
                    }
                    else
                        keep.push_back(f);
                }
                for(auto [edge, count] : horizon)
                    keep.push_back(face(edge.first, edge.second, i));
                out.faces.swap(keep);
            }
    }
    for(auto [u, v, w] : out.faces)
    {
        out.area+= norm3(cross(p[v] - p[u], p[w] - p[u])) / 2;
        if(out.dim == 3)
            out.volume+= dot(p[u] - p[a], cross(p[v] - p[a], p[w] - p[a])) / 6;
    }
    out.volume=fabsl(out.volume);return out;
}
