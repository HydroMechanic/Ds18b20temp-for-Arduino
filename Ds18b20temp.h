#include <Arduino.h>
# ifndef Ds18b20temp_h
#define Ds18b20temp_h

class Ds18b20temp
{
private:
int mdsPin;
int mnumDs;
bool portRegB=false;
bool portRegD=false;
 byte   byteVal;
 bool bitNum;
byte romMem[8][8];
byte serNum[8];
byte serNumArray[10][8];//change first demension if more than 10 ds
bool bitVal[66];
bool scratchPadVal[73];
bool readVal[66][2];
int last0Pos=0;
int lastDcp=0;
int pos=1;
int index=0;
int addIndex=0;
int converDelay=0;//defined in tempRes function
byte dirportB1=B00000011;

void resetPulse();
void sendByte(byte cmd);
void writeBit(bool bitState);
bool readBit();
bool searchBit(int pos);
bool nextSearchAdd();
void printSerNum();

public:
 Ds18b20temp(int dsPin,int numDs);
float readTemp(int deviceNum);
void tempRes(int resVal,int devicePos);
void dsbegin();
};
#endif



