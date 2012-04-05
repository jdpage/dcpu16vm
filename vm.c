#include <stdlib.h>
#include <string.h>
#include "codes.h"
#include "vm.h"

#define THREE_BITS 0x07
#define FOUR_BITS 0x0f
#define SIX_BITS 0x3f

#define ALLOC(type, size) ((type *)malloc((size) * sizeof(type)))

vm_cpu_t *vm_cpu_new() {
    vm_cpu_t *cpu;
    int k;

    if ((cpu = ALLOC(vm_cpu_t, 1)) == NULL) {
        /* out of memory! we're pretty much done here. */
        return NULL;
    }

    /* zero RAM */
    memset(cpu->ram, VM_RAM_INIT, sizeof(uint16_t) * VM_RAMSIZE);

    /* zero registers */
    for (k = 0; k < 8; k++) {
        cpu->reg[k] = VM_REG_INIT;
    }
    cpu->pc = VM_PROG_START;
    cpu->sp = VM_STACK_START;
    cpu->o = VM_REG_INIT;

    return cpu;
}

uint16_t *get_ptr(vm_cpu_t *cpu, uint16_t aaaaaa) {
    static uint16_t temp_a = 0, temp_b = 0, ticker = 0;
    uint16_t *temp;

    /* 
     * since every opcode has at most two fields, we can magic two literals
     * with static variables we alternate between.
     *
     * you might choose to call this a dirty hack.
     */
    if (!ticker) {
        temp = &temp_a;
    } else {
        temp = &temp_b;
    }
    ticker = !ticker;

    if (aaaaaa < MEM_AT) {
        return cpu->reg + aaaaaa;
    } else if (aaaaaa < NW_MEM_AT) {
        return cpu->ram + cpu->reg[aaaaaa & THREE_BITS];
    } else if (aaaaaa < POP) {
        return cpu->ram + cpu->ram[++cpu->pc] + cpu->reg[aaaaaa & THREE_BITS];
    }
    switch (aaaaaa) {
        case POP:
            return cpu->ram + (cpu->sp++);
        case PEEK:
            return cpu->ram + (cpu->sp);
        case PUSH:
            return cpu->ram + (--cpu->sp);
        case REG_SP:
            return &cpu->sp;
        case REG_PC:
            return &cpu->pc;
        case REG_O:
            return &cpu->o;
        case NW:
            return cpu->ram + cpu->ram[++cpu->pc];
        case LIT_NW:
            return cpu->ram + (++cpu->pc);
    }
    *temp = aaaaaa ^ LIT;
    return temp;
}

int vm_cpu_step(vm_cpu_t *cpu) {
    uint16_t word, opcode, *dest, *src;
    uint32_t left, right, out;

    /* In bits, a basic instruction has the format: bbbbbbaaaaaaoooo */
    /* a nonbasic one has the format: aaaaaaoooooo0000 */
    word = cpu->ram[cpu->pc];
    opcode = word & FOUR_BITS;
    dest  = get_ptr(cpu, (word >>  4) & SIX_BITS);
    src   = get_ptr(cpu, (word >> 10) & SIX_BITS);

    if (opcode != NONBAS) {
        left  = *dest;
        right = *src;

        switch (opcode) {
            case SET:
                *dest = *src;
                break;
            case ADD:
                out = left + right;
                *dest = (uint16_t)out;
                cpu->o = (out == *dest) ? 0x0 : 0x0001;
                break;
            case SUB:
                out = left - right;
                *dest = (uint16_t)out;
                cpu->o = (out == *dest) ? 0x0 : 0xffff;
                break;
            case MUL:
                out = left * right;
                *dest = (uint16_t)out;
                cpu->o = (out >> 16) & 0xffff;
                break;
            case DIV:
                if (right == 0) {
                    *dest = 0;
                    cpu->o = 0;
                } else {
                    *dest = (uint16_t)(left / right);
                    cpu->o = ((left << 16) / right) & 0xffff;
                }
                break;
            case MOD:
                *dest = (right != 0) ? left % right : 0;
                break;
            case SHL:
                out = left << right;
                *dest = (uint16_t)out;
                cpu->o = (out >> 16) & 0xffff;
                break;
            case SHR:
                out = left >> right;
                *dest = (uint16_t)out;
                cpu->o = ((left << 16) >> right) & 0xffff;
                break;
            case AND:
                *dest &= *src;
                break;
            case BOR:
                *dest |= *src;
                break;
            case XOR:
                *dest ^= *src;
                break;
            case IFE:
                if (!(left == right)) {
                    cpu->pc++;
                }
                break;
            case IFN:
                if (!(left == right)) {
                    cpu->pc++;
                }
                break;
            case IFG:
                if (!(left > right)) {
                    cpu->pc++;
                }
                break;
            case IFB:
                if (!((left & right) != 0)) {
                    cpu->pc++;
                }
                break;
        }
    } else {
        /* nonbasic opcode */
        opcode = (word >> 4) & SIX_BITS;
        switch (opcode) {
            case JSR:
                cpu->ram[cpu->sp++] = cpu->pc + 1;
                cpu->pc = *src;
                break;
            default:
                return VM_BADOP;
        }
    }
    cpu->pc++;
    return VM_OK;
}
