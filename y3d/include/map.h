/*
* Copyright (C) 2012 William Swanson
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the names of the authors or
* their institutions shall not be used in advertising or otherwise to
* promote the sale, use or other dealings in this Software without
* prior written authorization from the authors.
*/

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#define EXPAND(x) x
#define FOR_EACH_1(what, x, ...) what(x)
#define FOR_EACH_2(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_1(what,  __VA_ARGS__))
#define FOR_EACH_3(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_2(what, __VA_ARGS__))
#define FOR_EACH_4(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_3(what,  __VA_ARGS__))
#define FOR_EACH_5(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_4(what,  __VA_ARGS__))
#define FOR_EACH_6(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_5(what,  __VA_ARGS__))
#define FOR_EACH_7(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_6(what,  __VA_ARGS__))
#define FOR_EACH_8(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_7(what,  __VA_ARGS__))
#define FOR_EACH_9(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_8(what,  __VA_ARGS__))
#define FOR_EACH_10(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_9(what,  __VA_ARGS__))
#define FOR_EACH_11(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_10(what,  __VA_ARGS__))
#define FOR_EACH_12(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_11(what,  __VA_ARGS__))
#define FOR_EACH_13(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_12(what,  __VA_ARGS__))
#define FOR_EACH_14(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_13(what,  __VA_ARGS__))
#define FOR_EACH_15(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_14(what,  __VA_ARGS__))
#define FOR_EACH_16(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_15(what,  __VA_ARGS__))
#define FOR_EACH_17(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_16(what,  __VA_ARGS__))
#define FOR_EACH_18(what, x, ...)\
  what(x);\
  EXPAND(FOR_EACH_17(what,  __VA_ARGS__))
#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) EXPAND(FOR_EACH_ARG_N(__VA_ARGS__))
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define FOR_EACH_RSEQ_N() 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define CONCATENATE(x,y) x##y
#define FOR_EACH_(N, what, ...) EXPAND(CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__))
#define FOR_EACH(what, ...) FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, __VA_ARGS__)

#endif