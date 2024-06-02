class DynamicConvexHull {
public:
	using Point = pair<long long, long long>;

	DynamicConvexHull() = default;

	set<Point> upper, lower;

	// added: true
	bool add(long long x, long long y) {
		Point p(x, y);
		Point pu(x, numeric_limits<long long>::max()), pl(x, numeric_limits<long long>::lowest());

		bool u = false, l = false;
		if (upper.size() < 2) u = true;
		else {
			auto itr = upper.lower_bound(pl);
			if (itr == upper.end()) u = true;
			else {
				Point r = *itr;
				if (r.first == x) u = prev(upper.lower_bound(pu))->second < y;
				else {
					if (itr == upper.begin()) u = true;
					else {
						Point pre = *prev(itr);
						u = cross(diff(r, pre), diff(p, pre)) > 0;
					}
				}
			}
		}
		if (lower.size() < 2) l = true;
		else {
			auto itr = lower.lower_bound(pl);
			if (itr == lower.end()) l = true;
			else {
				Point r = *itr;
				if (r.first == x) l = r.second > y;
				else {
					if (itr == lower.begin()) l = true;
					else {
						Point pre = *prev(itr);
						l = cross(diff(r, pre), diff(p, pre)) < 0;
					}
				}
			}
		}

		if (u) {
			{
				auto itr = upper.insert(p).first;
				if (itr != upper.begin()) {
					itr--;
					while (itr != upper.begin()) {
						auto itr1 = prev(itr);
						if (cross(diff(*itr, p), diff(*itr1, *itr)) <= 0) {
							itr = prev(upper.erase(itr));
						}
						else break;
					}
				}
			}
			{
				auto itr = upper.lower_bound(p);
				if (itr != prev(upper.end())) {
					itr++;
					while (itr != prev(upper.end())) {
						auto itr1 = next(itr);
						if (cross(diff(*itr, p), diff(*itr1, *itr)) >= 0) {
							itr = upper.erase(itr);
						}
						else break;
					}
				}
			}
		}
		if (l) {
			{
				auto itr = lower.insert(p).first;
				if (itr != lower.begin()) {
					itr--;
					while (itr != lower.begin()) {
						auto itr1 = prev(itr);
						if (cross(diff(*itr, p), diff(*itr1, *itr)) >= 0) {
							itr = prev(lower.erase(itr));
						}
						else break;
					}
				}
			}
			{
				auto itr = lower.lower_bound(p);
				if (itr != prev(lower.end())) {
					itr++;
					while (itr != prev(lower.end())) {
						auto itr1 = next(itr);
						if (cross(diff(*itr, p), diff(*itr1, *itr)) <= 0) {
							itr = lower.erase(itr);
						}
						else break;
					}
				}
			}
		}
		return u || l;
	}

private:
	constexpr long long cross(const Point& a, const Point& b) { return a.first * b.second - a.second * b.first; }
	constexpr Point diff(const Point& a, const Point& b) { return { a.first - b.first, a.second - b.second }; }
};