/* efb.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) executable file/format builder (EFB).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef EFB_H
#define EFB_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define EFB_INLINE inline
#define EFB_API extern
#elif defined(__GNUC__) || defined(__clang__)
#define EFB_INLINE __inline__
#define EFB_API static
#elif defined(_MSC_VER)
#define EFB_INLINE __inline
#define EFB_API static
#else
#define EFB_INLINE
#define EFB_API static
#endif

typedef int efb_bool;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef EFB_MAX_EXE_SIZE
#define EFB_MAX_EXE_SIZE 8192
#endif

#define EFB_ALIGN_UP(val, align) (((val) + (align) - 1) & ~((align) - 1))

EFB_API EFB_INLINE void efb_zero_memory(unsigned char *buffer, unsigned long size)
{
  unsigned long i;
  for (i = 0; i < size; ++i)
  {
    buffer[i] = 0;
  }
}

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

EFB_API EFB_INLINE efb_bool efb_build_executable(char *out_file_name, unsigned char *text_section, unsigned long text_section_size)
{
  unsigned char efb_buffer[EFB_MAX_EXE_SIZE];

  HANDLE hFile;
  DWORD bytes_written;

  /* Constants */
  unsigned long file_align = 0x200;
  unsigned long section_align = 0x1000;
  unsigned long code_va = 0x1000;
  unsigned long entry_point_rva = code_va;
  unsigned short machine_type = IMAGE_FILE_MACHINE_AMD64;

  /* Header sizes */
  long nt_headers_offset = 0x40;
  unsigned long raw_size = EFB_ALIGN_UP(text_section_size, file_align);
  unsigned long virtual_size = EFB_ALIGN_UP(text_section_size, section_align);
  unsigned long size_of_headers = EFB_ALIGN_UP(0x200, file_align);
  unsigned long size_of_image = EFB_ALIGN_UP(code_va + virtual_size, section_align);
  unsigned long file_size = size_of_headers + raw_size;

  IMAGE_DOS_HEADER *dos;
  IMAGE_NT_HEADERS64 *nt;
  IMAGE_SECTION_HEADER *section;

  unsigned char *code_dest;
  unsigned long i;

  efb_bool ended = false;

  if (!out_file_name || !text_section)
  {

    return (ended);
  }

  /* Fail if file_size exceeds static buffer*/
  if (file_size > EFB_MAX_EXE_SIZE)
  {
    return (ended);
  }

  efb_zero_memory(efb_buffer, file_size);

  /* === DOS Header === */
  dos = (IMAGE_DOS_HEADER *)efb_buffer;
  dos->e_magic = 0x5A4D; /* 'MZ' */
  dos->e_lfanew = nt_headers_offset;

  /* === NT Headers === */
  nt = (IMAGE_NT_HEADERS64 *)(efb_buffer + nt_headers_offset);
  nt->Signature = 0x00004550; /* 'PE\0\0' */

  nt->FileHeader.Machine = machine_type;
  nt->FileHeader.NumberOfSections = 1;
  nt->FileHeader.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER64);
  nt->FileHeader.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_RELOCS_STRIPPED | IMAGE_FILE_LARGE_ADDRESS_AWARE;

  nt->OptionalHeader.Magic = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
  nt->OptionalHeader.MajorLinkerVersion = 14;
  nt->OptionalHeader.MinorLinkerVersion = 0;
  nt->OptionalHeader.SizeOfCode = raw_size;
  nt->OptionalHeader.AddressOfEntryPoint = entry_point_rva;
  nt->OptionalHeader.BaseOfCode = code_va;
  nt->OptionalHeader.ImageBase = ((DWORD64)1 << 32) | 0x40000000; /* 0x140000000 */
  nt->OptionalHeader.SectionAlignment = section_align;
  nt->OptionalHeader.FileAlignment = file_align;
  nt->OptionalHeader.MajorOperatingSystemVersion = 6;
  nt->OptionalHeader.MinorOperatingSystemVersion = 0;
  nt->OptionalHeader.MajorSubsystemVersion = 6;
  nt->OptionalHeader.MinorSubsystemVersion = 0;
  nt->OptionalHeader.SizeOfImage = size_of_image;
  nt->OptionalHeader.SizeOfHeaders = size_of_headers;
  nt->OptionalHeader.Subsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
  nt->OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;

  /* === Section Header ===*/
  section = (IMAGE_SECTION_HEADER *)((unsigned char *)&nt->OptionalHeader + nt->FileHeader.SizeOfOptionalHeader);
  section->Name[0] = '.';
  section->Name[1] = 't';
  section->Name[2] = 'e';
  section->Name[3] = 'x';
  section->Name[4] = 't';
  section->Misc.VirtualSize = text_section_size;
  section->VirtualAddress = code_va;
  section->SizeOfRawData = raw_size;
  section->PointerToRawData = size_of_headers;
  section->Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;

  /* === Write Code ===*/
  code_dest = efb_buffer + size_of_headers;
  for (i = 0; i < text_section_size; ++i)
  {
    code_dest[i] = text_section[i];
  }

  /* === Write to File === */
  hFile = CreateFileA(out_file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    return (ended);
  }

  ended = (WriteFile(hFile, efb_buffer, file_size, &bytes_written, 0) != 0) && (bytes_written == file_size);
  ended = CloseHandle(hFile);

  return (ended);
}

#endif /* EFB_H */

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
