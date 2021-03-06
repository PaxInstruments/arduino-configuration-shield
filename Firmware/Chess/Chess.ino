/*

  Chess.pde
  
  Little Rook Chess

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  

*/


// This document is modified to work with the Pax Instruments LCD Test Board v3
#define LCD_A0               9
#define LCD_BACKLIGHT_PIN    6    // LCD backlight on pin
#define LCD_CS               7
#define LCD_RST              8
// MISO                     14
// SCK                      15
// MOSI                     16
#define SW_A_pin             4
#define SW_B_pin             5
#define SW_C_pin             A3
#define SW_D_pin             A2
#define SW_E_pin             A1
#define SW_F_pin             A0
#define SD_CS                10

#define LCD_CONTRAST     0x018*7  // Sets the LCD contrast

#include "U8glib.h"

U8GLIB_PI13264  u8g(LCD_CS, LCD_A0, LCD_RST); // Use HW-SPI

uint8_t uiKeyNext = SW_B_pin;
uint8_t uiKeyPrev = SW_A_pin;
uint8_t uiKeySelect = SW_C_pin;
uint8_t uiKeyBack = SW_D_pin;

uint8_t uiKeyCodeFirst = CHESS_KEY_NONE;
uint8_t uiKeyCodeSecond = CHESS_KEY_NONE;
uint8_t uiKeyCode = CHESS_KEY_NONE;


void uiSetup(void) {
  // configure input keys 
  
  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyPrev, HIGH);       // turn on pullup resistors
  pinMode(uiKeyNext, INPUT);           // set pin to input
  digitalWrite(uiKeyNext, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelect, INPUT);           // set pin to input
  digitalWrite(uiKeySelect, HIGH);       // turn on pullup resistors
  pinMode(uiKeyBack, INPUT);           // set pin to input
  digitalWrite(uiKeyBack, HIGH);       // turn on pullup resistors
}

void uiStep(void)
{
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = CHESS_KEY_PREV;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = CHESS_KEY_NEXT;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = CHESS_KEY_SELECT;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = CHESS_KEY_BACK;
  else 
    uiKeyCodeFirst = CHESS_KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = CHESS_KEY_NONE;
}


void setup() {
  // rotate screen, if required
  u8g.setRot180();
  
  uiSetup();
  chess_Init(u8g.getU8g(), 0);
  
  u8g.setContrast(LCD_CONTRAST);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);           
  analogWrite(LCD_BACKLIGHT_PIN, 0);
}

void loop() {  
  uint8_t keyCode = CHESS_KEY_NONE;
  
  u8g.firstPage();
  do {
    chess_Draw();
    uiStep();
    if ( uiKeyCode != CHESS_KEY_NONE )
      keyCode = uiKeyCode;
  } while( u8g.nextPage() );
  
  u8g_Delay(10);
  chess_Step(keyCode);
  uiStep();
  keyCode = uiKeyCode;
}


