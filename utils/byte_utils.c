#include "byte_utils.h"

#include <stdint.h>

#include "data_utils.h"

uint64_t reverse_byte_order_uint64(const uint64_t in4byteVal) {
  uint64_t out_val = in4byteVal;
  out_val = (out_val & 0x00000000FFFFFFFF) << 32 |
            (out_val & 0xFFFFFFFF00000000) >> 32;
  out_val = (out_val & 0x0000FFFF0000FFFF) << 16 |
            (out_val & 0xFFFF0000FFFF0000) >> 16;
  out_val =
      (out_val & 0x00FF00FF00FF00FF) << 8 | (out_val & 0xFF00FF00FF00FF00) >> 8;
  return out_val;
}

uint32_t reverse_byte_order_uint32(const uint32_t in4byteVal) {
  uint32_t retval;
  retval = in4byteVal & 0xFF;
  retval = (retval << 8) | ((in4byteVal >> 8) & 0xFF);
  retval = (retval << 8) | ((in4byteVal >> 16) & 0xFF);
  retval = (retval << 8) | ((in4byteVal >> 24) & 0xFF);
  return retval;
}

uint32_t reverse_byte_order_uint24(const uint32_t in3byteVal) {
  Type32Union_t u32val_in,u32val_out;

  u32val_in.u32=in3byteVal;

  u32val_out.u8[0]=u32val_in.u8[2];
  u32val_out.u8[1]=u32val_in.u8[1];
  u32val_out.u8[2]=u32val_in.u8[0];
  u32val_out.u8[3]=0x00;

  return u32val_out.u32;
}

uint16_t reverse_byte_order_uint16(const uint16_t in2byteVal) {
  uint16_t swapped = 0;
  swapped = (in2byteVal >> 8) | (in2byteVal << 8);
  return swapped;
}
