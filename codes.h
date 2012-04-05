#ifndef CODES_H
#define CODES_H

/* general purpose registers */
#define REG_A 0x00
#define REG_B 0x01
#define REG_C 0x02
#define REG_X 0x03
#define REG_Y 0x04
#define REG_Z 0x05
#define REG_I 0x06
#define REG_J 0x07

/* OR these with register codes above */
#define MEM_AT 0x08    /* 0x08-0x0f: [register] */
#define NW_MEM_AT 0x10 /* 0x10-0x17: [next word + register] */

/* stack magic */
#define POP 0x18  /* POP / [SP++] */
#define PEEK 0x19 /* PEEK / [SP] */
#define PUSH 0x1a /* PUSH / [--SP] */

/* special registers */
#define REG_SP 0x1b
#define REG_PC 0x1c
#define REG_O 0x1d

/* opcode magic */
#define NW 0x1e     /* [next word] */
#define LIT_NW 0x1f /* next word (literal) */
#define LIT 0x20    /* literal value 0x00-0x1f (OR with value) */

/* basic opcodes */
#define NONBAS 0x0
#define SET 0x1 /* SET a, b - sets a to b */
#define ADD 0x2 /* ADD a, b - sets a to a+b, sets O to 0x0001 if there's an overflow, 0x0 otherwise */
#define SUB 0x3 /* SUB a, b - sets a to a-b, sets O to 0xffff if there's an underflow, 0x0 otherwise */
#define MUL 0x4 /* MUL a, b - sets a to a*b, sets O to ((a*b)>>16)&0xffff */
#define DIV 0x5 /* DIV a, b - sets a to a/b, sets O to ((a<<16)/b)&0xffff. if b==0, sets a and O to 0 instead. */
#define MOD 0x6 /* MOD a, b - sets a to a%b. if b==0, sets a to 0 instead. */
#define SHL 0x7 /* SHL a, b - sets a to a<<b, sets O to ((a<<b)>>16)&0xffff */
#define SHR 0x8 /* SHR a, b - sets a to a>>b, sets O to ((a<<16)>>b)&0xffff */
#define AND 0x9 /* AND a, b - sets a to a&b */
#define BOR 0xa /* BOR a, b - sets a to a|b */
#define XOR 0xb /* XOR a, b - sets a to a^b */
#define IFE 0xc /* IFE a, b - performs next instruction only if a==b */
#define IFN 0xd /* IFN a, b - performs next instruction only if a!=b */
#define IFG 0xe /* IFG a, b - performs next instruction only if a>b */
#define IFB 0xf /* IFB a, b - performs next instruction only if (a&b)!=0 */

/* nonbasic opcodes */
/* 0x00: reserved for future expansion */
#define JSR 0x01 /* JSR a - pushes the address of the next instruction to the stack, then sets PC to a */
/* 0x02-0x3f: reserved */

#endif
