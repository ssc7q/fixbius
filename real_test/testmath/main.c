
#include "system8051s_hw_platform.h"

#define uint32 unsigned long int
#define int32 signed long int

#define APB3_BASE_ADRESS 0xF000
#define GPIO0_addr APB3_BASE_ADRESS + COREGPIO_0


//__xdata at GPIO1_addr unsigned char configREG_1;
__xdata at GPIO0_addr unsigned char configREG_0;

__xdata at GPIO0_addr + 0x90 unsigned char datain;
__data at 0x9A unsigned char XWB1;
__data at 0x9B unsigned char XWB2;
__data at 0x9C unsigned char XWB3;

__xdata at GPIO0_addr + 0xA0 unsigned char dataout;
__data at 0x9D unsigned char XRB1;
__data at 0x9E unsigned char XRB2;
__data at 0x9F unsigned char XRB3;

__xdata volatile float af;
__xdata volatile float bf;
__xdata volatile signed long int ai;
__xdata volatile signed long int bi;
__xdata volatile signed long int ax;
__xdata volatile signed long int bx;

__xdata volatile int32 i = 0;

#define TOFIX(d,q) ((int)  ( (d)*(float)(1<<(q))))
#define TOFLT(a,q) ((float)( (a)/(float)(1<<(q))))

#define q 27


int32 MULT(int32 a, int32 b);
void debugGpioPing();

void main()
{
	for(i; i<12; i++){
		*(&configREG_0 + i*0x04)=0x03;
		//*(&configREG_1 + i*0x04)=0x03;
	}
	af = -0.00879;
	bf =  1.25121;
	ax = TOFIX(af,1);
	bx = TOFIX(bf,1);
	ai = ax;
	bi = bx;
	for(;;){
		dataout = 0x00;

		debugGpioPing();
		ai += bi;
		debugGpioPing();
		ax += bx;
		debugGpioPing();
		af += bf;
		debugGpioPing();
		ai *= bi;
		debugGpioPing();
		ax = MULT(ax,bx);
		debugGpioPing();
		af = af * bf;
		debugGpioPing();
		ai /= bi;
		debugGpioPing();
		ax /= ax;
		debugGpioPing();
		dataout = 0x02;
		af = -0.00879;
	}
}

int32 MULT(int32 a, int32 b)
{
	int32 temp, result;
	signed char sign = 1;

	if(a<0)
	{
		sign *= -1;
		a = -a;
	}
	if(b<0)
	{
		sign *= -1;
		b = -b;
	}
	result = (((a & 0x0000FFFF) * (b & 0x0000FFFF)) + 67108864)>>q;
	result = result + ((((a>>16) * (b & 0x0000FFFF)) + 16)>>(q-16));
	result = result + ((((b>>16) * (a & 0x0000FFFF)) + 16)>>(q-16));
	temp = (a>>16) * (b>>16);
	result = result + (temp<<(32-q));
	return (result * sign);
}

void debugGpioPing()
{
	static idata unsigned int buffer;
	buffer = dataout&0x02;
	dataout = 0;
	dataout = buffer;
}
