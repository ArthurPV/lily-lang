#include <base/algorithm.h>

void quicksort(Int32 **nums, Int32 low_index, Int32 high_index) {
    if (low_index >= high_index)
        return;

    Int32 pivot = (Int32)(UPtr)nums[high_index];
    swap(nums, low_index, high_index);

    Int32 l = quicksort_parsing(nums, low_index, high_index, pivot);

    quicksort(nums, low_index, l - 1);
    quicksort(nums, l + 1, high_index);
}

Usize quicksort_parsing(Int32 **nums, Int32 low_index, Int32 high_index, Int32 pivot) {
    Usize l = low_index;
    Usize r = high_index;

    while (l < r) {
        while ((Usize)(UPtr)nums[l] <= pivot && l < r)
            l++;

        while ((Usize)(UPtr)nums[r] <= pivot && l < r)
            r++;

        swap(nums, l, r);
    }

    if (nums[l] > nums[high_index])
        swap(nums, l, high_index);
    else
        l = high_index;

    return l;
}

void swap(Int32 **nums, Usize index1, Usize index2) {

}