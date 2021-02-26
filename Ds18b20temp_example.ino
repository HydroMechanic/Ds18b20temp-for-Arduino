/*Note: Device Search address order does not match physical order
 * UNO dsPin can be D2-D13
 * If using Mega 2560 (Specifed dsPin>Actual Mega dspin)13>11;12>10;3>18;2>19
 * Whatever the choice the two pins cannot be used for GPIO
 */
#include <Ds18b20temp.h>
const int dsPin=8;
const int numDs=5;
const int resolution=10;
const int butPin0=4;
const int butPin1=5;
const int ledPin0=10;
const int ledPin1=11;
Ds18b20temp  myDs(dsPin,numDs);
void setup()
{
Serial.begin(115200);
while(Serial.available())Serial.read();
pinMode(butPin0,INPUT);
pinMode(ledPin0,OUTPUT);
pinMode(butPin1,INPUT);
pinMode(ledPin1,OUTPUT);
delay(10);
   myDs.dsbegin();
    for(int w=0;w<numDs;w++)
  {
    myDs.tempRes(resolution,w);
  }
  }
void loop()
{
  for(int w=0;w<numDs;w++)
  {
  float Ctemp=  myDs.readTemp(w);
 Serial.print("Device No:");Serial.print(w);
 Serial.print(" Temperature=");Serial.print(Ctemp,2);Serial.println(" C");
 readInputPins(butPin0,ledPin0);
  readInputPins(butPin1,ledPin1);
  }
}

  void readInputPins(int button,int led)
  {
    if(digitalRead(button))
    {
digitalWrite(led,HIGH);
    }
    else
    {
      digitalWrite(led,LOW);
    }
  
}
