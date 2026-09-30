#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 各方案择一使用，复杂度均为 O(输入/输出字节数)。
// cin/cout：默认首选；在任何 I/O 前调用。关闭同步后不要混用 C I/O。
// cin.tie(nullptr) 后交互题必须显式 cout << flush，不能依赖自动刷新。
inline void fast_cin() { ios::sync_with_stdio(false); cin.tie(nullptr); }

// 整数解析器：输入须为由空白分隔、在 long long 范围内的十进制整数。
// 返回 false 表示 EOF/非法 token/溢出；失败时不修改答案。允许前导 + 和 -。
// 会消费末尾的一个分隔符；不要用于逗号等复杂格式，也不要交替读同一 FILE。
template<class Next>
bool read_integer(Next next, ll& value)
{
    int c;
    do { c = next(); } while(c != EOF && isspace((unsigned char)c));
    if(c == EOF) return false;
    bool neg = c == '-';
    if(c == '-' || c == '+') c = next();
    unsigned long long x = 0;
    const auto limit = (unsigned long long)LLONG_MAX + (neg ? 1ULL : 0ULL);
    bool digit = false, valid = true;
    while(c != EOF && !isspace((unsigned char)c))
    {
        if(c < '0' || c > '9') valid = false;
        else
        {
            digit = true;
            unsigned d = c - '0';
            if(x > (limit - d) / 10) valid = false;
            else x = x * 10 + d;
        }
        c = next();
    }
    if(!digit || !valid) return false;
    value = neg ? (x == (1ULL << 63) ? LLONG_MIN : -(ll)x) : (ll)x;
    return true;
}

// getchar：短小、无需自建缓冲；默认走 getchar()，测试时可传临时文件。
// Windows 单线程追求速度时可把 getchar() 换成 _getchar_nolock()；不可移植，
// 且不提供线程锁保护。返回 int 而非 char，才能可靠区分 EOF 与字节。
struct GetcharReader
{
    FILE* file;
    explicit GetcharReader(FILE* f = stdin) : file(f) {}
    int next() { return file == stdin ? getchar() : fgetc(file); }
    bool read(ll& x) { return read_integer([this] { return next(); }, x); }
};

// fread：保留 1 MiB 整块输入缓冲；文件重定向、大量整数时通常才有优势。
// 交互题不适合大块预读。换文件后必须新建 reader；不要混用 cin/scanf/getchar。
constexpr int BUF = 1 << 20;
struct FreadReader
{
    FILE* file;
    array<unsigned char, BUF> buffer{};
    size_t pos = 0, end = 0;
    explicit FreadReader(FILE* f = stdin) : file(f) {}
    int next()
    {
        if(pos == end)
        {
            end = fread(buffer.data(), 1, buffer.size(), file);
            pos = 0;
            if(end == 0) return EOF;
        }
        return buffer[pos++];
    }
    bool read(ll& x) { return read_integer([this] { return next(); }, x); }
    bool read_string(string& s)
    {
        int c;
        do { c = next(); } while(c != EOF && isspace((unsigned char)c));
        if(c == EOF) return false;
        s.clear();
        while(c != EOF && !isspace((unsigned char)c))
        {
            s.push_back((char)c);
            c = next();
        }
        return true;
    }
    // 浮点快读：先缓冲取 token 再 strtod，支持小数、符号、e/E 指数。
    // 比手写小数解析稳，但转换仍有成本；按 C locale 使用小数点 '.'。
    // 检查完整 token、范围错误和非有限值；失败不修改答案。
    bool read_double(double& x)
    {
        string s;
        if(!read_string(s)) return false;
        char* tail;
        errno = 0;
        double v = strtod(s.c_str(), &tail);
        if(tail == s.c_str() || *tail || errno == ERANGE || !isfinite(v)) return false;
        x = v;
        return true;
    }
};

// scanf/printf：浮点、固定分隔符或复杂格式时方便而稳，务必检查返回值。
// long long 用 %lld；scanf 的 double* 用 %lf，printf 的 double 用 %f。
// 数值仍须在目标类型范围内；格式字符串不匹配指针类型会产生未定义行为。
inline bool scanf_long(ll& x, FILE* f = stdin) { return fscanf(f, "%lld", &x) == 1; }
inline bool scanf_double(double& x, FILE* f = stdin) { return fscanf(f, "%lf", &x) == 1; }
inline bool printf_long(ll x, FILE* f = stdout) { return fprintf(f, "%lld", x) >= 0; }

// 缓冲快写：适合大量整数；支持 0、负数和 LLONG_MIN，避免直接 -LLONG_MIN。
// 结束/关闭文件前必须 flush；交互时每次询问后还需 fflush(file)。
// 不自动析构刷新，避免 FILE 已关闭；不可混用其他输出而不先刷新。
struct FastWriter
{
    FILE* file;
    array<char, BUF> buffer{};
    size_t pos = 0;
    explicit FastWriter(FILE* f = stdout) : file(f) {}
    bool flush()
    {
        size_t done = 0;
        while(done < pos)
        {
            size_t n = fwrite(buffer.data() + done, 1, pos - done, file);
            if(n == 0)
            {
                memmove(buffer.data(), buffer.data() + done, pos - done);
                pos -= done;
                return false;
            }
            done += n;
        }
        pos = 0;
        return true;
    }
    bool put(char c)
    {
        if(pos == buffer.size() && !flush()) return false;
        buffer[pos++] = c;
        return true;
    }
    bool write(ll x)
    {
        auto u = (unsigned long long)x;
        if(x < 0) { if(!put('-')) return false; u = 0ULL - u; }
        char digits[20];
        int n = 0;
        do { digits[n++] = (char)('0' + u % 10); u /= 10; } while(u);
        while(n) if(!put(digits[--n])) return false;
        return true;
    }
    bool write_string(const char* s) { while(*s) if(!put(*s++)) return false; return true; }
};

// 兼容常用的 stdin/stdout 简写；read_long 无参版 EOF 返回 0，推荐 bool 版。
inline FreadReader input;
inline FastWriter output;
inline int get_char() { return input.next(); }
inline bool read_long(ll& x) { return input.read(x); }
inline ll read_long() { ll x = 0; input.read(x); return x; }
inline int read_int() { return (int)read_long(); } // 调用者保证在 int 范围内
inline bool read_str(string& s) { return input.read_string(s); }
inline bool write_long(ll x) { return output.write(x); }
inline bool put_char(char c) { return output.put(c); }
inline bool write_char(char c) { return output.put(c); }
inline bool write_str(const char* s) { return output.write_string(s); }
inline bool flush_out() { return output.flush(); }

// 读入 1e6 个整数对照表（定性参考，非本机实测；受平台、库、文件/终端影响）：
// 写法            相对速度          使用建议
// 默认 cin        通常最慢          小数据足够，默认同步方便混用但不推荐混用
// 提速 cin        通常较快          默认首选，代码清楚、类型安全
// getchar         通常较快          简短整数模板；逐字符调用成本依平台而异
// fread           通常最快一档      重定向文件/海量整数，交互题避开
// scanf           通常中等          浮点与复杂格式方便；不保证比提速 cin 慢
// fread+strtod    转换成本较高      浮点含科学计数法，速度须实测
// 缓冲快写        大量输出通常快    显式 flush；勿用 endl 逐行强制刷新
