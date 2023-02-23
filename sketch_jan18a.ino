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

//alarm that goes every 8 itterations (singal to write a new char onto the virtual screen)
byte alarm = 0;

//upcoming letters, TODO: code a font
byte chr = 0;
byte len = 13;
char text[64] = "HELLO WORLD!";

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (byte i=0; i<4; i++) {
    lc.shutdown(i,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(i,8);
    /* and clear the display */
    lc.clearDisplay(i);
  }
  Serial.println("Booted!");
}

void loop() {

  if (alarm == 0) {
    if (chr > len) {
      //loop to the start
      chr = 0;
      alarm = 0;
    } else {
      // some more debug for adding to disp buf
      //char tob[40];
      //sprintf(tob,"bufst: %d..%d",bufStart,(bufStart+7)%40);
      //Serial.println(tob);
      byte idx = AlphabetLUT[text[chr]];
      dispBuf[0] = Alphabet[idx*8];
      dispBuf[1] = Alphabet[(idx*8)+1];
      dispBuf[2] = Alphabet[(idx*8)+2];
      dispBuf[3] = Alphabet[(idx*8)+3];
      dispBuf[4] = Alphabet[(idx*8)+4];
      dispBuf[5] = Alphabet[(idx*8)+5];
      dispBuf[6] = Alphabet[(idx*8)+6];
      dispBuf[7] = Alphabet[(idx*8)+7];
      // debug when pushing a new char to the disp buffer
      //sprintf(tob,"Pushed Char: %c (%x)\nidx: %d",text[chr],text[chr],idx);
      //Serial.println(tob);
      //for (byte t = 0; t < 8; t++)
      //{
      //  Serial.print("Pushed line: ");
      //  byte val = Alphabet[(idx*8)+t];
      //  for (int j = 0; j < 8; j++)
      //  {
      //      bool b = val & 0x80;
      //      Serial.print(b);
      //      val = val << 1;
      //  };Serial.println();
      //}
      alarm = 8;
      chr +=1;
    };
  } else {
    alarm-=1;
  };

  //debug dispBuf debug logger
  //for (byte t = 0; t < 41; t++)
  //{
  //  Serial.print("buffer: ");
  //  byte val = dispBuf[(bufStart+t)%40];
  //  for (int j = 0; j < 8; j++)
  //  {
  //      bool b = val & 0x80;
  //      Serial.print(b);
  //      val = val << 1;
  //  };Serial.println();
  //}

  // put your main code here, to run repeatedly:
  //Serial.println("Render");
  for (byte i = 8; i<=40;i++) { //skip to the 8th collumn, AKA the left-most physical collumn
    //put rendering code here
    //math.floor(i/8) the screen id
    //i%8 the collumn
    //Serial.println("rendering col: %d",i);
    //Serial.print("Render col: ");
    //byte val = dispBuf[(bufStart + i)%40];
    //for (int j = 0; j < 8; j++)
    //{
    //    bool b = val & 0x80;
    //    Serial.print(b);
    //    val = val << 1;
    //};Serial.println();
    lc.setColumn(floor(i/8)-1, i%8, dispBuf[i]);
  };
  
  // simple serial reading
  if (Serial.available()>0) {
    for (byte i=0; i<4; i++) {
      /* clear the display */
      lc.clearDisplay(i);
    }
    for (byte i=0; i<40; i++) {
      /* clear the buffer */
      dispBuf[i] = 0;
    }
    for (byte i=0; i<len; i++) {
      /* clear the text */
      text[i] = 0;
    }
    
    len = Serial.readBytes(text,64);

    chr=0;
    alarm=0;
  };

  byte tempBuf[40];
  memcpy(tempBuf,dispBuf,39*sizeof(byte));
  memcpy((dispBuf + sizeof(byte)),tempBuf,39*sizeof(byte));
  //delay(500);
}
