/*
 fly_by_knight_error.h
 Fly by Knight - Chess Engine
 Edward Sandor
 December 2020
 
 Common error checking and reporting for Fly by Knight
*/

#ifndef _FLY_BY_KNIGHT_ERROR_H_
#define _FLY_BY_KNIGHT_ERROR_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define FBK_ERROR_MSG(msg, ...) fprintf(stderr, "# [ERROR] FBK: " msg "\n", ##__VA_ARGS__)
#define FBK_ERROR_MSG_HARD(msg, ...) fprintf(stderr, "# [ERROR] (%s:%d) FBK: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define FBK_ASSERT_MSG(exp, msg, ...) if(!(exp)) {FBK_ERROR_MSG_HARD(msg, ##__VA_ARGS__); assert(exp); }
#define FBK_FATAL_MSG(msg, ...) FBK_ERROR_MSG_HARD(msg, ##__VA_ARGS__); exit(1);

#endif //_FLY_BY_KNIGHT_ERROR_H_