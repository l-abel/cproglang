/* this program forms the basis on which exercises 4-3 through 4-10 build */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> /* for atof() - in K&R, math.h is referenced - this is an anachronism */

#define BUFSIZE 100
#define MAXOP 100 /* max size of operand or operator */
#define MAXVAL  100 /* maximum depth of val stack */
#define NUMBER '0' /* signal that a number was found */

int getop(char []);
void push(double);
double pop(void);
double getTop(void);
void swapTop(void);
void clearStack(void);
int getch(void);
void ungetch(int);

/* reverse Polish calculator */

int main(void)
{
  int type;
  double op1;
  double op2;
  char s[MAXOP];

  while((type = getop(s)) != EOF)
  {
    switch(type)
    {
      case NUMBER:
        push(atof(s));
        break;
      case '+':
        push(pop() + pop());
        break;
      case '*':
        push(pop() * pop());
        break;
      case '-':
        op2 = pop();
        push(pop() - op2);
        break;
      case '/':
        op2 = pop();
        if(op2 != 0.0)
          push(pop() / op2);
        else
          printf("divide error: zero divisor\n");
        break;
      case '%':
        op2 = pop();
        if (op2 != 0.0)
          push(fmod(pop(), op2));
        else
          printf("modulo error: zero divisor\n");
        break;
      case '^':
        op1 = pop();
        op2 = pop();
        if ((op1 == 0 && op2 < 0)
            || (op1 < 0 && !isdigit(op2))) {
          printf("pow error:  invalid expression");
        } else {
          push(pow(op1, op2));
        }
        break;
      case '$':
        push(sin(pop()));
        break;
      case 'e':
        push(exp(pop()));
        break;
      case '?':
        printf("current top: %f\n", getTop());
        break;
      case '@':
        push(getTop());
        break;
      case '~':
        swapTop();
        break;
      case '!':
        clearStack();
        break;
      case '\n':
        printf("\t%.8g\n", pop());
        break;
      default:
        printf("error: unknown command %s\n", s);
        break;
    }
  }

  return 0;
}


int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */

/* push: push f onto value stack */
void push(double f)
{
  if(sp < MAXVAL)
    val[sp++] = f;
  else
    printf("push error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
  if (sp > 0) {
    return val[--sp];
  } else {
    printf("pop error: stack empty\n");
    return 0.0;
  }
}

double getTop(void) {
  if (sp > 0) {
    return val[sp - 1];
  } else {
    printf("getTop error: stack empty\n");
    return 0.0;
  }
}

void swapTop(void) {
  if (sp > 1) {
    double top = pop();
    double next = pop();
    push(top);
    push(next);
  } else {
    printf("swapTop error: not enough elements in stack");
  }
}

void clearStack(void) {
  sp = 0; // not safe!
}

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand */
int getop(char s[])
{
  int i, c, sign;

  // skip ws
  while((s[0] = c = getch()) == ' ' || c == '\t')
    ;

  s[1] = '\0';
  if(!isdigit(c) && c != '.' && c != '-')
    return c; /* not a number */

  i = 0;
  if(c == '-') {
    if (isdigit(s[++i] = c = getch())) {
      while(isdigit(s[++i] = c = getch()))
        ;
    } else {
      ungetch(c);
      return '-';
    }
  }
  if(isdigit(c)) /* collect integer part */
    while(isdigit(s[++i] = c = getch()))
      ;
  if(c == '.')
    while(isdigit(s[++i] = c = getch()))
      ;
  s[i] = '\0';

  if(c != EOF)
    ungetch(c);
  return NUMBER;
}

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
  if(bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

