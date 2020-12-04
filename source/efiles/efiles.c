/*
- Description: 模拟文件系统。提供模拟的文件读写功能。需要提前写入相关数据。
- Version: 0.0.1
- Author: Chiro
- Email: Chiro2001@163.com
*/

#include "efiles.h"

// 文件列表：一个元素为 FILE* 类型的数组，指向所有的文件
EFILE** root = NULL;
// 文件列表尾部指针，指向下一个的位置
EFILE** tail = NULL;
// 最大文件大小：默认10MB
int efiles_max_files_size = 1024 * 1024 * 10;
// 最多文件个数：默认1024
int efiles_max_file_num = 1024;
// 是efprintf等要用到的内存
char _efiles_buf_[EFILE_BUF_SIZE];

/* ========== 内部功能 ========== */

// @Description: 清除文件列表
void efiles_clear() {
  if (!root) return;
  EFILE** p = root;
  while (p) free(*p++);
  free(root);
}

// @Description: 查找文件
// @Return: EFILE* / NULL
EFILE* efiles_find(const char* filename) {
  // 直接顺序查找文件名
  // TODO: 使用堆优化（但是我懒得做
  for (EFILE** p = root; *p; p++) {
    if (strcmp((*p)->filename, filename) == 0) return *p;
  }
  return NULL;
}

// @Description: 新建一个文件
// @Return: EFILE*
EFILE* efiles_create(const char* filename) {
  if (efiles_find(filename) != NULL) {
    return NULL;
  }
  if (tail - root >= efiles_max_file_num) return NULL;
  EFILE* stream = (EFILE*)malloc(sizeof(EFILE));
  assert(stream);
  memset(stream, 0, sizeof(EFILE));  // 设置到默认的写入状态
  strcpy(stream->filename, filename);
  *tail = stream;
  tail++;
  return stream;
}

// @Description: 关闭目前正在读取的文件(也就是重置一下offset等状态)
//               请记得把对应指针置NULL
int efiles_close(EFILE* stream) {
  stream->flag = EFILE_FLAG_WRITE;
  stream->mode_flag = EFILE_MODE_WRITE;
  stream->offset = 0;
  return 0;
}

// @Description: 删除一个文件
// @Retrun: 0 -- 成功; 非零 -- 失败
int efiles_delete(EFILE* stream) {
  if (!stream) return 1;
  // 先释放对应数据区
  if (!stream->data) return 2;
  free(stream->data);
  // 然后从文件列表删除这个指针
  EFILE** target = NULL;
  for (EFILE** p = root; *p; p++)
    if (stream == *p) {
      target = p;
      break;
    }
  if (!target) return 3;
  // EFILE *t = *target;
  *target = *(tail - 1);
  // *(tail - 1) = t;
  *(tail - 1) = NULL;
  tail--;

  // 最后删除这个指针指向的内存
  free(stream);
  return 0;
}

// @Description: 标记一个文件的当前状态
void efiles_mark(EFILE* stream, EFILE_FLAG flag) {
  if (!stream) return;
  stream->flag = flag;
}

// @Description: 该文件是否可读
int efiles_readable(EFILE* stream) {
  if (!stream) return 0;
  if (stream->flag & EFILE_FLAG_ERROR || !(stream->flag & EFILE_FLAG_READ))
    return 0;
  return 1;
}

// @Description: 该文件是否可写
int efiles_writeable(EFILE* stream) {
  if (!stream) return 0;
  if (stream->flag & EFILE_FLAG_ERROR || stream->flag & EFILE_FLAG_READ)
    return 0;
  return 1;
}

/* ========== 中层 API ========== */

// @Description: 初始化函数：初始化文件列表信息
void efiles_init() {
  if (root) return;
  root = (EFILE**)malloc(sizeof(EFILE*) * efiles_max_file_num);
  assert(root);
  memset(root, 0, sizeof(EFILE*) * efiles_max_file_num);
  tail = root;
}

// @Description: 对文件写入数据
void efiles_write(EFILE* stream, void* data, size_t size) {
  // 申请内存空间
  void* temp = realloc(stream->data, size + stream->size);
  if (!temp) {
    efiles_mark(stream, EFILE_FLAG_WRITE_ERROR);
    return;
  }
  stream->data = temp;
  // 可能不对
  memcpy((uint8_t*)stream->data + stream->offset, data, size);
  stream->offset += size;
  stream->size += size;
}

