#include "../../01-基础与技巧/快读快写.cpp"

void check(bool ok)
{
    static int count = 0; ++count; if(!ok) { cerr << "check error at " << count << "\n"; exit(1); }
}
FILE* make_file(const string& data)
{
    static unsigned serial = 0;
    static const string prefix = to_string(chrono::high_resolution_clock::now().time_since_epoch().count()) + "_" + to_string(random_device{}());
    static vector<filesystem::path> paths;
    struct Cleanup { ~Cleanup() { for(const auto& p : paths) { error_code ec; filesystem::remove(p, ec); } } };
    static Cleanup cleanup;
    auto path = filesystem::current_path() / ("codex_io_" + prefix + "_" + to_string(serial++) + ".tmp");
    FILE* f = fopen(path.string().c_str(), "w+b");
    paths.push_back(path);
    if(!f) cerr << "file error " << errno << " " << path.string() << "\n";
    check(f != nullptr);
    check(fwrite(data.data(), 1, data.size(), f) == data.size());
    rewind(f);
    return f;
}
int main()
{
    fast_cin();
    const vector<ll> expected = {0, -1, 123, -456, 1000000000000LL,
                                LLONG_MIN, LLONG_MAX};
    string data = " \t\r\n";
    for(ll x : expected) data += to_string(x) + " \n";
    data += "+789"; // 最后一个数后无空白
    vector<ll> values = expected;
    values.push_back(789);
    istringstream stream(data);
    FILE* a = make_file(data);
    FILE* b = make_file(data);
    FILE* c = make_file(data);
    GetcharReader gc(a);
    auto fr = make_unique<FreadReader>(b);
    for(ll want : values)
    {
        ll x = 9, y = 9, z = 9, w = 9;
        check(bool(stream >> x));
        check(gc.read(y));
        check(fr->read(z));
        check(scanf_long(w, c));
        check(x == want && y == x && z == x && w == x);
    }
    ll untouched = 42;
    check(!gc.read(untouched) && untouched == 42);
    check(!fr->read(untouched) && untouched == 42);
    check(!scanf_long(untouched, c));
    check(!(stream >> untouched));
    fclose(a); fclose(b); fclose(c);

    // 真正跨越 1 MiB 输入与输出缓冲；唯一命名临时文件自动清理，无内存回退。
    string large;
    for(int i = 0; i < 200000; ++i) large += "-1234567 ";
    FILE* big = make_file(large);
    auto big_reader = make_unique<FreadReader>(big);
    for(int i = 0; i < 200000; ++i)
    {
        ll x;
        check(big_reader->read(x) && x == -1234567);
    }
    check(!big_reader->read(untouched));
    fclose(big);

    FILE* written = make_file("");
    check(written != nullptr);
    auto writer = make_unique<FastWriter>(written);
    for(ll x : values) check(writer->write(x) && writer->put('\n'));
    check(writer->write_string(large.c_str()));
    check(writer->flush());
    rewind(written);
    string actual;
    char block[4096];
    size_t n;
    while((n = fread(block, 1, sizeof block, written))) actual.append(block, n);
    string wanted;
    for(ll x : values) wanted += to_string(x) + "\n";
    check(actual == wanted + large);
    fclose(written);

    string floats = "0 -0.125 +3.5 6.02e23 -2E-4 .5 12.";
    FILE* f = make_file(floats);
    FILE* g = make_file(floats);
    auto float_reader = make_unique<FreadReader>(f);
    istringstream fs(floats);
    for(int i = 0; i < 7; ++i)
    {
        double x, y, z;
        check(float_reader->read_double(x) && scanf_double(y, g) && bool(fs >> z));
        check(x == y && y == z);
    }
    double d = 42;
    check(!float_reader->read_double(d) && d == 42);
    fclose(f); fclose(g);
    FILE* invalid = make_file("9223372036854775808 -9223372036854775809 + 12x 7");
    auto invalid_reader = make_unique<FreadReader>(invalid);
    for(int i = 0; i < 4; ++i) check(!invalid_reader->read(untouched));
    check(invalid_reader->read(untouched) && untouched == 7);
    fclose(invalid);
    FILE* invalid_float = make_file("1e9999 3x nan 2.5");
    auto invalid_float_reader = make_unique<FreadReader>(invalid_float);
    for(int i = 0; i < 3; ++i) check(!invalid_float_reader->read_double(d) && d == 42);
    check(invalid_float_reader->read_double(d) && d == 2.5);
    fclose(invalid_float);
    cout << "OK: integer readers, floating readers, EOF, bounds and buffered output\n";
}




