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
      dispBuf[bufStart%40] = Alphabet[idx*8];
      dispBuf[(bufStart+1)%40] = Alphabet[(idx*8)+1];
      dispBuf[(bufStart+2)%40] = Alphabet[(idx*8)+2];
      dispBuf[(bufStart+3)%40] = Alphabet[(idx*8)+3];
      dispBuf[(bufStart+4)%40] = Alphabet[(idx*8)+4];
      dispBuf[(bufStart+5)%40] = Alphabet[(idx*8)+5];
      dispBuf[(bufStart+6)%40] = Alphabet[(idx*8)+6];
      dispBuf[(bufStart+7)%40] = Alphabet[(idx*8)+7];
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
    lc.setColumn(floor(i/8)-1, i%8, dispBuf[(bufStart + i)%40]);
  };
  
  //put BT recieve code here
  //it should be *really* easy since it is just optional serial reading
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
  };

  //move start of circular buffer to the right
  //and wrap back to 0 if it extends past the end of the buffer
  if (bufStart == 0) {
    bufStart = 40;
  } else {
    bufStart -= 1;
  }
  //reset leftmost collumn
  dispBuf[bufStart%40] = 0;
  //delay(500);
}
