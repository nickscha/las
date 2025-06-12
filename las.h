/* las.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) lightweight assembler for x86_64.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef LAS_H
#define LAS_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define LAS_INLINE inline
#define LAS_API extern
#elif defined(__GNUC__) || defined(__clang__)
#define LAS_INLINE __inline__
#define LAS_API static
#elif defined(_MSC_VER)
#define LAS_INLINE __inline
#define LAS_API static
#else
#define LAS_INLINE
#define LAS_API static
#endif

typedef struct las_instruction
{
  char *mnemonic;
  unsigned char encoding[10];
  int length;

} las_instruction;

static las_instruction las_instruction_table[] = {
    /* Control  */
    {"cli", {0xFA}, 1},
    {"sti", {0xFB}, 1},
    {"hlt", {0xF4}, 1},
    {"nop", {0x90}, 1},
    {"ret", {0xC3}, 1},
    {"syscall", {0x0F, 0x05}, 2},

    /*  Stack */
    {"push rax", {0x50}, 1},
    {"push rbx", {0x53}, 1},
    {"push rcx", {0x51}, 1},
    {"push rdx", {0x52}, 1},
    {"push rbp", {0x55}, 1},
    {"push rsi", {0x56}, 1},
    {"push rdi", {0x57}, 1},
    {"push rsp", {0x54}, 1},
    {"pop rax", {0x58}, 1},
    {"pop rbx", {0x5B}, 1},
    {"pop rcx", {0x59}, 1},
    {"pop rdx", {0x5A}, 1},
    {"pop rbp", {0x5D}, 1},
    {"pop rsi", {0x5E}, 1},
    {"pop rdi", {0x5F}, 1},
    {"pop rsp", {0x5C}, 1},

    /* Arithmetic / Logical */
    {"xor rax, rax", {0x48, 0x31, 0xC0}, 3},
    {"xor rcx, rcx", {0x48, 0x31, 0xC9}, 3},
    {"xor rdx, rdx", {0x48, 0x31, 0xD2}, 3},
    {"add rax, rbx", {0x48, 0x01, 0xD8}, 3},
    {"sub rax, rbx", {0x48, 0x29, 0xD8}, 3},
    {"inc rax", {0x48, 0xFF, 0xC0}, 3},
    {"dec rax", {0x48, 0xFF, 0xC8}, 3},

    /* Simple Moves */
    {"mov rax, rbx", {0x48, 0x89, 0xD8}, 3},
    {"mov rbx, rax", {0x48, 0x89, 0xC3}, 3},
    {"mov rcx, rax", {0x48, 0x89, 0xC1}, 3},
    {"mov rax, rcx", {0x48, 0x89, 0xC8}, 3},

    /* Return and Call */
    {"call rax", {0xFF, 0xD0}, 2},
    {"jmp rax", {0xFF, 0xE0}, 2},
    {"jmp rcx", {0xFF, 0xE1}, 2},

    {0, {0}, 0}};

LAS_API LAS_INLINE unsigned long las_strlen(char *s)
{
  unsigned long len = 0;
  while (*s++)
  {
    len++;
  }
  return (len);
}

LAS_API LAS_INLINE void las_parse_code(char *code, unsigned long code_length)
{
  (void)code;
  (void)code_length;
  (void)las_instruction_table;
}

#endif /* LAS_H */

/*
   ------------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2025 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
