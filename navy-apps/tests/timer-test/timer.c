#include <unistd.h>
#include <stdio.h>
#include <NDL.h>

int main() {
    int pre_sec = NDL_GetTicks() / 1000;
    int present;
    while (1) {
        while (1) {
            present = NDL_GetTicks() / 1000;
            if (present != pre_sec) {
                pre_sec = present;
                break;
            }
        }
        printf("1 second passed\n");
    }
}