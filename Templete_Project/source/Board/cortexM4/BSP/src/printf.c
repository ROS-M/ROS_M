#include "includes.h"
#include <stdarg.h>

typedef int     bool;
#define true    1
#define false   0

#define SWAP8(A)    (A)
#define SWAP16(A)   ((((A)&0x00ff)<<8) | ((A)>>8))
#define SWAP32(A)   ((((A)&0x000000ff)<<24) | (((A)&0x0000ff00)<<8) | (((A)&0x00ff0000)>>8) | (((A)&0xff000000)>>24))

#define PUT_BYTE_PROTOTYPE print_byte_uart
#define GET_BYTE_PROTOTYPE 

typedef enum {
    VAR_LONG=32,
    VAR_SHORT=16,
    VAR_CHAR=8
} VAR_TYPE;

static void PrintChar(char *fmt, char c);
static void PrintDec(char *fmt, int value);
static void PrintHex(char *fmt, int value);
static void PrintDouble(char* fmt, double value);
static void PrintString(char *fmt, char *cptr);
static int  Power(int num, int cnt);

void print_byte_uart(unsigned int ch) {
	if (ch == '\n') {
		USART_SendData(USART6, ('\r'));
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
		USART_SendData(USART6, ('\n'));
	}
	else {
		USART_SendData(USART6, (uint8_t)ch);
	}
	while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
}

char get_byte(void) {
	while( !((*(volatile unsigned long *) 0x40013800) & 0x20) ) ;
	return (char) *(volatile unsigned long *) 0x40013804;
}

int Power(int num, int cnt) {
	long retval=num;
	cnt--;

	while (cnt--){
		retval *= num;
	}
	return retval;
}

int DecToLong(char *s, long *retval) {
	
    long remainder;
	if (!s || !s[0]) return false;


	for (*retval=0; *s; s++){
		if (*s < '0' || *s > '9') return false;
		remainder = *s - '0';
		*retval = *retval * 10 + remainder;
	}

	return true;
}   

void printf(char *fmt, ...) {
	return;//////////////
    int i;
	va_list args;
	char *s=fmt;
	char format[10];

	int tmp;

	va_start(args, fmt);

	while (*s) {

		if (*s=='%') {
			s++;
			format[0] = '%';

			for (i=1; i<10;) {

				if (*s=='c' || *s=='d' || *s=='x' || *s=='s' || *s=='f' || *s=='%') {
					format[i++] = *s;
					format[i] = '\0';
					break;
				}
				else {
					format[i++] = *s++;
				}
			}

			switch (*s++) {

				case 'c' :
					PrintChar(format, va_arg(args, int));
					break;

				case 'd' :
					tmp = va_arg(args, int);
					if (tmp < 0) {
						printf("-");
						PrintDec(format, -tmp);
					}
					else {
						PrintDec(format, tmp);
					}
					break;

				case 'x' :
					PrintHex(format, va_arg(args, int));
					break;

				case 's' :
					PrintString(format, va_arg(args, char *));
					break;

				case 'f':
					PrintDouble(format, va_arg(args, double));
					break;

				case '%' :
					PrintChar("%c", '%');
					break;
			}
		}
		else {
			PrintChar("%c", *s);
			s++;
		}
	}
	va_end(args);
	return;
}

void PrintDouble(char* fmt, double d) {

	int decimalDigits = 4;
	int i = 1;
	int intPart, fractPart;

	for (;decimalDigits!=0; i*=10, decimalDigits--);

	intPart = (int)d;
	fractPart = (int)((d-(double)(int)d)*i);

	if (fractPart < 0) {
		fractPart *= -1;
	}

	printf("%d.%d", intPart, fractPart);
}

void PrintChar(char *fmt, char c) {
	PUT_BYTE_PROTOTYPE(c);
	return;
}

