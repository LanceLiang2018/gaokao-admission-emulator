#include "admission_elements.h"

EMSCRIPTEN_KEEPALIVE
void merge_array(STU *student_list, int first, int mid, int last)
{
    STU temp[MAX_STU_NUMBER];
    int i = first, j = mid + 1;
    int m = mid, n = last;
    int k = 0;
    memset(temp, 0, sizeof(temp));
    while (i <= m && j <= n)
    {
        if (student_list[i].stu_score >= student_list[j].stu_score)
            temp[k++] = student_list[i++];
        else
        {
            temp[k++] = student_list[j++];
        }
    }
    while (i <= m)
        temp[k++] = student_list[i++];
    while (j <= n)
        temp[k++] = student_list[j++];
    for (i = 0; i < k; ++i)
        student_list[first + i] = temp[i];
    return;
}

void merge_sort(STU *student_list, int first, int last)
{
    if (first < last)
    {
        int mid = (first + last) / 2;
        merge_sort(student_list, first, mid);
        merge_sort(student_list, mid + 1,last);
        merge_array(student_list, first, mid, last);
    }
    return;
}
