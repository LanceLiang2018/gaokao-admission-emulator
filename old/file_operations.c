#include "admission_elements.h"


// #define MAX_BUF_LENGTH 256

int import_student_info(FILE *stu_file, int *stu_number, STU *stu_datalist)
{
    int sch_number, wish_number, read = 0;
    fscanf(stu_file, "%d\n", stu_number);

    for (int i = 0; i < *stu_number; ++i)
    {
        fscanf(stu_file, "%d,%d,%d,", &stu_datalist[i].stu_id, &stu_datalist[i].stu_score, &sch_number);
        for (int j = 0; j < sch_number; j++)
        {
            stu_datalist[i].stu_wishes[j].wish_enabled = 1;

            fscanf(stu_file, "%d,%d,%d,", &stu_datalist[i].stu_wishes[j].wish_sch_id, &stu_datalist[i].stu_wishes[j].wish_exchange, &wish_number);
            for (int k = 0; k < wish_number; k++)
            {
                fscanf(stu_file, "%d,", &stu_datalist[i].stu_wishes[j].wish_sub[k]);
            }
        }
        fscanf(stu_file, "%s\n", stu_datalist[i].stu_name);
        read++;
    }
    return read;
}

int import_school_info(FILE *sch_file, int *sch_number, SCH *sch_datalist)
{
    int read = 0;
    fscanf(sch_file, "%d\n", sch_number);

    memset(sch_datalist, 0, sizeof(SCH) * MAX_SCH_NUMBER);
    for (int i = 0; i < *sch_number; ++i)
    {
        fscanf(sch_file, "%d,%d,", &sch_datalist[i].sch_id, &sch_datalist[i].sch_sub_number);
        for (int j = 0; j < sch_datalist[i].sch_sub_number; j++)
            fscanf(sch_file, "%d,%d,", &sch_datalist[i].sch_planned_number[j][0], &sch_datalist[i].sch_planned_number[j][1]);
        fscanf(sch_file, "%s\n", sch_datalist[i].sch_name);
        read++;
    }
    return read;
}

