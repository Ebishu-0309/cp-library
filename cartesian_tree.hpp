template <typename T, class Compare = less<>>
vector<int> cartesian_tree(vector<T> a, Compare comp = Compare{}) {
    int n = a.size();
    vector<int> p(n, -1);
    stack<T> st;
    for (int i = 0; i < n; ++i) {
        int s = -1;
        while (!st.empty() && comp(a[i], a[st.top()])) s = st.top(), st.pop();
        if (s != -1) p[s] = i;
        if (!st.empty()) p[i] = st.top();
        st.push(i);
    }
    return p;
}