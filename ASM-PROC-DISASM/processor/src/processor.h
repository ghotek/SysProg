#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

const int REG_AMOUNT = 4;

typedef enum {

    CMD_PUSH = 0x1 ,
    CMD_POP  = 0x2 ,
    CMD_DIF  = 0x3 ,
    CMD_MUL  = 0x4 ,
    CMD_ADD  = 0x5 ,
    CMD_SUB  = 0x6 ,
    CMD_OUT  = 0x7 ,
    CMD_JA   = 0x8 ,
    CMD_JAE  = 0x9 ,
    CMD_JB   = 0xA ,
    CMD_JBE  = 0xB ,
    CMD_JE   = 0xC ,
    CMD_JNE  = 0xD ,
    CMD_JM   = 0xE ,
    CMD_JF   = 0xF ,
    CMD_HLT  = 0x10,
    CMD_IN   = 0x11,
    CMD_CALL = 0x12
} CMD_LIST;

int add(Stack* stk);
int sub(Stack* stk);
int mul(Stack* stk);
int dif(Stack* stk);
int ja(Stack* stk);
int jae(Stack* stk);
int jb(Stack* stk);
int jbe(Stack* stk);
int je(Stack* stk);
int jne(Stack* stk);
int jm();
int jf();

#endif // PROCESSOR_H_INCLUDED
