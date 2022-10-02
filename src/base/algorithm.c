/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/algorithm.h>

void
quicksort(Int32 **nums, Int32 low_index, Int32 high_index)
{
    if (low_index >= high_index)
        return;

    Int32 pivot = (Int32)(UPtr)nums[high_index];
    swap(nums, low_index, high_index);

    Int32 l = quicksort_parsing(nums, low_index, high_index, pivot);

    quicksort(nums, low_index, l - 1);
    quicksort(nums, l + 1, high_index);
}

Usize
quicksort_parsing(Int32 **nums, Int32 low_index, Int32 high_index, Int32 pivot)
{
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

void
swap(Int32 **nums, Usize index1, Usize index2)
{
}
