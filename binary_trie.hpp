template <typename T>
class BinaryTrie {
   private:
    static constexpr int depth = bit_width(static_cast<unsigned long long>(numeric_limits<T>::max()));

    struct Node;
    using Nodeptr_t = Node*;

    struct Node {
        Nodeptr_t c0, c1;
        int cnt = 0;

        Node() : c0(nullptr), c1(nullptr), cnt(0) {}
    };

    Nodeptr_t root;

    T lazy_xor = 0;

    void del(Nodeptr_t& node) {
        if (node == nullptr) return;

        del(node->c0);
        del(node->c1);
        delete node;
    }

    void erase(T x, Nodeptr_t& node, int d) {
        if (d == -1) {
            node = nullptr;
            return;
        }

        if ((x >> d & 1) == (lazy_xor >> d & 1))
            erase(x, node->c0, d - 1);
        else
            erase(x, node->c1, d - 1);

        node->cnt--;
        if (node->cnt == 0) node = nullptr;
    }

   public:
    BinaryTrie() { root = new Node(); }

    ~BinaryTrie() { del(root); }

    void insert(T x) {
        if (contains(x)) return;
        if (root == nullptr) root = new Node();

        Nodeptr_t node = root;
        for (int i = depth - 1; i >= 0; --i) {
            node->cnt++;
            if ((x >> i & 1) == (lazy_xor >> i & 1)) {
                if (node->c0 == nullptr) node->c0 = new Node();
                node = node->c0;
            } else {
                if (node->c1 == nullptr) node->c1 = new Node();
                node = node->c1;
            }
        }
    }
    void erase(T x) {
        if (!contains(x)) return;

        erase(x, root, depth - 1);
    }
    bool contains(T x) {
        if (root == nullptr) return false;

        Nodeptr_t node = root;
        for (int i = depth - 1; i >= 0; --i) {
            if ((x >> i & 1) == (lazy_xor >> i & 1)) {
                if (node->c0 == nullptr) return false;
                node = node->c0;
            } else {
                if (node->c1 == nullptr) return false;
                node = node->c1;
            }
        }
        return true;
    }

    T min_element() {
        Nodeptr_t node = root;
        T res = 0;
        for (int i = depth - 1; i >= 0; --i) {
            if (lazy_xor >> i & 1) {
                if (node->c1 != nullptr)
                    node = node->c1;
                else
                    node = node->c0, res |= (T(1) << i);
            } else {
                if (node->c0 != nullptr)
                    node = node->c0;
                else
                    node = node->c1, res |= (T(1) << i);
            }
        }
        return res;
    }
    T max_element() {
        Nodeptr_t node = root;
        T res = 0;
        for (int i = depth - 1; i >= 0; --i) {
            if (lazy_xor >> i & 1) {
                if (node->c0 != nullptr)
                    node = node->c0, res |= (T(1) << i);
                else
                    node = node->c1;
            } else {
                if (node->c1 != nullptr)
                    node = node->c1, res |= (T(1) << i);
                else
                    node = node->c0;
            }
        }
        return res;
    }

    void apply_xor(T x) { lazy_xor ^= x; }
};