void PrintDec(char *fmt, int l) {

	int i, j;
	char c, *s=fmt, tol[10];
	bool flag0=false, flagl=false;
	long flagcnt=0;
	bool leading_zero=true;
	long divisor, result, remainder;

	for (i=0; (c=s[i]) != 0; i++) {

		if (c=='d') {
			break;
		}
		else if (c>='1' && c<='9') {

			for (j=0; s[i]>='0' && s[i]<='9'; j++){
				tol[j] = s[i++];
			}

			tol[j] = '\0';
			i--;
			DecToLong(tol, &flagcnt);
		}
		else if (c=='0') {
			flag0=true;
		}
		else if (c=='l') {
			flagl=true;
		}
		else {
			continue;
		}
	}


	if (flagcnt) {

		if (flagcnt>9) {
			flagcnt=9;
		}

		remainder = l%(Power(10, flagcnt));

		for (divisor=Power(10, flagcnt-1); divisor>0; divisor/=10) {

			result = remainder/divisor;
			remainder %= divisor;

			if (result!=0 || divisor==1) {
				leading_zero = false;
			}

			if (leading_zero==true) {
				if (flag0) {
					PUT_BYTE_PROTOTYPE('0');  
				} 
				else {
					PUT_BYTE_PROTOTYPE(' ');
				}               
			}
			else {
				PUT_BYTE_PROTOTYPE((char)(result)+'0');
			}
		}
	} 
	else {

		remainder = l;
		for (divisor=1000000000; divisor>0; divisor/=10) {

			result = remainder/divisor;
			remainder %= divisor;

			if (result!=0 || divisor==1) {
				leading_zero = false;
			}

			if (leading_zero==false) {
				PUT_BYTE_PROTOTYPE((char)(result)+'0');
			}
		}
	}
	return;
}

void PrintHex(char *fmt, int l) {

	int i, j;
	char c, *s=fmt, tol[10];
	bool flag0=false, flagl=false;        // flags.
	long flagcnt=0;
	bool leading_zero=true;
	char uHex, lHex;
	int cnt;

	for (i=0; (c=s[i]) != 0; i++) {

		if (c=='x') {
			break;
		}
		else if (c>='1' && c<='9') {

			for (j=0; s[i]>='0' && s[i]<='9'; j++){
				tol[j] = s[i++];
			}
			tol[j] = '\0';
			i--;
			DecToLong(tol, &flagcnt);
		}
		else if (c=='0') {
			flag0=true;
		}
		else if (c=='l') {
			flagl=true;
		}
		else {
			continue;
		}
	}

	s = (char *)(&l);    
	l = SWAP32(l);

	if (flagcnt) {

		if (flagcnt&0x01) {

			c = s[(8-(flagcnt+1))/2];
			lHex = ((c>>0)&0x0f);

			if (lHex!=0) {
				leading_zero=false;
			}

			if (lHex<10) {
				lHex+='0';
			}
			else {
				lHex+='A'-10;    
			}


			if (leading_zero) {
				if (flag0) {
				    PUT_BYTE_PROTOTYPE('0');
				}
				else {
					PUT_BYTE_PROTOTYPE(' ');
				}   
			}
			else {
				PUT_BYTE_PROTOTYPE(lHex);
			}

			flagcnt--;
		}

		for (cnt=0, i=(8-flagcnt)/2; i<4; i++) {
			c = s[i];

			// get upper 4 bits and lower 4 bits.
			uHex = ((c>>4)&0x0f);
			lHex = ((c>>0)&0x0f);

			if (uHex!=0) {
				leading_zero = false;
			}
			if (uHex<10) {
				uHex+='0';
			}
			else {
				uHex+='A'-10;
			}

			if (leading_zero) {
				if (flag0) {
					PUT_BYTE_PROTOTYPE('0');
				}
				else {
					PUT_BYTE_PROTOTYPE(' ');
				}
			}
			else {
			     PUT_BYTE_PROTOTYPE(uHex);
			}

			if (lHex!=0) {
				leading_zero = false;
			}
			if (lHex<10) {
				lHex+='0';
			}
			else {
				lHex+='A'-10;  
			}

			if (leading_zero){
				if (flag0) {
					PUT_BYTE_PROTOTYPE('0');
				}
				else {
					PUT_BYTE_PROTOTYPE(' ');
				}
			}
			else {
				PUT_BYTE_PROTOTYPE(lHex);
			}
		}
	}
	else {

		for (i=0; i<4; i++) {

			c = s[i];

			// get upper 4 bits and lower 4 bits.
			uHex = ((c>>4)&0x0f);
			lHex = ((c>>0)&0x0f);

			// upper 4 bits and lower 4 bits to '0'~'9', 'A'~'F'.
			if (uHex!=0) {
				leading_zero = false;
			}

			if (uHex<10) {
				uHex+='0';
			}
			else {
				uHex+='A'-10;
			}

			if (!leading_zero) {
				PUT_BYTE_PROTOTYPE(uHex);
			}

			if (lHex!=0 || i==3) {
				leading_zero = false;
			}

			if (lHex<10) {
				lHex+='0';
			}
			else {
				lHex+='A'-10;
			}

			if (!leading_zero) {
			    PUT_BYTE_PROTOTYPE(lHex);
			}
		}
	}
	return;
}

void PrintString(char *fmt, char *s) {
	if (!fmt || !s) {
		return;
	}
	while(*s) {
		PUT_BYTE_PROTOTYPE(*s++);
	}
	return;
}



