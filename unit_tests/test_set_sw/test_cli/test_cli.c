#include "test_cli.h"

#include "cli_manager.h"
#include "bit_utils.h"
#include "unit_test_check.h"

bool test_cli(void) {
  char cmd_line[20]="gl rst";
  int argc=0;

  char* shell_argv[SHELL_MAX_ARG_COUNT];
  EXPECT_TRUE(cli_parse_args( cmd_line, &argc, &shell_argv[0]));
  EXPECT_EQ(2,argc);
  EXPECT_STREQ("gl", shell_argv[0]);
  EXPECT_STREQ("rst", shell_argv[1]);
  return true;
}

