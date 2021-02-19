#include <fixedptc.h>
#include <stdio.h>

char str[31];

char * to_str(fixedpt A) {
    fixedpt_str(A, str, -1);
    return str;
}

int main() {
    fixedpt a = fixedpt_rconst(13.25);
    fixedpt b = fixedpt_fromint(100);
    fixedpt res = fixedpt_add(a, b);
    
    printf("a + b = %s\n", to_str(res));
    printf("floor(res) = %s\n", to_str(fixedpt_floor(res)));
    printf("ceil(res) = %s\n", to_str(fixedpt_ceil(res)));

    fixedpt d = 0;
    fixedpt e = 1 << 31;
    printf("floor(d) = %s\n", to_str(fixedpt_floor(d)));
    printf("ceil(d) = %s\n", to_str(fixedpt_ceil(d)));
    printf("floor(e) = %s\n", to_str(fixedpt_floor(e)));
    printf("ceil(e) = %s\n", to_str(fixedpt_ceil(e)));

    printf("a * b = %s\n", to_str(fixedpt_mul(a, b)));
    printf("a / b = %s\n", to_str(fixedpt_div(a, b)));
    printf("a / 3 = %s\n", to_str(fixedpt_divi(a, 3)));
    printf("b / 3 = %s\n", to_str(fixedpt_divi(b, 3)));
    printf("a * 3 = %s\n", to_str(fixedpt_muli(a, 3)));
    printf("b * 3 = %s\n", to_str(fixedpt_muli(b, 3)));
    fixedpt f = fixedpt_rconst(-5.1);
    printf("abs(f) = %s\n", to_str(fixedpt_abs(f)));
    printf("abs(a) = %s\n", to_str(fixedpt_abs(a)));
    printf("floor(10.1) = %s\n", to_str(fixedpt_floor(fixedpt_rconst(10.1))));
    printf("floor(10.0) = %s\n", to_str(fixedpt_floor(fixedpt_rconst(10.0))));
    printf("floor(-10.1) = %s\n", to_str(fixedpt_floor(fixedpt_rconst(-10.1))));
    printf("floor(-10.0) = %s\n", to_str(fixedpt_floor(fixedpt_rconst(-10.0))));

    printf("ceil(10.1) = %s\n", to_str(fixedpt_ceil(fixedpt_rconst(10.1))));
    printf("ceil(10.0) = %s\n", to_str(fixedpt_ceil(fixedpt_rconst(10.0))));
    printf("ceil(-10.1) = %s\n", to_str(fixedpt_ceil(fixedpt_rconst(-10.1))));
    printf("ceil(-10.0) = %s\n", to_str(fixedpt_ceil(fixedpt_rconst(-10.0))));

}