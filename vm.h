/*
 * vm.h
 * Copyright 2012 Jonathan Page. All rights reserved.
 *
 * Interface for vm.c
 */

#ifndef VM_H
#define VM_H

#include <stdint.h>

#define VM_REG_INIT 0x0
#define VM_RAM_INIT 0x0
#define VM_RAMSIZE 0x10000
#define VM_STACK_START 0xffff
#define VM_PROG_START 0x0000

/* error codes */
#define VM_OK 0
#define VM_DONE 1
#define VM_BADOP 2

typedef struct vm_cpu {
    uint16_t ram[VM_RAMSIZE];
    uint16_t reg[8]; /* a, b, c, x, y, z, i, j */
    uint16_t pc, sp, o;
} vm_cpu_t;

vm_cpu_t *vm_cpu_new();
int vm_cpu_step(vm_cpu_t *cpu);

#endif
