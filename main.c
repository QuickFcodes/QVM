#include "vm.h"

typedef int vm[RAMMAX];

vm mbook={zerod};

int erx[64]={zeros},i=0,iflags=0;
int stack[RAMMAX]={zerod};
int pos=0;
void error(int codes) {
    switch (codes)
    {
    case 0:
        printf("ERROR:Not found reg.\n");
        break;
    case 1:
        printf("ERROR:Not found ram.\n");
        break;
    case 2:
        printf("ERROR:No input file.\n");
        break;
    case 3:
        printf("ERROR:Not found int_codes.\n");
        break;
    default:
        break;
    }
    exit(1);
}
void push(int num) {
    stack[pos] = num;
    pos<RAMMAX?pos++:0;
}
int pop(void) {
    return pos<0?pos++:0,stack[pos--];
}
void ints(int codes) {
    switch(codes) {
        case 10:
            printf("%c",(char)erx[15]);
            break;
        case 15:
            erx[31] = random(erx[32],erx[33]);
            break;
        case 20:
            erx[34] = getchar();
            break;
        default:
            error(3);
    }
}
void eval() {
    for(;i<RAMMAX;i++) {
        switch (mbook[i]) {
        case RET:
            i=pop();
            break;
        case ADD:
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]] += mbook[i+2];
            i+=2;
            break;
        case ADDR://add reg reg
            if(mbook[i+1]>=64)error(0);
            if(mbook[i+2]>=64)error(0);
            erx[mbook[i+1]] += erx[mbook[i+2]];
            i+=2;
            break;
        case MOV://mov reg num
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]] = mbook[i+2];
            i+=2;
            break;
        case MOVR://mov reg reg
            if(mbook[i+1]>=64)error(0);
            if(mbook[i+2]>=64)error(0);
            erx[mbook[i+1]] = erx[mbook[i+2]];
            i+=2;
            break;
        case MOVC://mov reg ram
            if(mbook[i+1]>=64)error(0);
            if(mbook[erx[mbook[i+2]]]>=RAMMAX)error(1);
            erx[mbook[i+1]] = mbook[erx[mbook[i+2]]];
            i+=2;
            break;
        case MOVRC:// mov ram reg
            if(mbook[i+1]>=RAMMAX)error(1);
            if(mbook[i+2]>=64)error(0);
            mbook[mbook[i+1]] = erx[mbook[i+2]];
            i+=2;
            break;
        case MOVA://mov reg $
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]]=i;
            i++;
            break;
        case debug:
            if(mbook[i+1]>=64)error(0);
            printf("%d\n",erx[mbook[i+1]]);
            i++;
            break;
        case JMP:
            if(mbook[i+1]>=RAMMAX)error(1);
            i=mbook[i+1];
            i--;
            break;
        case JMR:
            if(erx[mbook[i+2]]>=RAMMAX)error(0);
            i=erx[mbook[i+1]];
            i--;
            break;
        case CALL:
            if(mbook[i+1]>=RAMMAX)error(1);
            push(i+2);
            i=mbook[i+1];
            i--;
            break;
        case CALLC:
            if(erx[mbook[i+1]]>=RAMMAX)error(1);
            push(i+2);
            i=erx[mbook[i+1]];
            i--;
            break;
        case SUB:
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]] -= mbook[i+2];
            i+=2;
            break;
        case SUBR:
            if(mbook[i+1]>=64)error(0);
            if(mbook[i+2]>=64)error(0);
            erx[mbook[i+1]] -= erx[mbook[i+2]];
            i+=2;
            break;
        case POP:
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]] = pop();
            i++;
            break;
        case PSH:
            if(mbook[i+1]>=64)error(0);
            push(erx[mbook[i+1]]);
            i++;
            break;
        case PSHC://push num
            push(mbook[i+1]);
            i++;
            break;
        case MUL:
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]] *= mbook[i+2];
            i+=2;
            break;
        case MULR:
            if(mbook[i+1]>=64)error(0);
            if(mbook[i+2]>=64)error(0);
            erx[mbook[i+1]] *= erx[mbook[i+2]];
            i+=2;
            break;
        case INC:
            if(mbook[i+1]>=64)error(0);
            erx[mbook[i+1]]++;
            i++;
            break;
        case CMP://cmp reg num
            if(mbook[i+1]>=64)error(0);
            if(erx[mbook[i+1]]==mbook[i+2])iflags=0;
            if(erx[mbook[i+1]]>=mbook[i+2])iflags=1;
            if(erx[mbook[i+1]]<=mbook[i+2])iflags=2;
            i+=2;
            break;
        case CMPR://cmp reg reg
            if(mbook[i+1]>=64)error(0);
            if(erx[mbook[i+1]]==erx[mbook[i+2]])iflags=0;
            if(erx[mbook[i+1]]>=erx[mbook[i+2]])iflags=1;
            if(erx[mbook[i+1]]<=erx[mbook[i+2]])iflags=2;
            i+=2;
            break;
        case JAE:
            if(mbook[i+1]>=RAMMAX)error(1);
            if(iflags==0)i=mbook[i+1];
            i--;
            break;
        case JQE:
            if(mbook[i+1]>=RAMMAX)error(1);
            if(iflags==1)i=mbook[i+1];
            i--;
            break;
        case JKE:
            if(mbook[i+1]>=RAMMAX)error(1);
            if(iflags==2)i=mbook[i+1];
            i--;
            break;
        case JNE:
            if(mbook[i+1]>=RAMMAX)error(1);
            if(iflags!=0)i=mbook[i+1];
            i--;
            break;
        case JCE:
            if(mbook[i+1]>=RAMMAX)error(1);
            if((iflags==1) || (iflags==0))i=mbook[i+1];
            i--;
            break;
        case JIE:
            if(mbook[i+1]>=RAMMAX)error(1);
            if((iflags==2) || (iflags==0))i=mbook[i+1];
            i--;
            break;
        case INT:
            ints(mbook[i+1]);
            i++;
            break;
        case EXITC:
            exit(0);
        default:
            break;
        }
        continue;
    }
}
int main(int argc,char ** argv) {
    loader(mbook,argv[1]);
    eval();
}