// @Description: 读取文件数据
size_t efiles_read(EFILE* stream, void* data, size_t size) {
  if (!stream || !data || !size) return 0;
  size = size + stream->offset > stream->size ? stream->size - stream->offset
    : size;
  memcpy(data, (uint8_t*)stream->data + stream->offset, size);
  stream->offset += size;
  return size;
}

/* ========== 上层 API ========== */

EFILE* efopen(const char* filename, const char* mode) {
  // 暂时只支持两个操作："w" / "r"
  if (!filename || !mode || !*filename || !*mode) return NULL;
  EFILE* exist = efiles_find(filename);
  // 写入
  if (*mode == 'w') {
    // 对已经存在的文件进行覆盖，也就是先删除
    if (exist) {
      efiles_delete(exist);
    }
    EFILE* file = efiles_create(filename);
    return file;
  }
  else if (*mode == 'r') {
    // 读取
    if (!exist) return NULL;
    exist->flag = EFILE_FLAG_READ;
    return exist;
  }
  // 不支持的操作
  return NULL;
}

size_t eftell(EFILE* stream) {
  if (!stream) return 0;
  return stream->offset;
}

size_t efwrite(void* buf, size_t size, size_t count, EFILE* stream) {
  if (!buf || !size || !count || !stream || stream->flag & EFILE_FLAG_ERROR)
    return 0;
  uint8_t* p = (uint8_t*)buf;
  for (size_t i = 0; i < count; i++) {
    efiles_write(stream, p, size);
    if (stream->size >= efiles_max_files_size) {
      return (p - (uint8_t*)buf) * size;
    }
    p += size;
  }
  return size * count;
}

size_t efread(void* buf, size_t size, size_t count, EFILE* stream) {
  if (!buf || !size || !count || !stream || stream->flag & 0x80) return 0;
  uint8_t* p = (uint8_t*)buf;
  size_t wrote = 0;
  for (size_t i = 0; i < count; i++) {
    wrote = efiles_read(stream, p, size);
    if (wrote != size) return p - (uint8_t*)buf;
    p += size;
  }
  return size * count;
}

int efgetchar(EFILE* stream) {
  int c;
  size_t size = efiles_read(stream, &c, sizeof(char));
  if (size == 0) return EOF;
  return c;
}

int efclose(EFILE* stream) { return efiles_close(stream); }

// 使用宏定义完成
// int efprintf(EFILE *stream, const char *format, ...);

// @Description: 找到格式控制字符串中的控制参数个数还有对应位置
//               注意要调用efscanf_formats_clean来清理内存防止内存泄露
// @Return: 一个元素为char*类型的列表，长度为len +
//          1，末尾被置零。列表元素对应指向'%'
const char** efscanf_formats_find(const char* format) {
  if (!format || !*format) return NULL;
  // 最多就是len/2个控制参数
  const char** format_list =
    (const char**)malloc(sizeof(const char*) * (strlen(format) / 2 + 1));
  assert(format_list);
  memset((char**)format_list, 0, sizeof(char*) * (strlen(format) / 2 + 1));
  const char* p = format;
  const char** tail = format_list;
  while (*p) {
    if (*p == '%') {
      *tail = p;
      tail++;
      if (*(p + 1) != 0 && *(p + 1) == '%') p++;
    }
    p++;
  }
  return format_list;
}
void efscanf_formats_clean(const char** format_list) {
  if (!format_list) return;
  free((char**)format_list);
}

// @Description： 下面用到的输入限制函数们
int efscanf_read_limit_nospace(char c) { return !isspace(c); }
int efscanf_read_limit_number(char c) { return '0' <= c && c <= '9'; }
int efscanf_read_limit_float_number(char c) { return c == '.' || ('0' <= c && c <= '9'); }

// @Description: 读取文件中某一块的位置到字符串中
// @Return: 是否 (读到文件尾部 或者 读取到字符串尾部)
int efscanf_block_read(EFILE* stream, char* dst, int (*limit)(char)) {
  char c;
  char* sp = dst;
  // 忽略前导空白
  while ((c = efgetchar(stream)) != EOF && c != 0 && isspace(c))
    ;
  if (c == EOF || c == 0) return 1;
  *sp++ = c;
  // 读取到下一个空白或者读到末尾
  uint8_t flag_end = 0;
  while (1) {
    c = efgetchar(stream);
    if (c == EOF || c == 0) {
      *sp = 0;
      flag_end = 1;
      break;
    }
    if (!limit(c)) {
      *sp = 0;
      break;
    }
    *sp = c;
    sp++;
  }
  // 多读取了一个...
  // 悄悄移动文件指针...
  stream->offset--;
  return flag_end;
}

