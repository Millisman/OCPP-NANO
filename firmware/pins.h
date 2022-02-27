#pragma once

#include "mcu/pin.h"

#define PIN_LED_RED     MAKEPIN(A, 0, OUT) // PA0 (OUT), pull down 10K, driving LED LOAD
#define PIN_LED_GREEN   MAKEPIN(A, 1, OUT) // PA1 (OUT), pull down 10K, driving LED LOAD
#define PIN_LED_BLUE    MAKEPIN(A, 2, OUT) // PA2 (OUT), pull down 10K, driving LED LOAD

#define PIN_CS_ADE      MAKEPIN(B, 0, OUT) // PB0 (OUT) ADE77_CSN, need pull up, active - LOW
#define PIN_CS_NFC      MAKEPIN(B, 1, OUT) // PB1 (OUT) RC522_CSN, need pull up, active - LOW
#define PIN_GFI         MAKEPIN(B, 2, IN)  // PB2 (IN) INT 2 GFI
#define PIN_RF24_CE     MAKEPIN(B, 3, OUT) // PB3 (OUT) RF24_CE,  NRF24L01 chip ENABLE, out, active HIGH, free (pulldown in NRF24)
#define PIN_RF24_CSN    MAKEPIN(B, 4, OUT) // PB4 (OUT) RF24_CSN, NRF24L01 chip nSELECT, out, active LOW, free (pullup in NRF24)
#define PIN_MOSI        MAKEPIN(B, 5, OUT) // PB5 (OUT) MOSI
#define PIN_MISO        MAKEPIN(B, 6, IN)  // PB6 (IN)  MISO
#define PIN_SCK         MAKEPIN(B, 7, OUT) // PB7 (OUT) SCK

#define PIN_LCD_RS          MAKEPIN(C, 0, OUT)  // PC0 (OUT) PC0_LCD_RS LCD RS
#define PIN_LCD_EN          MAKEPIN(C, 1, OUT)  // PC1 (OUT) PC1_LCD_E  LCD E
#define PIN_LCD_D4          MAKEPIN(C, 2, OUT)  // PC2 (OUT) PC2_LCD_D4 LCD D4
#define PIN_LCD_D5          MAKEPIN(C, 3, OUT)  // PC3 (OUT) PC3_LCD_D5 LCD D5
#define PIN_LCD_D6          MAKEPIN(C, 4, OUT)  // PC4 (OUT) PC4_LCD_D6 LCD D6
#define PIN_LCD_D7          MAKEPIN(C, 5, OUT)  // PC5 (OUT) PC5_LCD_D7 LCD D7
#define PIN_MENNEKES_LOCK   MAKEPIN(C, 6, OUT)  // PC6 (OUT) PC6_2LOCK   LOCK,   pull-down 10K, active- UP
#define PIN_MENNEKES_UNLOCK MAKEPIN(C, 7, OUT)  // PC7 (OUT) PC7_2UNLOCK UNLOCK, pull-down 10K, active- UP
// UART0 PD0
// UART0 PD1
// UART1 PD2
// UART1 PD3
#define PIN_PILO            MAKEPIN(D, 4, OUT) // PD4 PILO
#define PIN_GSM_POWER       MAKEPIN(D, 5, OUT) // PD5 Enable Modem
#define PIN_GFITEST         MAKEPIN(D, 6, OUT) // PD6 pin is supposed to be wrapped around the GFI CT 5+ times
#define PIN_AC_RELAY        MAKEPIN(D, 7, OUT) // PD7 (OUT) PD7_RELAY Relay/Contactor driver, pull-down 10K, active- UP
