/*
- Description: 模拟文件系统。提供模拟的文件读写功能。需要提前写入相关数据。
- Version: 0.0.1
- Author: Chiro
- Email: Chiro2001@163.com
*/

#ifndef EFILES_H
#define EFILES_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
// #include <errno.h>     // 返回错误信息(暂无)

// 最大文件名长度
#define EFILE_FILENAME_LEN 512

typedef enum {
  EFILE_MODE_WRITE = 0,  // 默认状态为写入
  EFILE_MODE_READ
} EFILE_MODE_FLAG;

// 最高位是0的时候表示正常操作
typedef enum {
  EFILE_FLAG_WRITE = 0x00,  // 默认状态为写入
  EFILE_FLAG_READ = 0x01,
  EFILE_FLAG_WRITE_ERROR = 0x80,
  EFILE_FLAG_READ_ERROR = 0x81,
  EFILE_FLAG_EOF = 0xFF,
  EFILE_FLAG_ERROR = 0x80
} EFILE_FLAG;

typedef struct _EFILE_ {
  char filename[EFILE_FILENAME_LEN];
  char *mode;                 // 支持的读写类型：["r", "w", ...]
  EFILE_MODE_FLAG mode_flag;  // 对应 mode
  size_t size;                // 文件大小（字节数）
  // size_t block;       // 文件块数量
  size_t offset;  // 当前读取/写入的偏移量
  void *data;     // 内容，存在内存中
  EFILE_FLAG flag;  // 当前状态，[写, 读, 写错误, 读错误, 读到尾部, ...]
} EFILE;

// 上层 API
EFILE *efopen(const char *filename, const char *mode);
size_t efread(void *buf, size_t size, size_t count, EFILE *stream);
size_t efwrite(void *buf, size_t size, size_t count, EFILE *stream);
size_t eftell(EFILE *stream);
int efclose(EFILE *stream);
int efgetchar(EFILE *stream);
int efscanf(EFILE *stream, const char *format, ...);
// int efprintf(EFILE *stream, const char *format, ...);

// 中层 API
void efiles_write(EFILE *stream, void *data, size_t size);
size_t efiles_read(EFILE *stream, void *data, size_t size);
void efiles_init();
// 内部函数
EFILE *efiles_find(const char *filename);
EFILE *efiles_create(const char *filename);
int efiles_delete(EFILE *stream);
int efiles_close(EFILE *stream);
void efiles_clear();
void efiles_mark(EFILE *steam, EFILE_FLAG flag);
int efiles_readable(EFILE *stream);
int efiles_writeable(EFILE *stream);

#define EFILE_BUF_SIZE 10240
extern char _efiles_buf_[EFILE_BUF_SIZE];
// 使用宏定义。注意，do-while(0)型没法用返回值
#define efprintf(fp, str, ...)                              \
  do {                                                      \
    sprintf(_efiles_buf_, str, __VA_ARGS__);                \
    efwrite(_efiles_buf_, strlen(_efiles_buf_) + 1, 1, fp); \
  } while (0)

#endif
