#ifndef DBC_TEST_TABLE_UTILS_H
#define DBC_TEST_TABLE_UTILS_H

#include <stdint.h>

#include "io_utils.h"
#include "stream.h"

#define SIMPLE_TABLES

#ifdef SIMPLE_TABLES
#define TABLE_TOP_LEFT "+"
#define TABLE_TOP_RIGHT "+"
#define TABLE_TOP_SEPARATOR "+"
#define TABLE_TOP_FILL "-"

#define TABLE_SEP_LEFT "+"
#define TABLE_SEP_RIGHT "+"
#define TABLE_SEP_SEPARATOR "+"
#define TABLE_SEP_FILL "-"

#define TABLE_BTM_LEFT "+"
#define TABLE_BTM_RIGHT "+"
#define TABLE_BTM_SEPARATOR "+"
#define TABLE_BTM_FILL "-"

#define TABLE_LEFT "|"
#define TABLE_RIGHT "|"
#define TABLE_SEPARATOR "|"
#define T_SEP "|"
#define TSEP TABLE_SEPARATOR
#define TABLE_FILL " "
#else
#define TABLE_TOP_LEFT "╔"
#define TABLE_TOP_RIGHT "╗"
#define TABLE_TOP_SEPARATOR "╤"
#define TABLE_TOP_FILL "═"

#define TABLE_SEP_LEFT "╟"
#define TABLE_SEP_RIGHT "╢"
#define TABLE_SEP_SEPARATOR "┼"
#define TABLE_SEP_FILL "─"

#define TABLE_BTM_LEFT "╚"
#define TABLE_BTM_RIGHT "╝"
#define TABLE_BTM_SEPARATOR "╧"
#define TABLE_BTM_FILL "═"

#define TABLE_LEFT "║"
#define TABLE_RIGHT "║"
#define TABLE_SEPARATOR "│"
#define TABLE_FILL " "
#endif

typedef struct {
  uint8_t width;
  const char *name;
} table_col_t;

#ifdef __cplusplus
extern "C" {
#endif

void table_row_top(ostream_t *row_top_ostream, const table_col_t row_top_cols[],
                   int16_t row_top_col_count);
void table_row_separator(ostream_t *row_separator_ostream,
                         const table_col_t row_separator_cols[],
                         int16_t row_separator_col_count);
void table_row_bottom(ostream_t *row_bottom_ostream,
                      const table_col_t row_bottom_cols[],
                      int16_t row_bottom_col_count);
void table_row_header(ostream_t *row_header_ostream,
                      const table_col_t row_header_cols[],
                      int16_t row_header_col_count);
void table_header(ostream_t *header_ostream, const table_col_t header_cols[],
                  int16_t header_col_count);

#ifdef __cplusplus
}
#endif

#endif /* DBC_TEST_TABLE_UTILS_H */
