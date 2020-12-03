#include "admission_elements.h"

admission_status emulate_admission(STU student, SCH *school_list, int sch_number, STATUS *admission_result)
{
    int is_school_existed, is_subject_existed;
    int j = 0, plan_count;
    while (student.stu_wishes[j].wish_enabled)
    {
        printf("Wish No.%d: Searching for school (ID: %d)...", j + 1, student.stu_wishes[j].wish_sch_id);

        for (int k = 0; k < sch_number; k++)
        {
            if (student.stu_wishes[j].wish_sch_id == school_list[k].sch_id)
            {
                is_school_existed = 1;
                printf("Success!\n");
                printf("Checking if can be admitted...");
                plan_count = 0;
                for (int l = 0; l < school_list[k].sch_sub_number; l++)
                    plan_count += school_list[k].sch_planned_number[l][1];
                if (!plan_count)
                {
                    printf("No.\n");
                    break;
                }
                else
                {
                    printf("Yes!\n");
                    int l = 0;
                    while (student.stu_wishes[j].wish_sub[l])
                    {
                        printf("Trying subject No.%d: ID: %d...", l + 1, student.stu_wishes[j].wish_sub[l]);
                        is_subject_existed = 0;
                        for (int m = 0; m < school_list[k].sch_sub_number; m++)
                        {
                            if (school_list[k].sch_planned_number[m][0] == student.stu_wishes[j].wish_sub[l])
                            {
                                is_subject_existed = 1;
                                if (school_list[k].sch_planned_number[m][1] > 0)
                                {
                                    printf("Admitted!\n");
                                    admission_result->admission_status = ADMITTED;
                                    strcpy(admission_result->student_name, student.stu_name);
                                    admission_result->student_id = student.stu_id;
                                    strcpy(admission_result->school_name, school_list[k].sch_name);
                                    admission_result->school_id = school_list[k].sch_id;
                                    admission_result->subject_id = student.stu_wishes[j].wish_sub[l];
                                    school_list[k].sch_planned_number[m][1]--;
                                    return ADMITTED;
                                } else
                                {
                                    printf("Cannot be admitted!\n");
                                    break;
                                }
                            }
                        }
                        if (!is_subject_existed)
                        {
                            printf("Not found!\n");
                            printf("E: Cannot find the subject with ID %d!\n", student.stu_wishes[j].wish_sub[l]);
                            admission_result->admission_status = ERROR;
                            strcpy(admission_result->student_name, student.stu_name);
                            admission_result->student_id = student.stu_id;
                            return ERROR;
                        }
                        l++;
                    }
                    printf("Checking if can be exchanged...");
                    if (student.stu_wishes[j].wish_exchange)
                    {
                        printf("Yes!\n");
                        printf("Getting the subject with least remaining...");
                        int least_subject_id = 0;
                        for (int m = 0; m < school_list[k].sch_sub_number; m++)
                            if (school_list[k].sch_planned_number[m][1] > 0)
                            {
                                least_subject_id = m;
                                break;
                            }
                        for (int m = least_subject_id; m < school_list[k].sch_sub_number; m++)
                            if (school_list[k].sch_planned_number[least_subject_id][1] > school_list[k].sch_planned_number[m][1] && school_list[k].sch_planned_number[m][1] > 0)
                                least_subject_id = m;
                        printf("It's ID %d...Making exchange...", school_list[k].sch_planned_number[least_subject_id][0]);
                        admission_result->admission_status = ADMITTED;
                        strcpy(admission_result->student_name, student.stu_name);
                        admission_result->student_id = student.stu_id;
                        strcpy(admission_result->school_name, school_list[k].sch_name);
                        admission_result->school_id = school_list[k].sch_id;
                        admission_result->subject_id = school_list[k].sch_planned_number[least_subject_id][0];
                        school_list[k].sch_planned_number[least_subject_id][1]--;
                        printf("Admitted!\n");
                        return ADMITTED;
                    } else
                    {
                        printf("No. Rejected.\n");
                        admission_result->admission_status = RETURNED;
                        strcpy(admission_result->student_name, student.stu_name);
                        admission_result->student_id = student.stu_id;
                        return RETURNED;
                    }
                }
            }
        }
        if (!is_school_existed)
        {
            printf("Not exist!\n");
            printf("E: Cannot find the school with ID: %d given by student with ID: %d!\n",
                   student.stu_wishes[j].wish_sch_id, student.stu_id);
            admission_result->admission_status = ERROR;
            strcpy(admission_result->student_name, student.stu_name);
            admission_result->student_id = student.stu_id;
            return ERROR;
        }
        j++;
    }
    printf("None of school in wishlist can admit.\n");
    admission_result->admission_status = FAILED;
    strcpy(admission_result->student_name, student.stu_name);
    admission_result->student_id = student.stu_id;
    return FAILED;
}