template <class Iterator, class Compare>
void merge_sort(Iterator first, Iterator last, Compare comp) {
    const auto d = last - first;
    if (d <= 1) return;
    if (d == 2) {
        if (!comp(*first, *(first + 1))) swap(*first, *(first + 1));
        return;
    }
    merge_sort(first, first + d / 2, comp);
    merge_sort(first + d / 2, last, comp);
    inplace_merge(first, first + d / 2, last, comp);
}