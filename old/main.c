#include "admission_elements.h"
#define MAX_FILENAME 30
int main(void)
{
    STU all_student_list[MAX_STU_NUMBER];
    SCH all_school_list[MAX_SCH_NUMBER];
    int student_number;
    int school_number;
    FILE *student_file;
    FILE *school_file;
    char student_filename[MAX_FILENAME];
    char school_filename[MAX_FILENAME];
    STATUS all_student_results[MAX_STU_NUMBER];
    memset(all_student_results, 0, sizeof(all_student_results));
    memset(all_student_list, 0, sizeof(all_student_list));
    memset(all_school_list, 0, sizeof(all_school_list));

    printf("Enter the filename of students\' data list: "); scanf("%s", student_filename);
    printf("Enter the filename of schools\' data list: "); scanf("%s", school_filename);

    if ((student_file = fopen(student_filename, "r")) == NULL)
    {
        printf("E: Error(s) occurred while opening the students\' data file!\n");
        return 0;
    }

    if ((school_file = fopen(school_filename, "r")) == NULL)
    {
        printf("E: Error(s) occurred while opening the schools\' data file!\n");
        return 0;
    }
    
    printf("\n");
    printf("Successfully imported data of %d student(s)!\n", import_student_info(student_file, &student_number, all_student_list));
    for (int i = 0; i < student_number; i++)
    {
        printf("ID: %d, Name: %s, Score: %d\n", all_student_list[i].stu_id,all_student_list[i].stu_name, all_student_list[i].stu_score);
        int j = 0;
        while (all_student_list[i].stu_wishes[j].wish_enabled)
        {
            printf("\tWish No.%d: School: %d, Major(s): ", j + 1, all_student_list[i].stu_wishes[j].wish_sch_id);
            int k = 0;
            while (all_student_list[i].stu_wishes[j].wish_sub[k])
                printf("%d ", all_student_list[i].stu_wishes[j].wish_sub[k++]);
            printf(", Exchange: %d\n", all_student_list[i].stu_wishes[j].wish_exchange);
            j++;
        }
    }

    printf("\n");
    printf("Successfully imported data of %d school(s)!\n", import_school_info(school_file, &school_number, all_school_list));
    for (int i = 0; i < school_number; i++)
    {
        printf("ID: %d, Name: %s\n", all_school_list[i].sch_id, all_school_list[i].sch_name);
        for (int j = 0; j < all_school_list[i].sch_sub_number; j++)
        {
            printf("\tSubject No.%d: ID: %d, Plan: %d\n", j + 1, all_school_list[i].sch_planned_number[j][0], all_school_list[i].sch_planned_number[j][1]);
        }
    }

    printf("Start merge sort...\n");
    merge_sort(all_student_list, 0, student_number - 1);

    printf("\n");
    printf("Successfully sorted data of %d student(s)!\n", student_number);
    for (int i = 0; i < student_number; i++)
    {
        printf("ID: %d, Name: %s, Score: %d\n", all_student_list[i].stu_id,all_student_list[i].stu_name, all_student_list[i].stu_score);
        int j = 0;
        while (all_student_list[i].stu_wishes[j].wish_enabled)
        {
            printf("\tWish No.%d: School: %d, Major(s): ", j + 1, all_student_list[i].stu_wishes[j].wish_sch_id);
            int k = 0;
            while (all_student_list[i].stu_wishes[j].wish_sub[k])
                printf("%d ", all_student_list[i].stu_wishes[j].wish_sub[k++]);
            printf(", Exchange: %d\n", all_student_list[i].stu_wishes[j].wish_exchange);
            j++;
        }
    }

    for (int i = 0; i < student_number; i++)
    {
        printf("Processing student with ID %d...", all_student_list[i].stu_id);
        printf("Result: %d.\n", emulate_admission(all_student_list[i], all_school_list, school_number, &all_student_results[i]));
    }

    FILE *result_file;
    char result_filename[MAX_FILENAME];
    printf("Enter the name of file you want to save the result: "); scanf("%s", result_filename);

    if ((result_file = fopen(result_filename, "w")) == NULL)
    {
        printf("E: Error(s) occurred while opening this file.\n");
        return 0;
    }

    fprintf(result_file, "Name,ID,Score,Status,School(ID),School(Name),Subject(ID)\n");
    for (int i = 0; i < student_number; i++)
    {
        fprintf(result_file, "%s,%d,%d,", all_student_results[i].student_name, all_student_results[i].student_id, all_student_list[i].stu_score);
        switch (all_student_results[i].admission_status)
        {
            case ERROR:
                fprintf(result_file, "Error\n");
                break;
            case ADMITTED:
                fprintf(result_file, "Admitted,%d,%s,%d\n", all_student_results[i].school_id, all_student_results[i].school_name,all_student_results[i].subject_id);
                break;
            case RETURNED:
                fprintf(result_file, "Rejected\n");
                break;
            case FAILED:
                fprintf(result_file, "Failed\n");
                break;
        }
    }

    fclose(student_file);
    fclose(school_file);
    fclose(result_file);
    return 0;
}