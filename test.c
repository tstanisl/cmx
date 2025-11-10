#include "cmx.h"
#include <stdio.h>

int main() {
    auto m = cmxe4m3_encode( 21.37 );
    printf("%g\n", cmxe4m3_decode( m ));
}
