#ifndef LILY_ALGORITHM_H
#define LILY_ALGORITHM_H

#include <base/types.h>

void quicksort(Int32 **nums, Int32 low_index, Int32 high_index);
Usize quicksort_parsing(Int32 **nums, Int32 low_index, Int32 high_index, Int32 pivot);
void swap(Int32 **nums, Usize index1, Usize index2);

#endif // LILY_ALGORITHM_H