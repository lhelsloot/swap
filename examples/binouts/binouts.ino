/*
 * binouts
 *
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 01/26/2012
 *
 * Device:
 * Binary output + PWM module
 *
 * Description:
 * Device that provides 8 controllable binary (ON/OFF) outputs and 4 PWM
 * outputs
 *
 * PANSTAMP_AVR and PANSTAMP_NRG
 * Binary outputs : pins 2, 4, 5, 6, 8, 9, 10 and 22
 * PWM outputs: pins 3, 18, 20 and 21
 *
 * PANSTAMP_NRG 2
 * Binary outputs : pins 3, 4, 5, 6, 7, 8, 9 and 10
 * PWM outputs: pins 3, 18, 20 and 21
 *
 * PANSTAMP_AVR 2 : pins 5 (A7) and 6 (A6) not supported as binary outputs.
 * Better use a NRG 2 module instead.
 *
 * This sketch can be used to control loads (ON/OFF) and even run some type
 * of progressive control via PWM (dim lights, control motors, blinds, etc)
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * binouts.xml (Binary/PWM output module)
 */
 
#include "regtable.h"
#include "swap.h"
#ifdef PANSTAMP_NRG
#include "cc430aes.h"
#endif

#ifdef PANSTAMP_NRG
#ifdef __NRG_VERSION_2_0__
// Binary output pins (Arduino digital pins)
uint8_t binaryPin[] = {15, 14, 13, 12, 11, 10, 9, 8};
// PWM output pins (Arduino digital pins)
uint8_t pwmPin[] = {18, 19, 1, 2};
#else
// Binary output pins (Arduino digital pins)
uint8_t binaryPin[] = {16, 18, 15, 14, 12, 11, 10, 1};
// PWM output pins (Arduino digital pins)
uint8_t pwmPin[] = {17, 2, 3, 5};
#endif
#else // PANSTAMP_AVR
// Binary output pins (Arduino digital pins)
uint8_t binaryPin[] = {8, 14, 15, 16, 17, 18, 19, 7};
// PWM output pins (Arduino digital pins)
uint8_t pwmPin[] = {9, 6, 5, 3};
#endif

/**
 * swapStatusReceived
 *
 * Function automatically called by the panStamp API whenever a SWAP status
 * packet is received
 *
 * 'status'    SWAP status packet received
 */
 void swapStatusReceived(SWPACKET *status)
 {
   // Place here you code if you want the outputs to change
   // according to the value of any external register/endpoint
 }

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  int i;

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Configure output pins
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    pinMode(binaryPin[i], OUTPUT);
  for(i=0 ; i<sizeof(pwmPin) ; i++)
    pinMode(pwmPin[i], OUTPUT);
 
  // Init SWAP stack
  swap.init();
  
  // AES-128 encryption (for NRG only)
  //byte aesKey[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};  
  //swap.setAesPassword(aesKey);
  
  // Smart Encryption (software encryption)
  //byte password[] = {1,2,3,4,5,6,7,8,9,10,11,12};
  //panstamp.setSmartPassword(password);

  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();
  // Enter Rx ON state
  swap.enterSystemState(SYSTATE_RXON);
  // Transmit initial binary states
  swap.getRegister(REGI_BINOUTPUTS)->getData();
  // Transmit initial PWM values
  swap.getRegister(REGI_PWMOUTPUTS)->getData();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(4900);
}

