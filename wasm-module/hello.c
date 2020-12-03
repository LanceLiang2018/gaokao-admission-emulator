#include <emscripten/emscripten.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
  return a + b;
}

#ifdef __cplusplus
}
#endif