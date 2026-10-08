// 一般图最大总权匹配，O(n³) 时间、O(n²) 空间；允许不匹配，并非先最大匹配数。
// 顶点 1..n，match[u]=0 未匹配；只保留正权，非正权可舍弃。重边取最大值，自环忽略。
// solve 返回 {总权,匹配边数}，对象一次构建后只 solve 一次；边权/双倍顶标远离 1e18。
// 整理自 ShahjalalShohag/code-library/Graph Theory/Blossom Algorithm Weighted.cpp；动态 vector 存储。
// 花收缩维护奇环、花展开恢复交错树；辅助函数承担算法不变量，不能任意合并删除。
#include<bits/stdc++.h>
using namespace std;


// Complexity: O(n^3)
// It finds maximum cost matching on a general graph, not necessarily with maximum matching
// 1-indexed
struct Blossom {
  const long long inf = 1e18;

  struct edge {
    int u, v;
    long long w;
  };
  vector<vector<edge>> g;
  long long reduced(const edge &e) const {return lab[e.u]+lab[e.v]-2*g[e.u][e.v].w;}
  int n,n_x;
  vector<int> match,slack,st,pa,S,vis;vector<vector<int>> flower_from;
  vector<long long> lab;
  vector<vector<int>> flower;
  deque<int> q;
  explicit Blossom(int _n):g(2*_n+1,vector<edge>(2*_n+1)),n(_n),n_x(_n),
      match(2*n+1),slack(2*n+1),st(2*n+1),pa(2*n+1),S(2*n+1),vis(2*n+1),
      flower_from(2*n+1,vector<int>(2*n+1)),lab(2*n+1),flower(2*n+1) {
    q = deque<int>();
    for (int u = 1; u <= n * 2; ++u) {
      match[u] = slack[u] = st[u] = pa[u] = S[u] = vis[u] = lab[u] = 0;
      for (int v = 1; v <= n * 2; ++v) {
        g[u][v] = edge{u, v, 0};
        flower_from[u][v] = 0;
      }
      flower[u].clear();
    }
  }
  void add_edge(int u, int v, long long w) {
    assert(1<=u&&u<=n&&1<=v&&v<=n);if(u==v)return;
    g[u][v].w = max(g[u][v].w, w);
    g[v][u].w = max(g[v][u].w, w);
  }
  inline void update_slack(int u, int x) {
    if (!slack[x] || reduced(g[u][x]) < reduced(g[slack[x]][x])) slack[x] = u;
  }
  inline void set_slack(int x) {
    slack[x] = 0;
    for (int u = 1; u <= n; ++u) {
      if (g[u][x].w > 0 && st[u] != x && S[st[u]] == 0) update_slack(u, x);
    }
  }
  inline void q_push(int x) {
    if (x <= n) return q.push_back(x);
    for (int i = 0; i < (int)flower[x].size(); i++) q_push(flower[x][i]);
  }
  inline void set_st(int x, int b) {
    st[x] = b;
    if (x <= n) return;
    for (int i = 0; i < (int)flower[x].size(); ++i) set_st(flower[x][i], b);
  }
  inline int get_pr(int b, int xr) {
    int pr = find(flower[b].begin(), flower[b].end(), xr) - flower[b].begin();
    if (pr % 2 == 1) {
      reverse(flower[b].begin() + 1, flower[b].end());
      return (int)flower[b].size() - pr;
    } else return pr;
  }
  inline void set_match(int u, int v) {
    match[u] = g[u][v].v;
    if (u <= n) return;
    edge e = g[u][v];
    int xr = flower_from[u][e.u], pr = get_pr(u, xr);
    for (int i = 0; i < pr; ++i) set_match(flower[u][i], flower[u][i ^ 1]);
    set_match(xr, v);
    rotate(flower[u].begin(), flower[u].begin() + pr, flower[u].end());
  }
  inline void augment(int u, int v) {
    int xnv = st[match[u]];
    set_match(u, v);
    if (!xnv) return;
    set_match(xnv, st[pa[xnv]]);
    augment(st[pa[xnv]], xnv);
  }
  inline int get_lca(int u, int v) {
    static int t = 0;
    for (++t; u || v; swap(u, v)) {
      if (u == 0) continue;
      if (vis[u] == t) return u;
      vis[u] = t;
      u = st[match[u]];
      if (u) u = st[pa[u]];
    }
    return 0;
  }
  inline void add_blossom(int u, int lca, int v) {
    int b = n + 1;
    while(b <= n_x && st[b]) ++b;
    if (b > n_x) ++n_x;
    lab[b] = 0, S[b] = 0;
    match[b] = match[lca];
    flower[b].clear();
    flower[b].push_back(lca);
    for (int x = u, y; x != lca; x = st[pa[y]]) {
      flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), q_push(y);
    }
    reverse(flower[b].begin() + 1, flower[b].end());
    for (int x = v, y; x != lca; x = st[pa[y]]) {
      flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), q_push(y);
    }
    set_st(b, b);
    for (int x = 1; x <= n_x; ++x) g[b][x].w = g[x][b].w = 0;
    for (int x = 1; x <= n; ++x) flower_from[b][x] = 0;
    for (int i = 0; i < (int)flower[b].size(); ++i) {
      int xs = flower[b][i];
      for (int x = 1; x <= n_x; ++x) {
        if (g[b][x].w == 0 || reduced(g[xs][x]) < reduced(g[b][x]))
          g[b][x] = g[xs][x], g[x][b] = g[x][xs];
      }
      for (int x = 1; x <= n; ++x) {
        if (flower_from[xs][x]) flower_from[b][x] = xs;
      }
    }
    set_slack(b);
  }
  inline void expand_blossom(int b) { // S[b] == 1
    for (int i = 0; i < (int)flower[b].size(); ++i) set_st(flower[b][i], flower[b][i]);
    int xr = flower_from[b][g[b][pa[b]].u], pr = get_pr(b, xr);
    for (int i = 0; i < pr; i += 2) {
      int xs = flower[b][i], xns = flower[b][i + 1];
      pa[xs] = g[xns][xs].u;
      S[xs] = 1, S[xns] = 0;
      slack[xs] = 0, set_slack(xns);
      q_push(xns);
    }
    S[xr] = 1, pa[xr] = pa[b];
    for (int i = pr + 1; i < (int)flower[b].size(); ++i) {
      int xs = flower[b][i];
      S[xs] = -1, set_slack(xs);
    }
    st[b] = 0;
  }
  inline bool on_found_edge(const edge &e) {
    int u = st[e.u], v = st[e.v];
    if (S[v] == -1) {
      pa[v] = e.u, S[v] = 1;
      int nu = st[match[v]];
      slack[v] = slack[nu] = 0;
      S[nu] = 0, q_push(nu);
    }
    else if(S[v] == 0)
    {
        int lca= get_lca(u, v);
        if(!lca)
            return augment(u, v), augment(v, u), 1;
        else
            add_blossom(u, lca, v);
    }
    return 0;
  }
  inline bool matching() {
    fill(S.begin(),S.end(),-1), fill(slack.begin(),slack.end(),0);
    q.clear();
    for (int x = 1; x <= n_x; ++x) {
      if (st[x] == x && !match[x]) pa[x] = 0, S[x] = 0, q_push(x);
    }
    if (q.empty()) return 0;
    for (;;) {
      while((int)q.size()) {
        int u = q.front();
        q.pop_front();
        if (S[st[u]] == 1)continue;
        for (int v = 1; v <= n; ++v) {
          if (g[u][v].w > 0 && st[u] != st[v]) {
            if (reduced(g[u][v]) == 0) {
              if (on_found_edge(g[u][v])) return 1;
            } else update_slack(u, st[v]);
          }
        }
      }
      long long d = inf;
      for (int b = n + 1; b <= n_x; ++b) {
        if (st[b] == b && S[b] == 1) d = min(d, lab[b] / 2);
      }
      for (int x = 1; x <= n_x; ++x) {
        if (st[x] == x && slack[x]) {
          if (S[x] == -1) d = min(d, reduced(g[slack[x]][x]));
          else if (S[x] == 0) d = min(d, reduced(g[slack[x]][x]) / 2);
        }
      }
      for (int u = 1; u <= n; ++u) {
        if (S[st[u]] == 0) {
          if (lab[u] <= d)return 0;
          lab[u] -= d;
        } else if (S[st[u]] == 1) lab[u] += d;
      }
      for (int b = n + 1; b <= n_x; ++b) {
        if (st[b] == b) {
          if (S[st[b]] == 0) lab[b] += d * 2;
          else if (S[st[b]] == 1) lab[b] -= d * 2;
        }
      }
      q.clear();
      for (int x = 1; x <= n_x; ++x) {
        if (st[x] == x && slack[x] && st[slack[x]] != x && reduced(g[slack[x]][x]) == 0)
          if (on_found_edge(g[slack[x]][x])) return 1;
      }
      for (int b = n + 1; b <= n_x; ++b) {
        if (st[b] == b && S[b] == 1 && lab[b] == 0) expand_blossom(b);
      }
    }
    return 0;
  }
  pair<long long, int> solve() {
    fill(match.begin(),match.end(),0);
    n_x = n;
    int cnt = 0;
    long long ans = 0;
    for (int u = 0; u <= n; ++u) st[u] = u, flower[u].clear();
    long long w_max = 0;
    for (int u = 1; u <= n; ++u) {
      for (int v = 1; v <= n; ++v) {
        flower_from[u][v] = (u == v ? u : 0);
        w_max = max(w_max, g[u][v].w);
      }
    }
    for (int u = 1; u <= n; ++u) lab[u] = w_max;
    while (matching()) ++cnt;
    for (int u = 1; u <= n; ++u) {
      if (match[u] && match[u] < u) ans += g[u][match[u]].w;
    }
    return make_pair(ans, cnt);
  }
};
