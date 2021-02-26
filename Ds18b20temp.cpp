/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Arduino.h>
#include <Ds18b20temp.h>

Ds18b20temp::Ds18b20temp(int dsPin,int numDs)
{
mdsPin=dsPin;
mnumDs=numDs;
}

void Ds18b20temp::dsbegin()
{
if(mdsPin<2)
{
Serial.println("Pin D0 and D1 used for Serial communication,DO NOT USE!");
return;
}
  if(mdsPin>=8&&mdsPin<=13)
{
  bitSet(dirportB1,mdsPin-8);
 portRegB=true;
}

else  if(mdsPin>=2&&mdsPin<=7)
{
  bitSet(dirportB1,mdsPin);
 portRegD=true;
}


while(nextSearchAdd())
{
index++;
}

Serial.print("Search completed: ");Serial.print("Num Devices=");
Serial.println(index+1);
if((index+1)!=mnumDs)
{
Serial.println("WARNING:SEARCH DS NUMBER AND USER INPUT DS DOES NOT MATCH!");
 }
}

void Ds18b20temp::tempRes(int resVal,int devicePos)
{
  byte resByte;
  byte tlByte=B11111111;
  byte thByte=B01111111;
  switch(resVal)
  {
    case 9:
    resByte=B00000000;
    converDelay=100;
    break;

    case 10:
    resByte=B00100000;
    converDelay=200;
    break;

    case 11:
    resByte=B01000000;
    converDelay=400;
    break;

    case 12:
    resByte=B01100000;
    converDelay=700;
    break;
    
  }
  byte configArray[]={thByte,tlByte,resByte};
  resetPulse();
    sendByte(0x55);
  for(int n=0;n<8;n++)
  {
    for(int w=0;w<8;w++)
    {
      bool b=bitRead(serNumArray[devicePos][n],w);
       writeBit(b);
     }
   
  }
  sendByte(0x4e);
  for (int p=0;p<3;p++)
  {
    sendByte(configArray[p]);
  }
  resetPulse();
      sendByte(0x55);
  for(int n=0;n<8;n++)
  {
    for(int w=0;w<8;w++)
    {
      bool b=bitRead(serNumArray[devicePos][n],w);
       writeBit(b);
     }
  }
    // sendByte(0x48);
     //delay(10);
     resetPulse();
}

void Ds18b20temp::resetPulse()
{
if(portRegB==true)
{
DDRB|=dirportB1;
PORTB|=dirportB1;
PORTB&=~dirportB1;
delayMicroseconds(600);
PORTB|=dirportB1;
delayMicroseconds(30);
DDRB&=~dirportB1;
while(PINB==0)
{
 //Serial.println("pulse");
delayMicroseconds(60);
}
delayMicroseconds(400);
}
else if(portRegD==true)
{
DDRD|=dirportB1;
PORTD|=dirportB1;
PORTD&=~dirportB1;
delayMicroseconds(600);
PORTD|=dirportB1;
delayMicroseconds(30);
DDRD&=~dirportB1;
while(PIND==0)
{
 //Serial.println("pulse");//uncoment for confirmation of reset
delayMicroseconds(60);
}
delayMicroseconds(400);
}
}

void Ds18b20temp::sendByte(byte cmd)
{
   for(int j=0;j<8;j++)
  {
   boolean byteBit=bitRead(cmd,j);

writeBit(byteBit);
  }
}



void Ds18b20temp:: writeBit(bool bitState)
{
   if(portRegB==true)
{
     DDRB|=dirportB1;
PORTB|=dirportB1;
PORTB&=~dirportB1;
delayMicroseconds(1);
  if(bitState==1)
  {
    
     PORTB|=dirportB1;
     delayMicroseconds(50);
}
 else
  {
      PORTB&=~dirportB1;
      delayMicroseconds(50);
    PORTB|=dirportB1;
  }
delayMicroseconds(1);
}
else if(portRegD==true)
{
     DDRD|=dirportB1;
PORTD|=dirportB1;
PORTD&=~dirportB1;
delayMicroseconds(1);
  if(bitState==1)
  {
    
     PORTD|=dirportB1;
     delayMicroseconds(50);
}
 else
  {
      PORTD&=~dirportB1;
      delayMicroseconds(50);
    PORTD|=dirportB1;
  }
delayMicroseconds(1);
}
}

