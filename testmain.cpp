#include "gtest/gtest.h"
#include <exception>
#ifdef __linux__
#include <execinfo.h>

void handler() {
  void *trace_elems[20];
  int trace_elem_count(backtrace(trace_elems, 20));
  char **stack_syms(backtrace_symbols(trace_elems, trace_elem_count));
  for (int i = 0; i < trace_elem_count; ++i) {
    std::cout << stack_syms[i] << "\n";
  }
  free(stack_syms);

  exit(1);
}
#endif

int main(int argc, char **argv)
{
#ifdef __linux__
  std::set_terminate(handler);
#endif

  try {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
  } catch (...) {
    return -1;
  }
}
