#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

void load_file(vm_cpu_t *cpu, FILE *fp) {
    int c1, c2, ptr = 0;
    uint16_t word;

    while ((c1 = fgetc(fp)) != EOF && (c2 = fgetc(fp)) != EOF) {
        word = (c1 << 8) + c2;
        cpu->ram[ptr++] = word;
    }
}

void maybe_get_char(vm_cpu_t *cpu) {
    cpu = cpu;
    // TODO: implement me
}

void remap_video_mem(vm_cpu_t *cpu, FILE *fp) {
    int k;
    uint16_t c;
    /* clear screen */
    fprintf(fp, "\033[2J\033[H");
    for (k = 0; k < 80 * 25; k++) {
        c = cpu->ram[0x8000 + k];
        if (c == 0) {
            fprintf(fp, " ");
        } else {
            fprintf(fp, "%c", c);
        }
        if (k % 80 == 79) {
            fprintf(fp, "\n");
        }
    }
}

int main(int argc, char **argv) {
    vm_cpu_t *cpu;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", argv[1]);
        return 1;
    }

    cpu = vm_cpu_new();
    load_file(cpu, fp);
    while (vm_cpu_step(cpu) != VM_DONE) {
        maybe_get_char(cpu);
        remap_video_mem(cpu, stdout);
    }
    return 0;
}
