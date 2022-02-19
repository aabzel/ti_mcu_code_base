#ifndef PARAMS_TEST_H
#define PARAMS_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_param_types(void);
bool test_params(void);

#define TEST_SUIT_PARAM  \
           {"params", test_params}, \
           {"param_types", test_param_types},

#ifdef __cplusplus
}
#endif

#endif /* PARAMS_TEST_H */
