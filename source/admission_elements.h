#ifndef GAOKAO_ADMISSION_EMULATOR_ADMISSION_ELEMENTS_H
#define GAOKAO_ADMISSION_EMULATOR_ADMISSION_ELEMENTS_H

#include <stdio.h>
#include <string.h>

/* 选项部分 */
#define USE_WASM
#define USE_EFILES


#ifdef USE_WASM
/* 编译成 WASM 格式必需 */
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE extern
#endif
/* 内存虚拟文件系统 */
#ifdef USE_EFILES
#include "efiles/efiles.h"
#define fopen efopen
#define fprintf efprintf
#define fscanf efscanf
#define fclose efclose
#define fgetc efgetchar
#define FILE EFILE
#endif

#define MAX_STU_NUMBER 200
#define MAX_SCH_NUMBER 30
#define MAX_SUB_NUMBER 20
#define MAX_WISH_SCH_NUMBER 8
#define MAX_WISH_SUB_NUMBER 6

#define MAX_STU_NAME 25
#define MAX_SCH_NAME 25

#ifdef __cplusplus
extern "C" {
#endif

typedef enum status { ERROR, ADMITTED, FAILED, RETURNED } admission_status;

typedef struct student_wishes {
  int wish_enabled;
  int wish_exchange;
  int wish_sch_id;
  int wish_sub[MAX_WISH_SUB_NUMBER];
} WISH;

typedef struct student_elements {
  int stu_id;
  char stu_name[MAX_STU_NAME];
  int stu_score;
  WISH stu_wishes[MAX_WISH_SCH_NUMBER];
} STU;

typedef struct school_elements {
  int sch_id;
  char sch_name[MAX_SCH_NAME];
  int sch_sub_number;
  int sch_planned_number[MAX_SUB_NUMBER][2];

} SCH;

typedef struct student_admission_status {
  admission_status admission_status;
  int student_id;
  char student_name[MAX_STU_NAME];
  int school_id;
  char school_name[MAX_SCH_NAME];
  int subject_id;
} STATUS;

EMSCRIPTEN_KEEPALIVE int import_student_info(FILE *, int *, STU *);
EMSCRIPTEN_KEEPALIVE int import_school_info(FILE *, int *, SCH *);
EMSCRIPTEN_KEEPALIVE void merge_array(STU *, int, int, int);
EMSCRIPTEN_KEEPALIVE void merge_sort(STU *, int, int);
EMSCRIPTEN_KEEPALIVE admission_status emulate_admission(STU, SCH *, int,
                                                        STATUS *);

#ifdef __cplusplus
}
#endif

#endif