int efscanf(EFILE* stream, const char* format, ...) {
  if (!stream || stream->flag & 0x80 || !format || !*format) return 0;
  // 数一数控制符的数量
  const char** format_list = efscanf_formats_find(format);
  int format_list_len = 0;
  // 当前处理到的格式字符所在指针，确保能访问到p+1
  const char** fp = format_list;
  while (*(fp++)) format_list_len++;
  fp = format_list;
  va_list valist;
  // 为所有参数初始化
  // va_start(valist, format_list_len);
  va_start(valist, format);
  // 当前读取到的字符
  char c;
  // 当前处理到的格式字符
  const char* f = format;
  // 记录成功赋值了几个参数
  int count = 0;
  // 是否到末尾
  uint8_t flag_end;
  while (*f) {
    // 比较模式：按照字符比较/按照控制符比较
    if (*fp == f) {
      // 处理到格式字符部分，就按照控制符比较
      // 先比较%后面一个字符...
      // 对无符号照样写入，忽略%u_，直接变成%_
      if (*(*fp + 1) == 'u' && *(*fp + 2)) fp[1] += 1;
      switch (*(*fp + 1)) {
      case 'c':  // 字符模式
        c = efgetchar(stream);
        if (c == EOF || c == 0) goto efscanf_exit;
        *((char*)va_arg(valist, char*)) = c;
        count++;
        f++;
        break;
      case 's':  // 字符串模式
        if (efscanf_block_read(stream, (char*)va_arg(valist, char*),
          efscanf_read_limit_nospace)) {
          count++;
          goto efscanf_exit;
        }
        count++;
        f++;
        break;
      case 'd':  // 整型模式
        flag_end = efscanf_block_read(stream, _efiles_buf_,
          efscanf_read_limit_number);
        *((int*)va_arg(valist, int*)) = atoi(_efiles_buf_);
        count++;
        f++;
        if (flag_end) goto efscanf_exit;
        break;
      case 'f':  // 单精度浮点数模式
        flag_end = efscanf_block_read(stream, _efiles_buf_,
          efscanf_read_limit_float_number);
        *((float*)va_arg(valist, float*)) = (float)atof(_efiles_buf_);
        count++;
        f++;
        if (flag_end) goto efscanf_exit;
        break;
      case 'l':  // long 模式
        // 注意，这部分如果遇到不合理的格式字符串，会访问非法内存
        switch (*(*fp + 2)) {
        case 'e':
        case 'f':  // double 模式
          flag_end = efscanf_block_read(stream, _efiles_buf_,
            efscanf_read_limit_float_number);
          *((double*)va_arg(valist, double*)) =
            (double)atof(_efiles_buf_);
          count++;
          f += 2;
          if (flag_end) goto efscanf_exit;
          break;
        case 'd':
        default:  // 默认用 long (int) 类型
          flag_end = efscanf_block_read(stream, _efiles_buf_,
            efscanf_read_limit_number);
          *((long*)va_arg(valist, long*)) = (long)atol(_efiles_buf_);
          count++;
          f += 2;
          if (flag_end) goto efscanf_exit;
          break;
        }
        break;
      case 'L':  // long long 模式
        // 注意，这部分如果遇到不合理的格式字符串，会访问非法内存
        switch (*(*fp + 2)) {
        case 'e':
        case 'f':  // long double 模式
          // 不知道合不合适，这里使用atof
          flag_end = efscanf_block_read(stream, _efiles_buf_,
            efscanf_read_limit_float_number);
          *((long double*)va_arg(valist, long double*)) =
            (long double)atof(_efiles_buf_);
          count++;
          f += 2;
          if (flag_end) goto efscanf_exit;
          break;
        default:  // 默认就是 long int，long 也是 long int。
          flag_end = efscanf_block_read(stream, _efiles_buf_,
            efscanf_read_limit_number);
          *((long long*)va_arg(valist, long long*)) =
            (long long)atoll(_efiles_buf_);
          count++;
          f += 2;
          if (flag_end) goto efscanf_exit;
          break;
        }
        break;
      default:
        // 未支持格式!!
        break;
      }
      fp++;
    }
    else {
      // 处理到非格式控制符部分，按照字符比较
      c = efgetchar(stream);
      if (c == EOF || c == 0) return count;
      // 不匹配直接退出
      if (c != *f) return count;
      // 匹配就开始下一轮比较
    }
    f++;
  }
efscanf_exit:
  va_end(valist);
  return count;
}