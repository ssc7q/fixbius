#include <stdio.h>
#include <conio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>

#define uint32 unsigned long int
#define int32 signed int

#define TOFIX(d, q) ((int) ( (d)*(float)(1<<(q)) ))
#define TOFLT(a, q) ( (float) (a) / (float) (1<<(q)) )

#define cosinusDelta	0.999999
#define sinusDelta		0.000872
#define q 27

#define fltquaternionMUL(a,b,c) a[0]=b[0]*c[0] - b[1]*c[1] - b[2]*c[2] - b[3]*c[3];\
								a[1]=b[0]*c[1] + b[1]*c[0] + b[2]*c[3] - b[3]*c[2];\
								a[2]=b[0]*c[2] + b[2]*c[0] + b[3]*c[1] - b[1]*c[3];\
								a[3]=b[0]*c[3] + b[3]*c[0] + b[1]*c[2] - b[2]*c[1];

#define fixquaternionMUL(a,b,c) a[0] = (MULT(b[0],c[0])) - (MULT(b[1],c[1])) - (MULT(b[2],c[2])) - (MULT(b[3],c[3]));\
								a[1] = (MULT(b[0],c[1])) + (MULT(b[1],c[0])) + (MULT(b[2],c[3])) - (MULT(b[3],c[2]));\
								a[2] = (MULT(b[0],c[2])) + (MULT(b[2],c[0])) + (MULT(b[3],c[1])) - (MULT(b[1],c[3]));\
								a[3] = (MULT(b[0],c[3])) + (MULT(b[3],c[0])) + (MULT(b[1],c[2])) - (MULT(b[2],c[1]));
//global vars
float fltQuaternion[4] = {1.0, 0, 0, 0},			fltQuaternionBuffer[4], fltQuaternionDelta[4], difQuaternions[4];
int32 fixQuaternion[4] = {TOFIX(1.0, q), 0, 0, 0},	fixQuaternionBuffer[4], fixQuaternionDelta[4];

int32 MULT(int32 a, int32 b)
{
	int32 temp,result;
	signed char sign=1;
	
	if(a < 0)
	{
		sign*= -1;
		a = -a;
	}
	if(b < 0)
	{
		sign*= -1;
		b = -b;
	}

	result = (((a & 0x0000FFFF) * (b & 0x0000FFFF)) + 67108864)>>q;
	result = result + ((((a>>16) * (b & 0x0000FFFF)) + 16)>>11);
	result = result + ((((b>>16) * (a & 0x0000FFFF)) + 16)>>11);
	temp = (a>>16) * (b>>16);
    result = result + (temp<<5);
    return (result * sign);
}

void biusQuaternionCalc(float kSpeed, float rSpeed, float tSpeed)
{
	char i = 0;
	fltQuaternionDelta[0] = 1.5 - 0.5*(fltQuaternion[0]*fltQuaternion[0]
								    + fltQuaternion[1]*fltQuaternion[1]
                                    + fltQuaternion[2]*fltQuaternion[2]
                                    + fltQuaternion[3]*fltQuaternion[3]
                                    + kSpeed*kSpeed
                                    + rSpeed*rSpeed
                                    + tSpeed*tSpeed);
	fltQuaternionDelta[1] = kSpeed * fltQuaternionDelta[0];
	fltQuaternionDelta[2] = rSpeed * fltQuaternionDelta[0];
	fltQuaternionDelta[3] = tSpeed * fltQuaternionDelta[0];

	for (i=0; i<4; i++){
				fltQuaternionBuffer[i] = fltQuaternion[i];
			//	printf("fltQuaternion[%i]: %f\n", i, fltQuaternion[i]);
			}

	fltquaternionMUL(fltQuaternion,fltQuaternionBuffer,fltQuaternionDelta);

	fltQuaternionBuffer[1] = fltQuaternion[1];

	fltQuaternion[1]=fltQuaternion[1]*cosinusDelta - fltQuaternion[2]*sinusDelta;
	fltQuaternion[2]=fltQuaternion[2]*cosinusDelta + fltQuaternionBuffer[1]*sinusDelta;
}

void fixedbiusQuaternionCalc(float kSpeed, float rSpeed, float tSpeed)
{
	char i = 0;
	int32 fixSpeedkren, fixSpeedrysk, fixSpeedtang;
	float dQuaternionDelta[4]; 

	fixSpeedkren = TOFIX(kSpeed,q);
	fixSpeedrysk = TOFIX(rSpeed,q);
	fixSpeedtang = TOFIX(tSpeed,q);

	fixQuaternionDelta[0] = TOFIX(1.5,q) - MULT(TOFIX(0.5,q),(MULT(fixQuaternion[0],fixQuaternion[0])
															+ MULT(fixQuaternion[1],fixQuaternion[1])
															+ MULT(fixQuaternion[2],fixQuaternion[2])
															+ MULT(fixQuaternion[3],fixQuaternion[3])
															+ MULT(fixSpeedkren,fixSpeedkren)
															+ MULT(fixSpeedrysk,fixSpeedrysk)
															+ MULT(fixSpeedtang,fixSpeedtang)));
	fixQuaternionDelta[1] = MULT(fixSpeedkren,fixQuaternionDelta[0]);
	fixQuaternionDelta[2] = MULT(fixSpeedrysk,fixQuaternionDelta[0]);
	fixQuaternionDelta[3] = MULT(fixSpeedtang,fixQuaternionDelta[0]);

	for (i=0; i<4; i++){
		fixQuaternionBuffer[i] = fixQuaternion[i];
		dQuaternionDelta[i] = TOFLT(fixQuaternionDelta[i],q);
		//printf("fixQuaternion[%i]: %f\n", i, TOFLT(fixQuaternion[i],q));
	}

	fixquaternionMUL(fixQuaternion,fixQuaternionBuffer,fixQuaternionDelta);

	fixQuaternionBuffer[1] = fixQuaternion[1];

	fixQuaternion[1] = MULT(fixQuaternion[1],TOFIX(cosinusDelta,q)) - MULT(fixQuaternion[2],TOFIX(sinusDelta,q));
	fixQuaternion[2] = MULT(fixQuaternion[2],TOFIX(cosinusDelta,q)) - MULT(fixQuaternionBuffer[1],TOFIX(sinusDelta,q));
}

int main(){
	char const* const dirdata	 = "C:/Users/vmed/fixbius/logs/datain.txt";
	char const* const dirfixout	 = "C:/Users/vmed/fixbius/logs/fix.txt";

	
	char line[10];

	FILE* filedat	= fopen(dirdata, "r");
	FILE* filefix	= fopen(dirfixout, "w");	 


	float SpeedKren, SpeedRysk, SpeedTang;
	double ftemp;
	int c = 0, i;
	while (fgets(line, sizeof(line), filedat)){
		c++;
		ftemp = atof(line);

		switch (c){
		case(6):
			biusQuaternionCalc(SpeedKren, SpeedRysk, SpeedTang);
			fixedbiusQuaternionCalc(SpeedKren, SpeedRysk, SpeedTang);
			for(i = 0; i< 4; i++)
			{
				difQuaternions[i]=TOFLT(fixQuaternion[i],q)-fltQuaternion[i];
				fprintf(filefix,"%f ",difQuaternions[i]);
			}
			fprintf(filefix, "\n");

			c=0;
			break;
		case(5):
			SpeedTang = ftemp;
			break;
		case(4):
			SpeedRysk = ftemp;
			break;
		case(3):
			SpeedKren = ftemp;
			break;
		}
		//Sleep(100);
	}
	fclose(filedat);
	fclose(filefix);


	getch();
	return 0;
}