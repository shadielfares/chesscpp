#include "tests.h"
#include <iostream>
using std::cout, std::endl;;

bool run_test(const char *name, bool (*test_fn)()) {
  bool result = test_fn();
  cout << name << ":" << (result ? "PASSED" : "FAILED") << endl;
  return result;
}

#define RUN_TEST(test_fn) (failures += !run_test(#test_fn, test_fn))

int main() {
  int failures = 0;

  RUN_TEST(test_pawn_generator_from_game_start);
  RUN_TEST(test_pawn_capture);
  RUN_TEST(test_black_pawn_move_blocked);
  RUN_TEST(test_black_pawn_capture);
  RUN_TEST(test_black_pawn_capture_hfile);
  RUN_TEST(test_en_passant);
  RUN_TEST(test_black_pawn_generator_from_game_start);
  RUN_TEST(test_black_pawn_single_move_d6);
  RUN_TEST(test_black_pawn_double_move_blocked_on_rank5);
  RUN_TEST(test_black_pawn_capture_both_sides);
  RUN_TEST(test_black_pawn_capture_afile);
  RUN_TEST(test_black_en_passant);
  RUN_TEST(test_black_en_passant_hfile);

  return failures;
}
