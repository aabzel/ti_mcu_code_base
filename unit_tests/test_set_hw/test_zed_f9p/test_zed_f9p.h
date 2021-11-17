#ifndef TEST_ZED_F9P_H
#define TEST_ZED_F9P_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_zed_f9p_types(void);
bool test_zed_f9p_mon_hw(void);
bool test_zed_f9p_nav_clk(void);
bool test_zed_f9p_nav_status(void);
bool test_zed_f9p_nav_posllh(void);

#define TEST_SUIT_ZED_F9P                     \
    {"zedf9pType", test_zed_f9p_types},      \
    {"zedf9pHw", test_zed_f9p_mon_hw},      \
    {"zedf9pNavStat", test_zed_f9p_nav_status},      \
    {"zedf9pNavPosllh", test_zed_f9p_nav_posllh},      \
    {"zedf9pNavClk", test_zed_f9p_nav_clk},
 
#ifdef __cplusplus
}
#endif

#endif /* TEST_ZED_F9P_H */
