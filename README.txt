Ds18b20temp library for Arduino

Reading ds18b20 tempertures, choose 9,10,11,12 resolution,higher resolution 750ms for conversion.
 
Note: Device Search address order does not match physical order
Error message displayed if search results donot match number of ds18b20(numDs) connected .

UNO: dsPin can be D2-D13;


 If using Mega 2560 (Specifed dsPin>Actual Mega dspin)13>D11;12>D10;3>D18;2>D19
  what ever the choice the two pins cannot be used for GPIO

User functions:

Ds18b20temp(dsPin,numDs);constructor,numDs number of ds18b20 connected daisychain.

dsbegin;intialize ds18b20 8 byte hex code for each ds18b20 will be displayed in Serial Monitor.First address is device 0,next device 1 and so on.

tempRes(resolution,device number) ;resolution(9,10,11,12);device number(0 to the number of ds connected.

readTemp(device number);return temperture  of specifed device number