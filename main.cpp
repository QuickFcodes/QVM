#include "vm.h"
#include <vector>
typedef std::vector<int> vm;

vm mbook;

int erx[64] = {zeros}, i{0}, iflags{0};
vm stack;
void load(const char name[])
{
    FILE *fp = fopen(name, "rb");
    if (fp == NULL)
        error(4);
    int num1 = 0;
    while ((fscanf(fp, "%d\n", &num1)) != EOF)
        mbook.push_back(num1);
    fclose(fp);
    return;
}
void error(int codes)
{
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
    case 4:
        printf("ERROR:Not found file.\n");
        break;
    case 5:
        printf("ERROR:Too much parameters.\n");
        break;
    default:
        break;
    }
    exit(1);
}
void push(int num)
{
    stack.push_back(num);
}
int pop(void)
{
    int c = stack[stack.size() - 1];
    return stack.pop_back(), c;
}
void ints(int codes)
{
    switch (codes)
    {
    case 10:
        printf("%c", (char)erx[15]);
        break;
    case 15:
        erx[31] = random(erx[32], erx[33]);
        break;
    case 20:
        erx[34] = getchar();
        break;
    default:
        error(3);
    }
}
void eval()
{
    for (; i < RAMMAX; i++)
    {
        switch (mbook[i])
        {
        case RET:
            i = pop();
            break;
        case ADD:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] += mbook[i + 2];
            i += 2;
            break;
        case ADDR: // add reg reg
            if (mbook[i + 1] >= 64)
                error(0);
            if (mbook[i + 2] >= 64)
                error(0);
            erx[mbook[i + 1]] += erx[mbook[i + 2]];
            i += 2;
            break;
        case MOV: // mov reg num
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = mbook[i + 2];
            i += 2;
            break;
        case MOVR: // mov reg reg
            if (mbook[i + 1] >= 64)
                error(0);
            if (mbook[i + 2] >= 64)
                error(0);
            erx[mbook[i + 1]] = erx[mbook[i + 2]];
            i += 2;
            break;
        case MOVC: // mov reg ram
            if (mbook[i + 1] >= 64)
                error(0);
            if (mbook[erx[mbook[i + 2]]] >= RAMMAX)
                error(1);
            erx[mbook[i + 1]] = mbook[erx[mbook[i + 2]]];
            i += 2;
            break;
        case MOVRC: // mov ram reg
            if (mbook[i + 1] >= RAMMAX)
                error(1);
            if (mbook[i + 2] >= 64)
                error(0);
            mbook[mbook[i + 1]] = erx[mbook[i + 2]];
            i += 2;
            break;
        case MOVA: // mov reg $
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = i;
            i++;
            break;
        case debug:
            if (mbook[i + 1] >= 64)
                error(0);
            printf("%d\n", erx[mbook[i + 1]]);
            i++;
            break;
        case JMP:
            if (mbook[i + 1] >= RAMMAX)
                error(1);
            i = mbook[i + 1];
            i--;
            break;
        case JMR:
            if (erx[mbook[i + 2]] >= RAMMAX)
                error(0);
            i = erx[mbook[i + 1]];
            i--;
            break;
        case CALL:
            if (mbook[i + 1] >= RAMMAX)
                error(1);
            push(i + 2);
            i = mbook[i + 1];
            i--;
            break;
        case CALLC:
            if (erx[mbook[i + 1]] >= RAMMAX)
                error(1);
            push(i + 2);
            i = erx[mbook[i + 1]];
            i--;
            break;
        case SUB:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] -= mbook[i + 2];
            i += 2;
            break;
        case SUBR:
            if (mbook[i + 1] >= 64)
                error(0);
            if (mbook[i + 2] >= 64)
                error(0);
            erx[mbook[i + 1]] -= erx[mbook[i + 2]];
            i += 2;
            break;
        case POP:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = pop();
            i++;
            break;
        case PSH:
            if (mbook[i + 1] >= 64)
                error(0);
            push(erx[mbook[i + 1]]);
            i++;
            break;
        case PSHC: // push num
            push(mbook[i + 1]);
            i++;
            break;
        case MUL:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] *= mbook[i + 2];
            i += 2;
            break;
        case MULR:
            if (mbook[i + 1] >= 64)
                error(0);
            if (mbook[i + 2] >= 64)
                error(0);
            erx[mbook[i + 1]] *= erx[mbook[i + 2]];
            i += 2;
            break;
        case DIV:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] /= mbook[i + 2];
            i += 2;
            break;
        case DIVR:
            if (mbook[i + 1] >= 64)
                error(0);
            if (mbook[i + 2] >= 64)
                error(0);
            erx[mbook[i + 1]] /= erx[mbook[i + 2]];
            i += 2;
            break;
        case INC:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]]++;
            i++;
            break;
        case CMP: // cmp reg num
            if (mbook[i + 1] >= 64)
                error(0);
            if (erx[mbook[i + 1]] == mbook[i + 2])
            {
                iflags = 0;
                goto t;
            }
            if (erx[mbook[i + 1]] > mbook[i + 2])
            {
                iflags = 1;
                goto t;
            }
            if (erx[mbook[i + 1]] < mbook[i + 2])
            {
                iflags = 2;
                goto t;
            }
        t:
            i += 2;
            break;
        case CMPR: // cmp reg reg
            if (mbook[i + 1] >= 64)
                error(0);
            if (erx[mbook[i + 1]] == erx[mbook[i + 2]])
            {
                iflags = 0;
                goto ell;
            }
            if (erx[mbook[i + 1]] > erx[mbook[i + 2]])
            {
                iflags = 1;
                goto ell;
            }
            if (erx[mbook[i + 1]] < erx[mbook[i + 2]])
            {
                iflags = 2;
                goto ell;
            }
        ell:
            i += 2;
            break;
        case JAE:
            if (mbook[i + 1] >= RAMMAX)
            {
                error(1);
            }
            if (iflags == 0)
            {
                i = mbook[i + 1];
                i--;
                continue;
            }
            break;
        case JQE:
            if (mbook[i + 1] >= RAMMAX)
            {
                error(1);
            }
            if (iflags == 1)
            {
                i = mbook[i + 1];
                i--;
                continue;
            }
            break;
        case JKE:
            if (mbook[i + 1] >= RAMMAX)
            {
                error(1);
            }
            if (iflags == 2)
            {
                i = mbook[i + 1];
                i--;
                continue;
            }
            break;
        case JNE:
            if (mbook[i + 1] >= RAMMAX)
            {
                error(1);
            }
            if (iflags != 0)
            {
                i = mbook[i + 1];
                i--;
                continue;
            }
            break;
        case JCE:
            if (mbook[i + 1] >= RAMMAX)
            {
                error(1);
            }
            if ((iflags == 1) || (iflags == 0))
            {
                i = mbook[i + 1];
                i--;
                continue;
            }
            break;
        case JIE:
            if (mbook[i + 1] >= RAMMAX)
            {
                error(1);
            }
            if ((iflags == 2) || (iflags == 0))
            {
                i = mbook[i + 1];
                i--;
                continue;
            }
            break;
        case INT:
            ints(mbook[i + 1]);
            i++;
            break;
        case LEFT:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] <<= mbook[i + 2];
            i += 2;
            break;
        case RIGHT:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] >>= mbook[i + 2];
            i += 2;
            break;
        case AND:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = erx[mbook[i + 1]] & mbook[i + 2];
            i += 2;
            break;
        case OR:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = erx[mbook[i + 1]] | mbook[i + 2];
            i += 2;
            break;
        case NOT:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = !mbook[i + 2];
            i += 2;
            break;
        case XOR:
            if (mbook[i + 1] >= 64)
                error(0);
            erx[mbook[i + 1]] = erx[mbook[i + 1]] ^ mbook[i + 2];
            i += 2;
            break;
        case MALLOC:
            if (mbook[i + 1] >= 64)
                error(0);
            for (int j; j < erx[mbook[i + 1]]; j++)
            {
                mbook.push_back(0);
            }
            i += 1;
            break;
        case FREE:
            if (mbook[i + 1] >= 64)
                error(0);
            for (int j; j < erx[mbook[i + 1]]; j++)
            {
                mbook.pop_back();
            }
            i += 1;
            break;
        case EXITC:
            exit(0);
        default:
            break;
        }
        continue;
    }
}
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        load(argv[1]);
    }
    else if (argc == 1)
    {
        error(2);
    }
    else
    {
        error(5);
    }
    eval();
}
