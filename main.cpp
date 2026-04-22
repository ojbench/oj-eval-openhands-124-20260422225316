#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const size_t BUFSIZE = 1 << 20; // 1 MiB
    int idx, size;
    char buf[BUFSIZE];
    FastScanner(): idx(0), size(0) {}
    inline char read() {
        if (idx >= size) {
            size = fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }
    template<typename T>
    bool nextInt(T &out) {
        char c;
        T sign = 1;
        T x = 0;
        c = read();
        if (!c) return false;
        while (c && (c<'0' || c>'9') && c!='-' ) { c = read(); if (!c) return false; }
        if (c=='-') { sign = -1; c = read(); }
        for (; c>='0' && c<='9'; c = read()) x = x*10 + (c - '0');
        out = x * sign;
        return true;
    }
};

struct SegTree {
    int n;
    vector<int> mx;
    vector<int> lz;
    SegTree(int n_ = 0) { init(n_); }
    void init(int n_) {
        n = max(1, n_);
        mx.assign(4*n + 5, 0);
        lz.assign(4*n + 5, 0);
    }
    inline void apply(int p, int val) {
        mx[p] += val;
        lz[p] += val;
    }
    inline void push(int p) {
        if (lz[p] != 0) {
            int v = lz[p];
            apply(p<<1, v);
            apply(p<<1|1, v);
            lz[p] = 0;
        }
    }
    inline void pull(int p) {
        mx[p] = max(mx[p<<1], mx[p<<1|1]);
    }
    void range_add(int p, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) { apply(p, val); return; }
        push(p);
        int m = (l + r) >> 1;
        if (ql <= m) range_add(p<<1, l, m, ql, qr, val);
        if (qr > m) range_add(p<<1|1, m+1, r, ql, qr, val);
        pull(p);
    }
    int range_max(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return mx[p];
        push(p);
        int m = (l + r) >> 1;
        int res = INT_MIN;
        if (ql <= m) res = max(res, range_max(p<<1, l, m, ql, qr));
        if (qr > m) res = max(res, range_max(p<<1|1, m+1, r, ql, qr));
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastScanner fs;
    int x, k, p;
    if (!fs.nextInt(x)) return 0;
    fs.nextInt(k); fs.nextInt(p);

    int segments = max(0, x - 1);
    SegTree st(segments);

    // Output buffer
    string out;
    out.reserve((size_t)p * 2);

    for (int i = 0; i < p; ++i) {
        int u, v, n;
        fs.nextInt(u); fs.nextInt(v); fs.nextInt(n);
        if (u > v) swap(u, v); // ensure u <= v
        if (v <= u) {
            // Zero-length travel consumes no capacity; accept.
            out.push_back('T'); out.push_back('\n');
            continue;
        }
        int l = u, r = v - 1; // segments are [u, v-1]
        int curMax = 0;
        if (segments > 0) curMax = st.range_max(1, 1, segments, l, r);
        if (curMax + n <= k) {
            if (segments > 0) st.range_add(1, 1, segments, l, r, n);
            out.push_back('T'); out.push_back('\n');
        } else {
            out.push_back('N'); out.push_back('\n');
        }
    }

    fwrite(out.c_str(), 1, out.size(), stdout);
    return 0;
}
