## Overview.
Single-header C library [OCP microscaling (MX) formats](https://www.opencompute.org/documents/ocp-microscaling-formats-mx-v1-0-spec-final-pdf).


## Example.

```c
#include "cmx.h"
#include <stdio.h>

int main() {
    auto a = mx_encode(cmxe5m2, 21.37);
    printf("%g\n", mx_decode(a));
}
```
Prints
```
21.5
```

