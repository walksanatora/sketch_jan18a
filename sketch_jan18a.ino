#include <LedControl.h>
#include "font.h"
//din - 11
//cs - 10
//clk 13
// 4 adresses
LedControl lc=LedControl(11,13,10,4);

//8 rows * 5 displays
//(0-7 is a "virtual" display that is scrolled from)
byte dispBuf[40] = {};
//we use the above buffer as a circular buffer, this is the "start" of the buffer
byte bufStart = 0;

//alarm that goes every 8 itterations (singal to write a new char onto the virtual screen)
byte alarm = 0;

//upcoming letters, TODO: code a font
byte chr = 0;
char text[64] = "Hello World!";

void setup() {
  // put your setup code here, to run once:
  for (byte i=0; i<4; i++) {
    lc.shutdown(i,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(i,8);
    /* and clear the display */
    lc.clearDisplay(i);
  }
  
}

void loop() {
  //old code, copied directly from static example
  //byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  //byte r[5]={B00111110,B00010000,B00100000,B00100000,B00010000};
  //byte d[5]={B00011100,B00100010,B00100010,B00010010,B11111110};
  //byte u[5]={B00111100,B00000010,B00000010,B00000100,B00111110};
  //lc.setColumn(3,0,a[0]);
  //lc.setColumn(3,1,a[1]);
  //lc.setColumn(3,2,a[2]);
  //lc.setColumn(3,3,a[3]);
  //lc.setColumn(3,4,a[4]);
  //lc.setColumn(2,0,r[0]);
  //lc.setColumn(2,1,r[1]);
  //lc.setColumn(2,2,r[2]);
  //lc.setColumn(2,3,r[3]);
  //lc.setColumn(2,4,r[4]);
  //lc.setColumn(1,0,d[0]);
  //lc.setColumn(1,1,d[1]);
  //lc.setColumn(1,2,d[2]);
  //lc.setColumn(1,3,d[3]);
  //lc.setColumn(1,4,d[4]);
  //lc.setColumn(0,0,u[0]);
  //lc.setColumn(0,1,u[1]);
  //lc.setColumn(0,2,u[2]);
  //lc.setColumn(0,3,u[3]);
  //lc.setColumn(0,4,u[4]);

  if (alarm == 0) {
    if (chr > 64) {
      //do nothing, we have reached the end of the text buffer
    } else {
      byte idx = AlphabetLUT[chr];
      dispBuf[bufStart%40] = Alphabet[idx];
      dispBuf[(bufStart+1)%40] = Alphabet[idx+1];
      dispBuf[(bufStart+2)%40] = Alphabet[idx+2];
      dispBuf[(bufStart+3)%40] = Alphabet[idx+3];
      dispBuf[(bufStart+4)%40] = Alphabet[idx+4];
      dispBuf[(bufStart+5)%40] = Alphabet[idx+5];
      dispBuf[(bufStart+6)%40] = Alphabet[idx+6];
      dispBuf[(bufStart+7)%40] = Alphabet[idx+7];
      alarm = 8;
      chr +=1;
    };
  } else {
    alarm-=1;
  };

  // put your main code here, to run repeatedly:
  //re-render screens
  Serial.print("bufStart ");
  Serial.println(bufStart);
  for (byte i = 8; 1<=40;i++) { //skip to the 8th collumn, AKA the left-most physical collumn
    Serial.println(i);
    //put rendering code here
    //math.floor(i/8) the screen id
    //i%8 the collumn
    lc.setColumn(floor(i/8), i%8,dispBuf[(bufStart + i)%40]);
  };
  
  //put BT recieve code here
  //it should be *really* easy since it is just optional serial reading
  if (Serial.available()>0) {
    Serial.readBytes(text,64);
    chr=0;
  };

  //move start of circular buffer to the right
  //and wrap back to 0 if it extends past the end of the buffer
  bufStart = (bufStart + 1)%40;
  //reset leftmost collumn
  dispBuf[bufStart] = 0;
  delay(1000);
}
