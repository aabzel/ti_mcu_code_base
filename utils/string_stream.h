/*****************************************************************************
 * @file    string_stream.h
 *
 * @details This include for compatibility with new MCAL based libraries
 *****************************************************************************/

#ifndef STRING_STREAM_H_
#define STRING_STREAM_H_

#ifdef HAS_MCAL
#error "This file can't be used in MCAL builds"
#else
#include "string_writer.h"
#endif

#endif /* STRING_STREAM_H_ */
