// AVR High-voltage Serial Programmer
// Originally created by Paul Willoughby 03/20/2010
// http://www.rickety.us/2010/03/arduino-avr-high-voltage-serial-programmer/
// Inspired by Jeff Keyzer http://mightyohm.com
// Serial Programming routines from ATtiny25/45/85 datasheet

#include "main.h"

// Desired fuse configuration
#define HFUSE 0xDF // Defaults for ATtiny25/45/85
#define LFUSE 0x62

#define VCC       PD7     // Connect to VCC Pin 8
#define RST       PD6     // Output to level shifter transistor base, for 12V !RESET from transistor to AVR Pin 1, 12V -- 5K -- Transistor Collector
#define CLKOUT    PD5     // Connect to Serial Clock Input (SCI) Pin 2
#define DATAIN    PD4     // Connect to Serial Data Output (SDO) Pin 7
#define INSTOUT   PD3     // Connect to Serial Instruction Input (SII) Pin 6
#define DATAOUT   PD2     // Connect to Serial Data Input (SDI) Pin 5

#define CLKOUT_DELAY 50
#define CLK_PULSE SETBIT(PORTD, CLKOUT); _delay_us(CLKOUT_DELAY); CLEARBIT(PORTD, CLKOUT); _delay_us(CLKOUT_DELAY);

uint8_t shiftOut2(uint8_t val, uint8_t val1);
void readFuses();

void main(){
  // Set up control lines for HV parallel programming
  SETBIT(DDRD, VCC);
  SETBIT(DDRD, RST);
  SETBIT(DDRD, DATAOUT);
  SETBIT(DDRD, INSTOUT);
  SETBIT(DDRD, CLKOUT);
  SETBIT(DDRD, DATAIN);
  // Initialize output pins as needed
  SETBIT(PORTD, RST);
  // start serial port at 9600 bps:
  UARTInit();
  UARTSendString("\n\rHVPP Hello\n\r"); // send an initial string
  // Initialize pins to enter programming mode
  CLEARBIT(PORTD, DATAOUT);
  CLEARBIT(PORTD, INSTOUT);
  CLEARBIT(PORTD, DATAIN);
  SETBIT(PORTD, RST);
  // Enter High-voltage Serial programming mode
  SETBIT(PORTD, VCC);
  _delay_us(20);
  CLEARBIT(PORTD, RST);
  _delay_us(10);
  CLEARBIT(DDRD, DATAIN);
  _delay_us(300);
  //Programming mode
  readFuses();
  //Write hfuse
  UARTSendString("\n\rWriting hfuse");
  shiftOut2(0x40, 0x4C);
  shiftOut2(HFUSE, 0x2C); 
  shiftOut2(0x00, 0x74);
  shiftOut2(0x00, 0x7C);
  //Write lfuse
  UARTSendString("\n\rWriting lfuse");
  shiftOut2(0x40, 0x4C);
  shiftOut2(LFUSE, 0x2C);
  shiftOut2(0x00, 0x64);
  shiftOut2(0x00, 0x6C);
  readFuses();
  UARTSendString("\n\rExiting programming Mode");
  CLEARBIT(PORTD, CLKOUT);
  CLEARBIT(PORTD, VCC);
  SETBIT(PORTD, RST);
}


uint8_t shiftOut2(uint8_t val, uint8_t val1){
  uint8_t i;
  uint8_t inBits = 0;
  //Wait until DATAIN goes high
  while (!(PIND & (1 << DATAIN)))
    ;
  //Start bit
  CLEARBIT(PORTD, DATAOUT);
  CLEARBIT(PORTD, INSTOUT);
  CLK_PULSE;

  for (i = 0; i < 8; i++){
    // if (val & (1 << (7 - i))){
    //   SETBIT(PORTD, DATAOUT);
    // }else{
    //   CLEARBIT(PORTD, DATAOUT);
    // }
    val & (1 << (7 - i)) ? SETBIT(PORTD, DATAOUT) : CLEARBIT(PORTD, DATAOUT);
    val1 & (1 << (7 - i)) ? SETBIT(PORTD, INSTOUT) : CLEARBIT(PORTD, INSTOUT);

    // if (val1 & (1 << (7 - i))){
    //   SETBIT(PORTD, INSTOUT);
    // }else{
    //   CLEARBIT(PORTD, INSTOUT);
    // }

    inBits <<= 1;
    inBits |= (PIND & (1 << DATAIN)) ? 1 : 0;
    CLK_PULSE;
  }
  //End bits
  CLEARBIT(PORTD, DATAOUT);
  CLEARBIT(PORTD, INSTOUT);
  CLK_PULSE;
  CLK_PULSE;
  return inBits;
}

void readFuses(){
  uint8_t inData = 0; // incoming serial byte AVR
  //Read lfuse
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x68);
  inData = shiftOut2(0x00, 0x6C);
  UARTSendString("\n\rlfuse reads as ");
  UARTSend_uint8(inData);
  //Read hfuse
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x7A);
  inData = shiftOut2(0x00, 0x7E);
  UARTSendString("\n\rhfuse reads as ");
  UARTSend_uint8(inData);
  //Read efuse
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x6A);
  inData = shiftOut2(0x00, 0x6E);
  UARTSendString("\n\refuse reads as ");
  UARTSend_uint8(inData);
}