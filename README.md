# las
A C89 standard compliant, single header, nostdlib (no C Standard Library) lightweight assembler for x86_64.

For more information please look at the "las.h" file or take a look at the "examples" or "tests" folder.

## Quick Start

Download or clone las.h and include it in your project.

```C
#include "las.h"

int main() {

    char *code = "mov rdi, 1; exit code\nmov rax, 60; syscall: exit\nsyscall";
    unsigned long code_length = las_strlen(code);

    /* parses the assembly to machine code */
    las_parse_code(code, code_length);

    return 0;
}
```
