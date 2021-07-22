/*****************************************************************************
 * @file    stream.h
 *
 * @details This include for compatibility with new MCAL based libraries
 *****************************************************************************/

#ifndef STREAM_H_
#define STREAM_H_

#ifdef HAS_MCAL
#error "This file can't be used in MCAL builds"
#else
#include "ostream.h"
#endif

#endif
