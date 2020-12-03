#include <emscripten/emscripten.h>
#include <stdio.h>

int main(int argc, char** argv) {
  printf("Hello World\n");
  return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE
char* myFunction(int argc, char** argv) {
  char *str = "程序被调用了!!!\n";
  printf(str);
  return str;
}

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
  return a + b;
}

#ifdef __cplusplus
}
#endif