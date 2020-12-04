#include "admission_elements.h"
#define MAX_FILENAME 30

#ifdef USE_WASM
EMSCRIPTEN_KEEPALIVE
void save_student_data(char *data) {
  // 尝试写入数据
  FILE *writer = fopen("student.csv", "w");
  if (!writer) return 1;
  fprintf(writer, "%s", data);
  fclose(writer);
}

EMSCRIPTEN_KEEPALIVE
void save_school_data(char *data) {
  FILE *writer = fopen("school.csv", "w");
  if (!writer) return 1;
  fprintf(writer, "%s", data);
  fclose(writer);
}
#endif

EMSCRIPTEN_KEEPALIVE
#ifdef USE_WASM
int program_main(void)
#else
int main(void)
#endif
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

#ifdef USE_EFILES
  // 配置虚拟文件系统
  efiles_init();
#endif

#ifdef USE_WASM
//   save_student_data(
//       "41\n\
// 20031,685,1,20008,1,1,201,Chiro\n\
// 20045,671,3,20008,1,2,201,202,20002,1,2,201,202,20003,1,3,201,202,203,Alex\n\
// 20103,698,1,20002,1,4,204,204,201,202,Abe\n\
// 20154,700,5,20006,0,4,204,204,204,204,20004,1,2,202,201,20001,1,2,201,202,20001,0,3,203,203,202,20001,1,4,203,201,201,203,Nick\n\
// 20186,700,4,20002,1,1,201,20002,0,3,201,203,201,20001,0,4,204,201,201,204,20007,1,4,203,204,204,202,Saoirse\n\
// 20194,694,2,20004,0,2,201,202,20009,0,3,202,201,203,Chip\n\
// 20206,648,5,20003,0,3,202,201,203,20002,1,4,202,203,201,204,20009,1,1,201,20007,0,4,204,204,204,204,20005,0,2,201,202,Dale\n\
// 20208,671,1,20003,1,4,202,204,202,201,Elsa\n\
// 20228,624,5,20007,0,4,201,203,202,204,20003,0,1,201,20008,0,2,202,201,20006,0,4,201,201,201,202,20004,0,1,201,Anna\n\
// 20234,673,4,20006,1,1,201,20006,1,1,201,20001,0,3,201,203,201,20005,1,2,201,202,Olaf\n\
// 20243,650,1,20008,0,2,202,201,Kristoff\n\
// 20255,632,1,20005,1,3,202,203,202,Bell\n\
// 20269,620,3,20002,1,3,203,201,203,20009,0,2,201,202,20002,0,3,203,201,203,Einstein\n\
// 20287,625,2,20005,0,2,202,202,20003,0,2,202,201,Monica\n\
// 20288,671,3,20002,1,2,201,202,20006,1,4,204,203,204,203,20001,0,2,201,201,Yuri\n\
// 20321,664,3,20005,0,3,203,201,202,20003,0,4,203,201,202,203,20003,0,2,201,201,Sayori\n\
// 20374,622,6,20007,0,1,201,20002,0,4,204,203,201,202,20005,0,3,202,202,202,20009,1,4,204,204,203,201,20004,0,2,201,202,20005,0,1,201,Natsuki\n\
// 20390,622,2,20006,0,1,201,20008,1,2,202,202,Lily\n\
// 20438,666,5,20006,1,4,201,204,202,204,20007,0,2,202,202,20004,0,2,201,201,20005,0,1,201,20001,0,1,201,Delta\n\
// 20451,646,2,20002,0,4,203,201,204,201,20002,1,4,202,202,201,203,Sarah\n\
// 20474,630,1,20009,0,3,203,202,201,Serena\n\
// 20490,647,1,20006,0,2,202,202,Qiana\n\
// 20511,550,3,20001,1,1,201,20005,0,3,202,203,203,20004,1,3,201,202,201,Petra\n\
// 20529,569,1,20007,0,2,201,201,Tina\n\
// 20542,553,2,20006,0,1,201,20001,0,4,204,201,202,204,Emy\n\
// 20559,517,2,20006,1,3,201,203,201,20008,0,1,201,Amy\n\
// 20564,522,5,20004,0,4,202,204,201,202,20002,0,2,201,201,20006,1,3,202,201,203,20007,0,1,201,20005,0,3,203,201,201,Shelton\n\
// 20582,568,4,20005,0,1,201,20007,0,4,204,201,203,201,20006,0,3,201,202,202,20002,0,4,204,201,204,204,Canvas\n\
// 20593,528,3,20001,1,1,201,20007,1,4,203,202,204,203,20005,0,3,202,202,201,Vanilla\n\
// 20613,599,6,20005,1,2,201,201,20004,0,3,201,202,202,20003,0,2,202,202,20009,0,4,202,202,203,203,20009,0,4,204,204,201,202,20006,0,3,203,201,203,John\n\
// 20718,524,4,20004,0,4,201,202,204,204,20002,0,2,202,201,20005,1,3,203,202,201,20003,1,3,203,202,202,Wyles\n\
// 20746,528,5,20005,0,2,202,201,20002,1,1,201,20006,0,4,203,202,204,203,20007,1,3,203,203,202,20007,0,1,201,River\n\
// 20763,599,6,20003,0,3,203,202,201,20007,1,4,202,204,201,201,20003,0,4,202,203,204,201,20004,0,4,204,201,203,201,20007,0,3,203,201,203,20007,0,2,201,202,Umi\n\
// 20787,603,5,20005,1,3,201,201,203,20003,0,1,201,20009,0,3,201,203,201,20002,0,1,201,20003,0,2,202,201,Colin\n\
// 20792,602,3,20009,0,1,201,20007,1,3,202,201,203,20004,0,2,202,201,Faye\n\
// 20821,586,1,20001,0,3,203,203,201,Sofia\n\
// 20871,543,4,20001,1,4,201,203,202,203,20009,1,2,201,202,20009,1,4,202,203,204,201,20008,1,1,201,Kan\n\
// 20876,537,4,20003,1,1,201,20006,1,1,201,20004,1,4,201,202,203,203,20007,1,1,201,Laura\n\
// 20885,518,3,20001,0,1,201,20004,0,3,201,203,203,20006,0,2,201,202,Mayo\n\
// 20899,598,6,20005,1,1,201,20007,1,2,202,202,20009,1,4,201,202,203,203,20001,0,3,203,201,202,20002,1,1,201,20003,1,3,201,203,203,Sensa\n\
// 20909,537,4,20001,0,4,201,204,201,203,20006,0,1,201,20005,1,1,201,20007,1,3,203,201,201,Phini");
//   save_school_data(
//       "9\n\
// 20001,2,201,2,202,2,FDU\n\
// 20002,4,201,2,202,2,203,3,204,1,NJU\n\
// 20003,4,201,3,202,3,203,2,204,2,USTC\n\
// 20004,4,201,3,202,1,203,3,204,2,ZJU\n\
// 20005,2,201,2,202,1,SJTU\n\
// 20006,3,201,3,202,3,203,3,XJTU\n\
// 20007,4,201,3,202,3,203,3,204,1,HIT\n\
// 20008,2,201,1,202,1,THU\n\
// 20009,2,201,1,202,2,PKU");
  if (!efiles_find("student.csv") || !efiles_find("school.csv")) return 1;