bool  Ds18b20temp::readBit()
{
 if(portRegB==true)
{
    byteVal=0;
  bitNum=0;
DDRB|=dirportB1;
PORTB|=dirportB1;
PORTB&=~dirportB1;
delayMicroseconds(1);
PORTB|=dirportB1;
DDRB&=~dirportB1;
delayMicroseconds(7);
  byteVal=(PINB);
     
  
DDRB|=dirportB1;
 PORTB|=dirportB1;
    delayMicroseconds(25);
     bitNum=bitRead(byteVal,mdsPin-8);

    return bitNum;
}
 if(portRegD==true)
{
  byteVal=0;
  bitNum=0;
DDRD|=dirportB1;
PORTD|=dirportB1;
PORTD&=~dirportB1;
delayMicroseconds(1);
PORTD|=dirportB1;
DDRD&=~dirportB1;
delayMicroseconds(7);
  byteVal=(PIND);
     
  
DDRD|=dirportB1;
 PORTD|=dirportB1;
    delayMicroseconds(25);
     bitNum=bitRead(byteVal,mdsPin);

    return bitNum;
}
}


bool Ds18b20temp::searchBit(int pos)
{
 
 readVal[pos][0]=readBit();

 readVal[pos][1]=readBit();


if(readVal[pos][0]==0&&readVal[pos][1]==1)
{
 bitVal[pos]=0;
 writeBit(bitVal[pos]);
  return bitVal[pos];
}
if(readVal[pos][0]==1&&readVal[pos][1]==0)
{
   bitVal[pos]=1;
   writeBit(bitVal[pos]);
  return bitVal[pos];
}
if(readVal[pos][0]==0&&readVal[pos][1]==0)
{

 if(pos==lastDcp)
 {
 
  writeBit(1);
  readVal[pos][0]=1;
   readVal[pos][1]=0;
   bitVal[pos]=1;
   return bitVal[pos];
 }
else if(pos>lastDcp)
{

  writeBit(0);
  last0Pos=pos;
  readVal[pos][0]=0;
   readVal[pos][1]=1;
   bitVal[pos]=0;
   return bitVal[pos];
}

 else if(lastDcp>pos)
  {
  
    writeBit(bitVal[pos]);
    
    if(bitVal[pos]==0)
    {
      
      last0Pos=pos;
    
      return bitVal[pos];
    }
     else
    {
      
     
      return bitVal[pos];
    }
  }

}

}


bool Ds18b20temp::nextSearchAdd()
{
    pos=1;
  last0Pos=0;
  resetPulse();
    sendByte(0xf0);
  for(int q=1;q<65;q++)
{
 bitVal[q]=searchBit(q);
 if(readVal[pos][0]==1&&readVal[pos][1]==1)
{
 break;
}
}
lastDcp=last0Pos;
printSerNum();
if(lastDcp==0)
{
  return false;
}
else
{
  return true;
}
}


void Ds18b20temp:: printSerNum()
{

 //  read8Byte();
 for(int r=0;r<8;r++)
{
  for(int q=8;q>0;q--)
  {
    romMem[r][8-q]=bitVal[r*8+q];
   bitWrite(serNum[r],q-1,romMem[r][8-q]);
  }
 serNumArray[addIndex][r]=serNum[r];
 Serial.print(serNumArray[addIndex][r],HEX);Serial.print(':');
 
}
 addIndex++;
 Serial.println("");
Serial.println("--------------------------------------------");

 
}
float Ds18b20temp::readTemp(int deviceNum)
{
  byte loByte=B0;
  byte hiByte=B0;
  byte sign=B0;
 byte signMask=B11111000;
  int temp=0;
float cTemp=0;
  
  resetPulse();
  sendByte(0x55);
  for(int n=0;n<8;n++)
  {
    for(int w=0;w<8;w++)
    {
      bool b=bitRead(serNumArray[deviceNum][n],w);
       writeBit(b);
     }
   
  }
  sendByte(0x44);
    delay(converDelay);
    resetPulse();
     sendByte(0x55);
   for(int n=0;n<8;n++)
  {
    for(int w=0;w<8;w++)
    {
      bool b=bitRead(serNumArray[deviceNum][n],w);
       writeBit(b);
     }
  }
  sendByte(0xbe);

   for ( int k=0;k<72;k++)
 {
  scratchPadVal[k]=readBit();
  
 }
 resetPulse();

 for(int r=0;r<8;r++)
 {
  bitWrite(loByte,r,scratchPadVal[r]);
  bitWrite(hiByte,r,scratchPadVal[r+8]);
 }

 sign=signMask&hiByte;
if(sign==B11111000){hiByte|B10000000;}

 temp=(hiByte<<8)+loByte;
cTemp=temp/16.;
return cTemp;

}