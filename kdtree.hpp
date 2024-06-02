#pragma once
#include <algorithm>
#include <array>
#include <vector>
using namespace std;

struct kd {
private:

	struct Node {
		int xmax = INT_MAX, xmin = INT_MIN;
		int ymax = INT_MAX, ymin = INT_MIN;

		Node* child[2];
		int idx;

		Node()
			: idx(-1)
		{
			child[0] = child[1] = nullptr;
		}
	};
	using T = array<int, 3>;

	constexpr long long sq(long long x) const { return x * x; }
	constexpr long long distance(long long x1, long long y1, long long x2, long long y2) const { return sq(x2 - x1) + sq(y2 - y1); }

	Node* root;
	vector<T> points;

public:

	kd(const vector<T> points_)
		: points(points_)
	{
		root = build(points.begin(), points.end());
	}
	~kd() {
		destruct(root);
	}

	vector<int> radius_search(const T& center, long long r) const {
		vector<int> res;
		radius_search_sub(center, root, res, r);
		sort(res.begin(), res.end());
		return res;
	}

private:

	Node* build(vector<T>::iterator l, vector<T>::iterator r) {
		if (r - l <= 0) return nullptr;

		Node* node = new Node();

		{
			const auto [argxmin, argxmax] = minmax_element(l, r, [](const T& l1, const T& r1) { return l1[0] < r1[0]; });
			node->xmin = (*argxmin)[0];
			node->xmax = (*argxmax)[0];
		}
		{
			const auto [argymin, argymax] = minmax_element(l, r, [](const T& l1, const T& r1) { return l1[1] < r1[1]; });
			node->ymin = (*argymin)[1];
			node->ymax = (*argymax)[1];
		}

		const int axis = ((node->xmax - node->xmin) > (node->ymax - node->ymin) ? 0 : 1);

		const auto mid = l + (r - l) / 2;
		nth_element(l, mid, r, [&](const T& l1, const T& r1) { return l1[axis] < r1[axis]; });

		node->idx = mid - points.begin();

		if (r - l > 1) {
			node->child[0] = build(l, mid);
			node->child[1] = build(mid + 1, r);
		}

		return node;
	}

	void destruct(Node* node) {
		if (node == nullptr) return;
		destruct(node->child[0]);
		destruct(node->child[1]);
		delete node;
	}

	template<class F>
	void each(Node* node, F f) const {
		if (node == nullptr) return;
		f(node->idx);
		each(node->child[0], f);
		each(node->child[1], f);
	}

	void radius_search_sub(const T& center, Node* node, vector<int>& res, long long r) const {
		if (node == nullptr) return;
		if (sq(max(abs(center[0] - node->xmin), abs(center[0] - node->xmax))) + sq(max(abs(center[1] - node->ymin), abs(center[1] - node->ymax))) <= r * r) {
			each(node, [&](int i) { res.push_back(points[i][2]); });
			return;
		}
		if (distance(center[0], center[1], clamp(center[0], node->xmin, node->xmax), clamp(center[1], node->ymin, node->ymax)) > r * r) return;

		if (distance(center[0], center[1], points[node->idx][0], points[node->idx][1]) <= r * r) res.push_back(points[node->idx][2]);

		radius_search_sub(center, node->child[0], res, r);
		radius_search_sub(center, node->child[1], res, r);
	}
};