#endif

#ifndef USE_WASM
  printf("Enter the filename of students\' data list: ");
  scanf("%s", student_filename);
  printf("Enter the filename of schools\' data list: ");
  scanf("%s", school_filename);
#else
  strcpy(student_filename, "student.csv");
  strcpy(school_filename, "school.csv");
#endif

  if ((student_file = fopen(student_filename, "r")) == NULL) {
    printf("E: Error(s) occurred while opening the students\' data file!\n");
    return 0;
  }

  if ((school_file = fopen(school_filename, "r")) == NULL) {
    printf("E: Error(s) occurred while opening the schools\' data file!\n");
    return 0;
  }

  printf("\n");
  printf("Successfully imported data of %d student(s)!\n",
         import_student_info(student_file, &student_number, all_student_list));
  for (int i = 0; i < student_number; i++) {
    printf("ID: %d, Name: %s, Score: %d\n", all_student_list[i].stu_id,
           all_student_list[i].stu_name, all_student_list[i].stu_score);
    int j = 0;
    while (all_student_list[i].stu_wishes[j].wish_enabled) {
      printf("\tWish No.%d: School: %d, Major(s): ", j + 1,
             all_student_list[i].stu_wishes[j].wish_sch_id);
      int k = 0;
      while (all_student_list[i].stu_wishes[j].wish_sub[k])
        printf("%d ", all_student_list[i].stu_wishes[j].wish_sub[k++]);
      printf(", Exchange: %d\n",
             all_student_list[i].stu_wishes[j].wish_exchange);
      j++;
    }
  }

  printf("\n");
  printf("Successfully imported data of %d school(s)!\n",
         import_school_info(school_file, &school_number, all_school_list));
  for (int i = 0; i < school_number; i++) {
    printf("ID: %d, Name: %s\n", all_school_list[i].sch_id,
           all_school_list[i].sch_name);
    for (int j = 0; j < all_school_list[i].sch_sub_number; j++) {
      printf("\tSubject No.%d: ID: %d, Plan: %d\n", j + 1,
             all_school_list[i].sch_planned_number[j][0],
             all_school_list[i].sch_planned_number[j][1]);
    }
  }

  printf("Start merge sort...\n");
  merge_sort(all_student_list, 0, student_number - 1);

  printf("\n");
  printf("Successfully sorted data of %d student(s)!\n", student_number);
  for (int i = 0; i < student_number; i++) {
    printf("ID: %d, Name: %s, Score: %d\n", all_student_list[i].stu_id,
           all_student_list[i].stu_name, all_student_list[i].stu_score);
    int j = 0;
    while (all_student_list[i].stu_wishes[j].wish_enabled) {
      printf("\tWish No.%d: School: %d, Major(s): ", j + 1,
             all_student_list[i].stu_wishes[j].wish_sch_id);
      int k = 0;
      while (all_student_list[i].stu_wishes[j].wish_sub[k])
        printf("%d ", all_student_list[i].stu_wishes[j].wish_sub[k++]);
      printf(", Exchange: %d\n",
             all_student_list[i].stu_wishes[j].wish_exchange);
      j++;
    }
  }

  for (int i = 0; i < student_number; i++) {
    printf("Processing student with ID %d...", all_student_list[i].stu_id);
    printf("Result: %d.\n",
           emulate_admission(all_student_list[i], all_school_list,
                             school_number, &all_student_results[i]));
  }

  FILE *result_file;
  char result_filename[MAX_FILENAME];
  printf("Enter the name of file you want to save the result: ");
  // scanf("%s", result_filename);
  strcpy(result_filename, "result.csv");

  if ((result_file = fopen(result_filename, "w")) == NULL) {
    printf("E: Error(s) occurred while opening this file.\n");
    return 0;
  }

  fprintf(result_file, "%s",
          "Name,ID,Score,Status,School(ID),School(Name),Subject(ID)\n");
  for (int i = 0; i < student_number; i++) {
    fprintf(result_file, "%s,%d,%d,", all_student_results[i].student_name,
            all_student_results[i].student_id, all_student_list[i].stu_score);
    switch (all_student_results[i].admission_status) {
      case ERROR:
        fprintf(result_file, "%s", "Error\n");
        break;
      case ADMITTED:
        fprintf(result_file, "%s", "Admitted,%d,%s,%d\n",
                all_student_results[i].school_id,
                all_student_results[i].school_name,
                all_student_results[i].subject_id);
        break;
      case RETURNED:
        fprintf(result_file, "%s", "Rejected\n");
        break;
      case FAILED:
        fprintf(result_file, "%s", "Failed\n");
        break;
    }
  }

  fclose(student_file);
  fclose(school_file);
  fclose(result_file);

  if ((result_file = fopen(result_filename, "r")) == NULL) {
    printf("E: Error(s) occurred while opening this file.\n");
    return 0;
  }
  printf("%s content:\n", result_filename);
  char c;
  while ((c = fgetc(result_file)) > 0) {
    if (c != '\0') putchar(c);
  }
  puts("Program done!");
  return 0;
}
