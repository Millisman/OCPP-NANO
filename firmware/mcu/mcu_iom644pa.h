#pragma once

#define _SFR_ASM_COMPAT 0
#include <inttypes.h>
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _MMIO_WORD(mem_addr) (*(volatile uint16_t *)(mem_addr))
#define _MMIO_DWORD(mem_addr) (*(volatile uint32_t *)(mem_addr))


#ifndef __SFR_OFFSET
#  if __AVR_ARCH__ >= 100
#    define __SFR_OFFSET 0x00
#  else
#    define __SFR_OFFSET 0x20
#  endif
#endif
#define _BV(bit) (1 << (bit))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
#define loop_until_bit_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))

/* Registers and associated bit numbers. */

#define PINA (0x00)
#define PINA0 0
#define PINA1 1
#define PINA2 2
#define PINA3 3
#define PINA4 4
#define PINA5 5
#define PINA6 6
#define PINA7 7

#define DDRA (0x01)
#define DDA0 0
#define DDA1 1
#define DDA2 2
#define DDA3 3
#define DDA4 4
#define DDA5 5
#define DDA6 6
#define DDA7 7

#define PORTA (0x02)
#define PORTA0 0
#define PORTA1 1
#define PORTA2 2
#define PORTA3 3
#define PORTA4 4
#define PORTA5 5
#define PORTA6 6
#define PORTA7 7

#define PINB (0x03)
#define PINB0 0
#define PINB1 1
#define PINB2 2
#define PINB3 3
#define PINB4 4
#define PINB5 5
#define PINB6 6
#define PINB7 7

#define DDRB (0x04)
#define DDB0 0
#define DDB1 1
#define DDB2 2
#define DDB3 3
#define DDB4 4
#define DDB5 5
#define DDB6 6
#define DDB7 7

#define PORTB (0x05)
#define PORTB0 0
#define PORTB1 1
#define PORTB2 2
#define PORTB3 3
#define PORTB4 4
#define PORTB5 5
#define PORTB6 6
#define PORTB7 7

#define PINC (0x06)
#define PINC0 0
#define PINC1 1
#define PINC2 2
#define PINC3 3
#define PINC4 4
#define PINC5 5
#define PINC6 6
#define PINC7 7

#define DDRC (0x07)
#define DDC0 0
#define DDC1 1
#define DDC2 2
#define DDC3 3
#define DDC4 4
#define DDC5 5
#define DDC6 6
#define DDC7 7

#define PORTC (0x08)
#define PORTC0 0
#define PORTC1 1
#define PORTC2 2
#define PORTC3 3
#define PORTC4 4
#define PORTC5 5
#define PORTC6 6
#define PORTC7 7

static uint8_t PIND;

// #define PIND (0x09)
#define PIND0 0
#define PIND1 1
#define PIND2 2
#define PIND3 3
#define PIND4 4
#define PIND5 5
#define PIND6 6
#define PIND7 7

#define DDRD (0x0A)
#define DDD0 0
#define DDD1 1
#define DDD2 2
#define DDD3 3
#define DDD4 4
#define DDD5 5
#define DDD6 6
#define DDD7 7

static uint8_t PORTD;


// #define PORTD (0x0B)
#define PORTD0 0
#define PORTD1 1
#define PORTD2 2
#define PORTD3 3
#define PORTD4 4
#define PORTD5 5
#define PORTD6 6
#define PORTD7 7

#define TIFR0 (0x15)
#define TOV0 0
#define OCF0A 1
#define OCF0B 2

#define TIFR1 (0x16)
#define TOV1 0
#define OCF1A 1
#define OCF1B 2
#define ICF1 5

#define TIFR2 (0x17)
#define TOV2 0
#define OCF2A 1
#define OCF2B 2

#define PCIFR (0x1B)
#define PCIF0 0
#define PCIF1 1
#define PCIF2 2
#define PCIF3 3

#define EIFR (0x1C)
#define INTF0 0
#define INTF1 1
#define INTF2 2

#define EIMSK (0x1D)
#define INT0 0
#define INT1 1
#define INT2 2

#define GPIOR0 (0x1E)
#define GPIOR00 0
#define GPIOR01 1
#define GPIOR02 2
#define GPIOR03 3
#define GPIOR04 4
#define GPIOR05 5
#define GPIOR06 6
#define GPIOR07 7

#define EECR (0x1F)
#define EERE 0
#define EEPE 1
#define EEMPE 2
#define EERIE 3
#define EEPM0 4
#define EEPM1 5

#define EEDR (0x20)
#define EEDR0 0
#define EEDR1 1
#define EEDR2 2
#define EEDR3 3
#define EEDR4 4
#define EEDR5 5
#define EEDR6 6
#define EEDR7 7

#define EEAR _SFR_IO16(0x21)

#define EEARL (0x21)
#define EEAR0 0
#define EEAR1 1
#define EEAR2 2
#define EEAR3 3
#define EEAR4 4
#define EEAR5 5
#define EEAR6 6
#define EEAR7 7

#define EEARH (0x22)
#define EEAR8 0
#define EEAR9 1
#define EEAR10 2
#define EEAR11 3

#define GTCCR (0x23)
#define PSRSYNC 0
#define PSRASY 1
#define TSM 7

#define TCCR0A (0x24)
#define WGM00 0
#define WGM01 1
#define COM0B0 4
#define COM0B1 5
#define COM0A0 6
#define COM0A1 7

#define TCCR0B (0x25)
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM02 3
#define FOC0B 6
#define FOC0A 7

#define TCNT0 (0x26)
#define TCNT0_0 0
#define TCNT0_1 1
#define TCNT0_2 2
#define TCNT0_3 3
#define TCNT0_4 4
#define TCNT0_5 5
#define TCNT0_6 6
#define TCNT0_7 7

#define OCR0A (0x27)
#define OCR0A_0 0
#define OCR0A_1 1
#define OCR0A_2 2
#define OCR0A_3 3
#define OCR0A_4 4
#define OCR0A_5 5
#define OCR0A_6 6
#define OCR0A_7 7

#define OCR0B (0x28)
#define OCR0B_0 0
#define OCR0B_1 1
#define OCR0B_2 2
#define OCR0B_3 3
#define OCR0B_4 4
#define OCR0B_5 5
#define OCR0B_6 6
#define OCR0B_7 7

#define GPIOR1 (0x2A)
#define GPIOR10 0
#define GPIOR11 1
#define GPIOR12 2
#define GPIOR13 3
#define GPIOR14 4
#define GPIOR15 5
#define GPIOR16 6
#define GPIOR17 7

#define GPIOR2 (0x2B)
#define GPIOR20 0
#define GPIOR21 1
#define GPIOR22 2
#define GPIOR23 3
#define GPIOR24 4
#define GPIOR25 5
#define GPIOR26 6
#define GPIOR27 7

#define SPCR (0x2C)
#define SPR0 0
#define SPR1 1
#define CPHA 2
#define CPOL 3
#define MSTR 4
#define DORD 5
#define SPE 6
#define SPIE 7

#define SPSR (0x2D)
#define SPI2X 0
#define WCOL 6
#define SPIF 7

#define SPDR (0x2E)
#define SPDR0 0
#define SPDR1 1
#define SPDR2 2
#define SPDR3 3
#define SPDR4 4
#define SPDR5 5
#define SPDR6 6
#define SPDR7 7

#define ACSR (0x30)
#define ACIS0 0
#define ACIS1 1
#define ACIC 2
#define ACIE 3
#define ACI 4
#define ACO 5
#define ACBG 6
#define ACD 7

#define OCDR (0x31)
#define OCDR0 0
#define OCDR1 1
#define OCDR2 2
#define OCDR3 3
#define OCDR4 4
#define OCDR5 5
#define OCDR6 6
#define OCDR7 7

#define SMCR (0x33)
#define SE 0
#define SM0 1
#define SM1 2
#define SM2 3

#define MCUSR (0x34)
#define PORF 0
#define EXTRF 1
#define BORF 2
#define WDRF 3
#define JTRF 4

#define MCUCR (0x35)
#define IVCE 0
#define IVSEL 1
#define PUD 4
#define BODSE 5
#define BODS 6
#define JTD 7

#define SPMCSR (0x37)
#define SPMEN 0
#define PGERS 1
#define PGWRT 2
#define BLBSET 3
#define RWWSRE 4
#define SIGRD 5
#define RWWSB 6
#define SPMIE 7

#define WDTCSR (0x60)
#define WDP0 0
#define WDP1 1
#define WDP2 2
#define WDE 3
#define WDCE 4
#define WDP3 5
#define WDIE 6
#define WDIF 7

#define CLKPR (0x61)
#define CLKPS0 0
#define CLKPS1 1
#define CLKPS2 2
#define CLKPS3 3
#define CLKPCE 7

#define PRR0 (0x64)
#define PRADC 0
#define PRUSART0 1
#define PRSPI 2
#define PRTIM1 3
#define PRUSART1 4
#define PRTIM0 5
#define PRTIM2 6
#define PRTWI 7

#define __AVR_HAVE_PRR0	((1<<PRADC)|(1<<PRSPI)|(1<<PRTIM1)|(1<<PRUSART0)|(1<<PRUSART1)|(1<<PRTIM0)|(1<<PRTIM2)|(1<<PRTWI))
#define __AVR_HAVE_PRR0_PRADC
#define __AVR_HAVE_PRR0_PRSPI
#define __AVR_HAVE_PRR0_PRTIM1
#define __AVR_HAVE_PRR0_PRUSART0
#define __AVR_HAVE_PRR0_PRUSART1
#define __AVR_HAVE_PRR0_PRTIM0
#define __AVR_HAVE_PRR0_PRTIM2
#define __AVR_HAVE_PRR0_PRTWI

#define OSCCAL (0x66)
#define CAL0 0
#define CAL1 1
#define CAL2 2
#define CAL3 3
#define CAL4 4
#define CAL5 5
#define CAL6 6
#define CAL7 7

#define PCICR (0x68)
#define PCIE0 0
#define PCIE1 1
#define PCIE2 2
#define PCIE3 3

#define EICRA (0x69)
#define ISC00 0
#define ISC01 1
#define ISC10 2
#define ISC11 3
#define ISC20 4
#define ISC21 5

#define PCMSK0 (0x6B)
#define PCINT0 0
#define PCINT1 1
#define PCINT2 2
#define PCINT3 3
#define PCINT4 4
#define PCINT5 5
#define PCINT6 6
#define PCINT7 7

#define PCMSK1 (0x6C)
#define PCINT8 0
#define PCINT9 1
#define PCINT10 2
#define PCINT11 3
#define PCINT12 4
#define PCINT13 5
#define PCINT14 6
#define PCINT15 7

#define PCMSK2 (0x6D)
#define PCINT16 0
#define PCINT17 1
#define PCINT18 2
#define PCINT19 3
#define PCINT20 4
#define PCINT21 5
#define PCINT22 6
#define PCINT23 7

#define TIMSK0 (0x6E)
#define TOIE0 0
#define OCIE0A 1
#define OCIE0B 2

#define TIMSK1 (0x6F)
#define TOIE1 0
#define OCIE1A 1
#define OCIE1B 2
#define ICIE1 5

#define TIMSK2 (0x70)
#define TOIE2 0
#define OCIE2A 1
#define OCIE2B 2

#define PCMSK3 (0x73)
#define PCINT24 0
#define PCINT25 1
#define PCINT26 2
#define PCINT27 3
#define PCINT28 4
#define PCINT29 5
#define PCINT30 6
#define PCINT31 7

#ifndef __ASSEMBLER__
#define ADC (0x78)
#endif
#define ADCW (0x78)

#define ADCL (0x78)
#define ADCL0 0
#define ADCL1 1
#define ADCL2 2
#define ADCL3 3
#define ADCL4 4
#define ADCL5 5
#define ADCL6 6
#define ADCL7 7

#define ADCH (0x79)
#define ADCH0 0
#define ADCH1 1
#define ADCH2 2
#define ADCH3 3
#define ADCH4 4
#define ADCH5 5
#define ADCH6 6
#define ADCH7 7

#define ADCSRA (0x7A)
#define ADPS0 0
#define ADPS1 1
#define ADPS2 2
#define ADIE 3
#define ADIF 4
#define ADATE 5
#define ADSC 6
#define ADEN 7

#define ADCSRB (0x7B)
#define ADTS0 0
#define ADTS1 1
#define ADTS2 2
#define ACME 6

#define ADMUX (0x7C)
#define MUX0 0
#define MUX1 1
#define MUX2 2
#define MUX3 3
#define MUX4 4
#define ADLAR 5
#define REFS0 6
#define REFS1 7

#define DIDR0 (0x7E)
#define ADC0D 0
#define ADC1D 1
#define ADC2D 2
#define ADC3D 3
#define ADC4D 4
#define ADC5D 5
#define ADC6D 6
#define ADC7D 7

#define DIDR1 (0x7F)
#define AIN0D 0
#define AIN1D 1

#define TCCR1A (0x80)
#define WGM10 0
#define WGM11 1
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7

#define TCCR1B (0x81)
#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4
#define ICES1 6
#define ICNC1 7

#define TCCR1C (0x82)
#define FOC1B 6
#define FOC1A 7

#define TCNT1 (0x84)

#define TCNT1L (0x84)
#define TCNT1L0 0
#define TCNT1L1 1
#define TCNT1L2 2
#define TCNT1L3 3
#define TCNT1L4 4
#define TCNT1L5 5
#define TCNT1L6 6
#define TCNT1L7 7

#define TCNT1H (0x85)
#define TCNT1H0 0
#define TCNT1H1 1
#define TCNT1H2 2
#define TCNT1H3 3
#define TCNT1H4 4
#define TCNT1H5 5
#define TCNT1H6 6
#define TCNT1H7 7

#define ICR1 (0x86)

#define ICR1L (0x86)
#define ICR1L0 0
#define ICR1L1 1
#define ICR1L2 2
#define ICR1L3 3
#define ICR1L4 4
#define ICR1L5 5
#define ICR1L6 6
#define ICR1L7 7

#define ICR1H (0x87)
#define ICR1H0 0
#define ICR1H1 1
#define ICR1H2 2
#define ICR1H3 3
#define ICR1H4 4
#define ICR1H5 5
#define ICR1H6 6
#define ICR1H7 7

#define OCR1A (0x88)

#define OCR1AL (0x88)
#define OCR1AL0 0
#define OCR1AL1 1
#define OCR1AL2 2
#define OCR1AL3 3
#define OCR1AL4 4
#define OCR1AL5 5
#define OCR1AL6 6
#define OCR1AL7 7

#define OCR1AH (0x89)
#define OCR1AH0 0
#define OCR1AH1 1
#define OCR1AH2 2
#define OCR1AH3 3
#define OCR1AH4 4
#define OCR1AH5 5
#define OCR1AH6 6
#define OCR1AH7 7

#define OCR1B (0x8A)

#define OCR1BL (0x8A)
#define OCR1BL0 0
#define OCR1BL1 1
#define OCR1BL2 2
#define OCR1BL3 3
#define OCR1BL4 4
#define OCR1BL5 5
#define OCR1BL6 6
#define OCR1BL7 7

#define OCR1BH (0x8B)
#define OCR1BH0 0
#define OCR1BH1 1
#define OCR1BH2 2
#define OCR1BH3 3
#define OCR1BH4 4
#define OCR1BH5 5
#define OCR1BH6 6
#define OCR1BH7 7

#define TCCR2A (0xB0)
#define WGM20 0
#define WGM21 1
#define COM2B0 4
#define COM2B1 5
#define COM2A0 6
#define COM2A1 7

#define TCCR2B (0xB1)
#define CS20 0
#define CS21 1
#define CS22 2
#define WGM22 3
#define FOC2B 6
#define FOC2A 7

#define TCNT2 (0xB2)
#define TCNT2_0 0
#define TCNT2_1 1
#define TCNT2_2 2
#define TCNT2_3 3
#define TCNT2_4 4
#define TCNT2_5 5
#define TCNT2_6 6
#define TCNT2_7 7

#define OCR2A (0xB3)
#define OCR2A_0 0
#define OCR2A_1 1
#define OCR2A_2 2
#define OCR2A_3 3
#define OCR2A_4 4
#define OCR2A_5 5
#define OCR2A_6 6
#define OCR2A_7 7

#define OCR2B (0xB4)
#define OCR2B_0 0
#define OCR2B_1 1
#define OCR2B_2 2
#define OCR2B_3 3
#define OCR2B_4 4
#define OCR2B_5 5
#define OCR2B_6 6
#define OCR2B_7 7

#define ASSR (0xB6)
#define TCR2BUB 0
#define TCR2AUB 1
#define OCR2BUB 2
#define OCR2AUB 3
#define TCN2UB 4
#define AS2 5
#define EXCLK 6

#define TWBR (0xB8)
#define TWBR0 0
#define TWBR1 1
#define TWBR2 2
#define TWBR3 3
#define TWBR4 4
#define TWBR5 5
#define TWBR6 6
#define TWBR7 7

#define TWSR (0xB9)
#define TWPS0 0
#define TWPS1 1
#define TWS3 3
#define TWS4 4
#define TWS5 5
#define TWS6 6
#define TWS7 7

#define TWAR (0xBA)
#define TWGCE 0
#define TWA0 1
#define TWA1 2
#define TWA2 3
#define TWA3 4
#define TWA4 5
#define TWA5 6
#define TWA6 7

#define TWDR (0xBB)
#define TWD0 0
#define TWD1 1
#define TWD2 2
#define TWD3 3
#define TWD4 4
#define TWD5 5
#define TWD6 6
#define TWD7 7

#define TWCR (0xBC)
#define TWIE 0
#define TWEN 2
#define TWWC 3
#define TWSTO 4
#define TWSTA 5
#define TWEA 6
#define TWINT 7

#define TWAMR (0xBD)
#define TWAM0 1
#define TWAM1 2
#define TWAM2 3
#define TWAM3 4
#define TWAM4 5
#define TWAM5 6
#define TWAM6 7

#define UCSR0A (0xC0)
#define MPCM0 0
#define U2X0 1
#define UPE0 2
#define DOR0 3
#define FE0 4
#define UDRE0 5
#define TXC0 6
#define RXC0 7

#define UCSR0B (0xC1)
#define TXB80 0
#define RXB80 1
#define UCSZ02 2
#define TXEN0 3
#define RXEN0 4
#define UDRIE0 5
#define TXCIE0 6
#define RXCIE0 7

#define UCSR0C (0xC2)
#define UCPOL0 0
#define UCSZ00 1
#define UCSZ01 2
#define USBS0 3
#define UPM00 4
#define UPM01 5
#define UMSEL00 6
#define UMSEL01 7

#define UBRR0 (0xC4)

#define UBRR0L (0xC4)
#define _UBRR0 0
#define _UBRR1 1
#define UBRR2 2
#define UBRR3 3
#define UBRR4 4
#define UBRR5 5
#define UBRR6 6
#define UBRR7 7

#define UBRR0H (0xC5)
#define UBRR8 0
#define UBRR9 1
#define UBRR10 2
#define UBRR11 3

#define UDR0 (0xC6)
#define UDR0_0 0
#define UDR0_1 1
#define UDR0_2 2
#define UDR0_3 3
#define UDR0_4 4
#define UDR0_5 5
#define UDR0_6 6
#define UDR0_7 7

#define UCSR1A (0xC8)
#define MPCM1 0
#define U2X1 1
#define UPE1 2
#define DOR1 3
#define FE1 4
#define UDRE1 5
#define TXC1 6
#define RXC1 7

#define UCSR1B (0xC9)
#define TXB81 0
#define RXB81 1
#define UCSZ12 2
#define TXEN1 3
#define RXEN1 4
#define UDRIE1 5
#define TXCIE1 6
#define RXCIE1 7

#define UCSR1C (0xCA)
#define UCPOL1 0
#define UCSZ10 1
#define UCSZ11 2
#define USBS1 3
#define UPM10 4
#define UPM11 5
#define UMSEL10 6
#define UMSEL11 7

#define UBRR1 (0xCC)

#define UBRR1L (0xCC)
#define UBRR_0 0
#define UBRR_1 1
#define UBRR_2 2
#define UBRR_3 3
#define UBRR_4 4
#define UBRR_5 5
#define UBRR_6 6
#define UBRR_7 7

#define UBRR1H (0xCD)
#define UBRR_8 0
#define UBRR_9 1
#define UBRR_10 2
#define UBRR_11 3

#define UDR1 (0xCE)
#define UDR1_0 0
#define UDR1_1 1
#define UDR1_2 2
#define UDR1_3 3
#define UDR1_4 4
#define UDR1_5 5
#define UDR1_6 6
#define UDR1_7 7


/* Interrupt vectors */
/* Vector 0 is the reset vector */
#define INT0_vect_num  1
#define INT0_vect      (1)  /* External Interrupt Request 0 */
#define INT1_vect_num  2
#define INT1_vect      (2)  /* External Interrupt Request 1 */
#define INT2_vect_num  3
#define INT2_vect      (3)  /* External Interrupt Request 2 */
#define PCINT0_vect_num  4
#define PCINT0_vect      (4)  /* Pin Change Interrupt Request 0 */
#define PCINT1_vect_num  5
#define PCINT1_vect      (5)  /* Pin Change Interrupt Request 1 */
#define PCINT2_vect_num  6
#define PCINT2_vect      (6)  /* Pin Change Interrupt Request 2 */
#define PCINT3_vect_num  7
#define PCINT3_vect      (7)  /* Pin Change Interrupt Request 3 */
#define WDT_vect_num  8
#define WDT_vect      (8)  /* Watchdog Time-out Interrupt */
#define TIMER2_COMPA_vect_num  9
#define TIMER2_COMPA_vect      (9)  /* Timer/Counter2 Compare Match A */
#define TIMER2_COMPB_vect_num  10
#define TIMER2_COMPB_vect      (10)  /* Timer/Counter2 Compare Match B */
#define TIMER2_OVF_vect_num  11
#define TIMER2_OVF_vect      (11)  /* Timer/Counter2 Overflow */
#define TIMER1_CAPT_vect_num  12
#define TIMER1_CAPT_vect      (12)  /* Timer/Counter1 Capture Event */
#define TIMER1_COMPA_vect_num  13
#define TIMER1_COMPA_vect      (13)  /* Timer/Counter1 Compare Match A */
#define TIMER1_COMPB_vect_num  14
#define TIMER1_COMPB_vect      (14)  /* Timer/Counter1 Compare Match B */
#define TIMER1_OVF_vect_num  15
#define TIMER1_OVF_vect      (15)  /* Timer/Counter1 Overflow */
#define TIMER0_COMPA_vect_num  16
#define TIMER0_COMPA_vect      (16)  /* Timer/Counter0 Compare Match A */
#define TIMER0_COMPB_vect_num  17
#define TIMER0_COMPB_vect      (17)  /* Timer/Counter0 Compare Match B */
#define TIMER0_OVF_vect_num  18
#define TIMER0_OVF_vect      (18)  /* Timer/Counter0 Overflow */
#define SPI_STC_vect_num  19
#define SPI_STC_vect      (19)  /* SPI Serial Transfer Complete */
#define USART0_RX_vect_num  20
#define USART0_RX_vect      (20)  /* USART0, Rx Complete */
#define USART0_UDRE_vect_num  21
#define USART0_UDRE_vect      (21)  /* USART0 Data register Empty */
#define USART0_TX_vect_num  22
#define USART0_TX_vect      (22)  /* USART0, Tx Complete */
#define ANALOG_COMP_vect_num  23
#define ANALOG_COMP_vect      (23)  /* Analog Comparator */
#define ADC_vect_num  24
#define ADC_vect      (24)  /* ADC Conversion Complete */
#define EE_READY_vect_num  25
#define EE_READY_vect      (25)  /* EEPROM Ready */
#define TWI_vect_num  26
#define TWI_vect      (26)  /* 2-wire Serial Interface */
#define SPM_READY_vect_num  27
#define SPM_READY_vect      (27)  /* Store Program Memory Read */
#define USART1_RX_vect_num  28
#define USART1_RX_vect      (28)  /* USART1 RX complete */
#define USART1_UDRE_vect_num  29
#define USART1_UDRE_vect      (29)  /* USART1 Data Register Empty */
#define USART1_TX_vect_num  30
#define USART1_TX_vect      (30)  /* USART1 TX complete */

#define _SIZE 4 /* Size of individual vector. */
#define S_SIZE (31 * _SIZE)


/* Constants */
#define SPM_PAGESIZE (256)
#define RAMSTART     (0x100)
#define RAMSIZE      (4096)
#define RAMEND       (RAMSTART + RAMSIZE - 1)
#define XRAMSTART    (0x0)
#define XRAMSIZE     (0)
#define XRAMEND      (RAMEND)
#define E2END        (0x7FF)
#define E2PAGESIZE   (8)
#define FLASHEND     (0xFFFF)


/* Fuses */
#define FUSE_MEMORY_SIZE 3

/* Low Fuse Byte */
#define FUSE_CKSEL0  (unsigned char)~_BV(0)  /* Select Clock Source */
#define FUSE_CKSEL1  (unsigned char)~_BV(1)  /* Select Clock Source */
#define FUSE_CKSEL2  (unsigned char)~_BV(2)  /* Select Clock Source */
#define FUSE_CKSEL3  (unsigned char)~_BV(3)  /* Select Clock Source */
#define FUSE_SUT0  (unsigned char)~_BV(4)  /* Select start-up time */
#define FUSE_SUT1  (unsigned char)~_BV(5)  /* Select start-up time */
#define FUSE_CKOUT  (unsigned char)~_BV(6)  /* Clock output */
#define FUSE_CKDIV8  (unsigned char)~_BV(7)  /* Divide clock by 8 */
#define LFUSE_DEFAULT (FUSE_CKDIV8 & FUSE_SUT1 & FUSE_SUT0 & FUSE_CKSEL3 & FUSE_CKSEL2 & FUSE_CKSEL0)

/* High Fuse Byte */
#define FUSE_BOOTRST  (unsigned char)~_BV(0)  /* Select Reset Vector */
#define FUSE_BOOTSZ0  (unsigned char)~_BV(1)  /* Select Boot Size */
#define FUSE_BOOTSZ1  (unsigned char)~_BV(2)  /* Select Boot Size */
#define FUSE_EESAVE  (unsigned char)~_BV(3)  /* EEPROM memory is preserved through chip erase */
#define FUSE_WDTON  (unsigned char)~_BV(4)  /* Watchdog timer always on */
#define FUSE_SPIEN  (unsigned char)~_BV(5)  /* Enable Serial programming and Data Downloading */
#define FUSE_JTAGEN  (unsigned char)~_BV(6)  /* Enable JTAG */
#define FUSE_OCDEN  (unsigned char)~_BV(7)  /* Enable OCD */
#define HFUSE_DEFAULT (FUSE_JTAGEN & FUSE_SPIEN & FUSE_BOOTSZ1 & FUSE_BOOTSZ0)

/* Extended Fuse Byte */
#define FUSE_BODLEVEL0  (unsigned char)~_BV(0)  /* Brown-out Detector trigger level */
#define FUSE_BODLEVEL1  (unsigned char)~_BV(1)  /* Brown-out Detector trigger level */
#define FUSE_BODLEVEL2  (unsigned char)~_BV(2)  /* Brown-out Detector trigger level */
#define EFUSE_DEFAULT (0xFF)


/* Lock Bits */
#define __LOCK_BITS_EXIST
#define __BOOT_LOCK_BITS_0_EXIST
#define __BOOT_LOCK_BITS_1_EXIST


/* Signature */
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x96
#define SIGNATURE_2 0x0A


/* Device Pin Definitions */
#define MOSI_DDR   DDRB
#define MOSI_PORT  PORTB
#define MOSI_PIN   PINB
#define MOSI_BIT   5

#define PCINT13_DDR   DDRB
#define PCINT13_PORT  PORTB
#define PCINT13_PIN   PINB
#define PCINT13_BIT   5

#define MISO_DDR   DDRB
#define MISO_PORT  PORTB
#define MISO_PIN   PINB
#define MISO_BIT   6

#define PCINT14_DDR   DDRB
#define PCINT14_PORT  PORTB
#define PCINT14_PIN   PINB
#define PCINT14_BIT   6

#define SCK_DDR   DDRB
#define SCK_PORT  PORTB
#define SCK_PIN   PINB
#define SCK_BIT   7

#define PCINT15_DDR   DDRB
#define PCINT15_PORT  PORTB
#define PCINT15_PIN   PINB
#define PCINT15_BIT   7

#define RXD_DDR   DDRD
#define RXD_PORT  PORTD
#define RXD_PIN   PIND
#define RXD_BIT   0

#define PCINT24_DDR   DDRD
#define PCINT24_PORT  PORTD
#define PCINT24_PIN   PIND
#define PCINT24_BIT   0

#define TXD_DDR   DDRD
#define TXD_PORT  PORTD
#define TXD_PIN   PIND
#define TXD_BIT   1

#define PCINT25_DDR   DDRD
#define PCINT25_PORT  PORTD
#define PCINT25_PIN   PIND
#define PCINT25_BIT   1

#define INT0_DDR   DDRD
#define INT0_PORT  PORTD
#define INT0_PIN   PIND
#define INT0_BIT   2

#define RDX1_DDR   DDRD
#define RDX1_PORT  PORTD
#define RDX1_PIN   PIND
#define RDX1_BIT   2

#define PCINT26_DDR   DDRD
#define PCINT26_PORT  PORTD
#define PCINT26_PIN   PIND
#define PCINT26_BIT   2

#define INT1_DDR   DDRD
#define INT1_PORT  PORTD
#define INT1_PIN   PIND
#define INT1_BIT   3

#define TXD1_DDR   DDRD
#define TXD1_PORT  PORTD
#define TXD1_PIN   PIND
#define TXD1_BIT   3

#define PCINT27_DDR   DDRD
#define PCINT27_PORT  PORTD
#define PCINT27_PIN   PIND
#define PCINT27_BIT   3

#define OC1B_DDR   DDRD
#define OC1B_PORT  PORTD
#define OC1B_PIN   PIND
#define OC1B_BIT   4

#define XCK1_DDR   DDRD
#define XCK1_PORT  PORTD
#define XCK1_PIN   PIND
#define XCK1_BIT   4

#define PCINT28_DDR   DDRD
#define PCINT28_PORT  PORTD
#define PCINT28_PIN   PIND
#define PCINT28_BIT   4

#define OC1A_DDR   DDRD
#define OC1A_PORT  PORTD
#define OC1A_PIN   PIND
#define OC1A_BIT   5

#define PCINT29_DDR   DDRD
#define PCINT29_PORT  PORTD
#define PCINT29_PIN   PIND
#define PCINT29_BIT   5

#define ICP_DDR   DDRD
#define ICP_PORT  PORTD
#define ICP_PIN   PIND
#define ICP_BIT   6

#define OC2B_DDR   DDRD
#define OC2B_PORT  PORTD
#define OC2B_PIN   PIND
#define OC2B_BIT   6

#define PCINT30_DDR   DDRD
#define PCINT30_PORT  PORTD
#define PCINT30_PIN   PIND
#define PCINT30_BIT   6

#define OC2A_DDR   DDRD
#define OC2A_PORT  PORTD
#define OC2A_PIN   PIND
#define OC2A_BIT   7

#define PCINT31_DDR   DDRD
#define PCINT31_PORT  PORTD
#define PCINT31_PIN   PIND
#define PCINT31_BIT   7

#define SCL_DDR   DDRC
#define SCL_PORT  PORTC
#define SCL_PIN   PINC
#define SCL_BIT   0

#define PCINT16_DDR   DDRC
#define PCINT16_PORT  PORTC
#define PCINT16_PIN   PINC
#define PCINT16_BIT   0

#define SDA_DDR   DDRC
#define SDA_PORT  PORTC
#define SDA_PIN   PINC
#define SDA_BIT   1

#define PCINT17_DDR   DDRC
#define PCINT17_PORT  PORTC
#define PCINT17_PIN   PINC
#define PCINT17_BIT   1

#define PCINT18_DDR   DDRC
#define PCINT18_PORT  PORTC
#define PCINT18_PIN   PINC
#define PCINT18_BIT   2

#define PCINT19_DDR   DDRC
#define PCINT19_PORT  PORTC
#define PCINT19_PIN   PINC
#define PCINT19_BIT   3

#define PCINT20_DDR   DDRC
#define PCINT20_PORT  PORTC
#define PCINT20_PIN   PINC
#define PCINT20_BIT   4

#define PCINT21_DDR   DDRC
#define PCINT21_PORT  PORTC
#define PCINT21_PIN   PINC
#define PCINT21_BIT   5

#define PCINT22_DDR   DDRC
#define PCINT22_PORT  PORTC
#define PCINT22_PIN   PINC
#define PCINT22_BIT   6

#define PCINT23_DDR   DDRC
#define PCINT23_PORT  PORTC
#define PCINT23_PIN   PINC
#define PCINT23_BIT   7

#define ADC7_DDR   DDRA
#define ADC7_PORT  PORTA
#define ADC7_PIN   PINA
#define ADC7_BIT   7

#define PCINT7_DDR   DDRA
#define PCINT7_PORT  PORTA
#define PCINT7_PIN   PINA
#define PCINT7_BIT   7

#define ADC6_DDR   DDRA
#define ADC6_PORT  PORTA
#define ADC6_PIN   PINA
#define ADC6_BIT   6

#define PCINT6_DDR   DDRA
#define PCINT6_PORT  PORTA
#define PCINT6_PIN   PINA
#define PCINT6_BIT   6

#define ADC5_DDR   DDRA
#define ADC5_PORT  PORTA
#define ADC5_PIN   PINA
#define ADC5_BIT   5

#define PCINT5_DDR   DDRA
#define PCINT5_PORT  PORTA
#define PCINT5_PIN   PINA
#define PCINT5_BIT   5

#define ADC4_DDR   DDRA
#define ADC4_PORT  PORTA
#define ADC4_PIN   PINA
#define ADC4_BIT   4

#define PCINT4_DDR   DDRA
#define PCINT4_PORT  PORTA
#define PCINT4_PIN   PINA
#define PCINT4_BIT   4

#define ADC3_DDR   DDRA
#define ADC3_PORT  PORTA
#define ADC3_PIN   PINA
#define ADC3_BIT   3

#define PCINT3_DDR   DDRA
#define PCINT3_PORT  PORTA
#define PCINT3_PIN   PINA
#define PCINT3_BIT   3

#define ADC2_DDR   DDRA
#define ADC2_PORT  PORTA
#define ADC2_PIN   PINA
#define ADC2_BIT   2

#define PCINT2_DDR   DDRA
#define PCINT2_PORT  PORTA
#define PCINT2_PIN   PINA
#define PCINT2_BIT   2

#define ADC1_DDR   DDRA
#define ADC1_PORT  PORTA
#define ADC1_PIN   PINA
#define ADC1_BIT   1

#define PCINT1_DDR   DDRA
#define PCINT1_PORT  PORTA
#define PCINT1_PIN   PINA
#define PCINT1_BIT   1

#define ADC0_DDR   DDRA
#define ADC0_PORT  PORTA
#define ADC0_PIN   PINA
#define ADC0_BIT   0

#define PCINT0_DDR   DDRA
#define PCINT0_PORT  PORTA
#define PCINT0_PIN   PINA
#define PCINT0_BIT   0

#define XCK_DDR   DDRB
#define XCK_PORT  PORTB
#define XCK_PIN   PINB
#define XCK_BIT   0

#define T0_DDR   DDRB
#define T0_PORT  PORTB
#define T0_PIN   PINB
#define T0_BIT   0

#define PCINT8_DDR   DDRB
#define PCINT8_PORT  PORTB
#define PCINT8_PIN   PINB
#define PCINT8_BIT   0

#define T1_DDR   DDRB
#define T1_PORT  PORTB
#define T1_PIN   PINB
#define T1_BIT   1

#define CLKO_DDR   DDRB
#define CLKO_PORT  PORTB
#define CLKO_PIN   PINB
#define CLKO_BIT   1

#define PCINT9_DDR   DDRB
#define PCINT9_PORT  PORTB
#define PCINT9_PIN   PINB
#define PCINT9_BIT   1

#define AIN0_DDR   DDRB
#define AIN0_PORT  PORTB
#define AIN0_PIN   PINB
#define AIN0_BIT   2

#define INT2_DDR   DDRB
#define INT2_PORT  PORTB
#define INT2_PIN   PINB
#define INT2_BIT   2

#define PCINT10_DDR   DDRB
#define PCINT10_PORT  PORTB
#define PCINT10_PIN   PINB
#define PCINT10_BIT   2

#define AIN1_DDR   DDRB
#define AIN1_PORT  PORTB
#define AIN1_PIN   PINB
#define AIN1_BIT   3

#define OC0A_DDR   DDRB
#define OC0A_PORT  PORTB
#define OC0A_PIN   PINB
#define OC0A_BIT   3

#define PCINT11_DDR   DDRB
#define PCINT11_PORT  PORTB
#define PCINT11_PIN   PINB
#define PCINT11_BIT   3

#define SS_DDR   DDRB
#define SS_PORT  PORTB
#define SS_PIN   PINB
#define SS_BIT   4

#define OC0B_DDR   DDRB
#define OC0B_PORT  PORTB
#define OC0B_PIN   PINB
#define OC0B_BIT   4

#define PCINT12_DDR   DDRB
#define PCINT12_PORT  PORTB
#define PCINT12_PIN   PINB
#define PCINT12_BIT   4


#define SLEEP_MODE_IDLE (0x00<<1)
#define SLEEP_MODE_ADC (0x01<<1)
#define SLEEP_MODE_PWR_DOWN (0x02<<1)
#define SLEEP_MODE_PWR_SAVE (0x03<<1)
#define SLEEP_MODE_STANDBY (0x06<<1)
#define SLEEP_MODE_EXT_STANDBY (0x07<<1)




/* Port Data Register (generic) */
#define    PORT7        7
#define    PORT6        6
#define    PORT5        5
#define    PORT4        4
#define    PORT3        3
#define    PORT2        2
#define    PORT1        1
#define    PORT0        0

/* Port Data Direction Register (generic) */
#define    DD7          7
#define    DD6          6
#define    DD5          5
#define    DD4          4
#define    DD3          3
#define    DD2          2
#define    DD1          1
#define    DD0          0

/* Port Input Pins (generic) */
#define    PIN7         7
#define    PIN6         6
#define    PIN5         5
#define    PIN4         4
#define    PIN3         3
#define    PIN2         2
#define    PIN1         1
#define    PIN0         0

/* Define PORTxn an Pxn values for all possible port pins if not defined already by io.h. */

/* PORT A */

#if defined(PA0) && !defined(PORTA0)
#  define PORTA0 PA0
#elif defined(PORTA0) && !defined(PA0)
#  define PA0 PORTA0
#endif
#if defined(PA1) && !defined(PORTA1)
#  define PORTA1 PA1
#elif defined(PORTA1) && !defined(PA1)
#  define PA1 PORTA1
#endif
#if defined(PA2) && !defined(PORTA2)
#  define PORTA2 PA2
#elif defined(PORTA2) && !defined(PA2)
#  define PA2 PORTA2
#endif
#if defined(PA3) && !defined(PORTA3)
#  define PORTA3 PA3
#elif defined(PORTA3) && !defined(PA3)
#  define PA3 PORTA3
#endif
#if defined(PA4) && !defined(PORTA4)
#  define PORTA4 PA4
#elif defined(PORTA4) && !defined(PA4)
#  define PA4 PORTA4
#endif
#if defined(PA5) && !defined(PORTA5)
#  define PORTA5 PA5
#elif defined(PORTA5) && !defined(PA5)
#  define PA5 PORTA5
#endif
#if defined(PA6) && !defined(PORTA6)
#  define PORTA6 PA6
#elif defined(PORTA6) && !defined(PA6)
#  define PA6 PORTA6
#endif
#if defined(PA7) && !defined(PORTA7)
#  define PORTA7 PA7
#elif defined(PORTA7) && !defined(PA7)
#  define PA7 PORTA7
#endif

/* PORT B */

#if defined(PB0) && !defined(PORTB0)
#  define PORTB0 PB0
#elif defined(PORTB0) && !defined(PB0)
#  define PB0 PORTB0
#endif
#if defined(PB1) && !defined(PORTB1)
#  define PORTB1 PB1
#elif defined(PORTB1) && !defined(PB1)
#  define PB1 PORTB1
#endif
#if defined(PB2) && !defined(PORTB2)
#  define PORTB2 PB2
#elif defined(PORTB2) && !defined(PB2)
#  define PB2 PORTB2
#endif
#if defined(PB3) && !defined(PORTB3)
#  define PORTB3 PB3
#elif defined(PORTB3) && !defined(PB3)
#  define PB3 PORTB3
#endif
#if defined(PB4) && !defined(PORTB4)
#  define PORTB4 PB4
#elif defined(PORTB4) && !defined(PB4)
#  define PB4 PORTB4
#endif
#if defined(PB5) && !defined(PORTB5)
#  define PORTB5 PB5
#elif defined(PORTB5) && !defined(PB5)
#  define PB5 PORTB5
#endif
#if defined(PB6) && !defined(PORTB6)
#  define PORTB6 PB6
#elif defined(PORTB6) && !defined(PB6)
#  define PB6 PORTB6
#endif
#if defined(PB7) && !defined(PORTB7)
#  define PORTB7 PB7
#elif defined(PORTB7) && !defined(PB7)
#  define PB7 PORTB7
#endif

/* PORT C */

#if defined(PC0) && !defined(PORTC0)
#  define PORTC0 PC0
#elif defined(PORTC0) && !defined(PC0)
#  define PC0 PORTC0
#endif
#if defined(PC1) && !defined(PORTC1)
#  define PORTC1 PC1
#elif defined(PORTC1) && !defined(PC1)
#  define PC1 PORTC1
#endif
#if defined(PC2) && !defined(PORTC2)
#  define PORTC2 PC2
#elif defined(PORTC2) && !defined(PC2)
#  define PC2 PORTC2
#endif
#if defined(PC3) && !defined(PORTC3)
#  define PORTC3 PC3
#elif defined(PORTC3) && !defined(PC3)
#  define PC3 PORTC3
#endif
#if defined(PC4) && !defined(PORTC4)
#  define PORTC4 PC4
#elif defined(PORTC4) && !defined(PC4)
#  define PC4 PORTC4
#endif
#if defined(PC5) && !defined(PORTC5)
#  define PORTC5 PC5
#elif defined(PORTC5) && !defined(PC5)
#  define PC5 PORTC5
#endif
#if defined(PC6) && !defined(PORTC6)
#  define PORTC6 PC6
#elif defined(PORTC6) && !defined(PC6)
#  define PC6 PORTC6
#endif
#if defined(PC7) && !defined(PORTC7)
#  define PORTC7 PC7
#elif defined(PORTC7) && !defined(PC7)
#  define PC7 PORTC7
#endif

/* PORT D */

#if defined(PD0) && !defined(PORTD0)
#  define PORTD0 PD0
#elif defined(PORTD0) && !defined(PD0)
#  define PD0 PORTD0
#endif
#if defined(PD1) && !defined(PORTD1)
#  define PORTD1 PD1
#elif defined(PORTD1) && !defined(PD1)
#  define PD1 PORTD1
#endif
#if defined(PD2) && !defined(PORTD2)
#  define PORTD2 PD2
#elif defined(PORTD2) && !defined(PD2)
#  define PD2 PORTD2
#endif
#if defined(PD3) && !defined(PORTD3)
#  define PORTD3 PD3
#elif defined(PORTD3) && !defined(PD3)
#  define PD3 PORTD3
#endif
#if defined(PD4) && !defined(PORTD4)
#  define PORTD4 PD4
#elif defined(PORTD4) && !defined(PD4)
#  define PD4 PORTD4
#endif
#if defined(PD5) && !defined(PORTD5)
#  define PORTD5 PD5
#elif defined(PORTD5) && !defined(PD5)
#  define PD5 PORTD5
#endif
#if defined(PD6) && !defined(PORTD6)
#  define PORTD6 PD6
#elif defined(PORTD6) && !defined(PD6)
#  define PD6 PORTD6
#endif
#if defined(PD7) && !defined(PORTD7)
#  define PORTD7 PD7
#elif defined(PORTD7) && !defined(PD7)
#  define PD7 PORTD7
#endif

/* PORT E */

#if defined(PE0) && !defined(PORTE0)
#  define PORTE0 PE0
#elif defined(PORTE0) && !defined(PE0)
#  define PE0 PORTE0
#endif
#if defined(PE1) && !defined(PORTE1)
#  define PORTE1 PE1
#elif defined(PORTE1) && !defined(PE1)
#  define PE1 PORTE1
#endif
#if defined(PE2) && !defined(PORTE2)
#  define PORTE2 PE2
#elif defined(PORTE2) && !defined(PE2)
#  define PE2 PORTE2
#endif
#if defined(PE3) && !defined(PORTE3)
#  define PORTE3 PE3
#elif defined(PORTE3) && !defined(PE3)
#  define PE3 PORTE3
#endif
#if defined(PE4) && !defined(PORTE4)
#  define PORTE4 PE4
#elif defined(PORTE4) && !defined(PE4)
#  define PE4 PORTE4
#endif
#if defined(PE5) && !defined(PORTE5)
#  define PORTE5 PE5
#elif defined(PORTE5) && !defined(PE5)
#  define PE5 PORTE5
#endif
#if defined(PE6) && !defined(PORTE6)
#  define PORTE6 PE6
#elif defined(PORTE6) && !defined(PE6)
#  define PE6 PORTE6
#endif
#if defined(PE7) && !defined(PORTE7)
#  define PORTE7 PE7
#elif defined(PORTE7) && !defined(PE7)
#  define PE7 PORTE7
#endif

/* PORT F */

#if defined(PF0) && !defined(PORTF0)
#  define PORTF0 PF0
#elif defined(PORTF0) && !defined(PF0)
#  define PF0 PORTF0
#endif
#if defined(PF1) && !defined(PORTF1)
#  define PORTF1 PF1
#elif defined(PORTF1) && !defined(PF1)
#  define PF1 PORTF1
#endif
#if defined(PF2) && !defined(PORTF2)
#  define PORTF2 PF2
#elif defined(PORTF2) && !defined(PF2)
#  define PF2 PORTF2
#endif
#if defined(PF3) && !defined(PORTF3)
#  define PORTF3 PF3
#elif defined(PORTF3) && !defined(PF3)
#  define PF3 PORTF3
#endif
#if defined(PF4) && !defined(PORTF4)
#  define PORTF4 PF4
#elif defined(PORTF4) && !defined(PF4)
#  define PF4 PORTF4
#endif
#if defined(PF5) && !defined(PORTF5)
#  define PORTF5 PF5
#elif defined(PORTF5) && !defined(PF5)
#  define PF5 PORTF5
#endif
#if defined(PF6) && !defined(PORTF6)
#  define PORTF6 PF6
#elif defined(PORTF6) && !defined(PF6)
#  define PF6 PORTF6
#endif
#if defined(PF7) && !defined(PORTF7)
#  define PORTF7 PF7
#elif defined(PORTF7) && !defined(PF7)
#  define PF7 PORTF7
#endif

/* PORT G */

#if defined(PG0) && !defined(PORTG0)
#  define PORTG0 PG0
#elif defined(PORTG0) && !defined(PG0)
#  define PG0 PORTG0
#endif
#if defined(PG1) && !defined(PORTG1)
#  define PORTG1 PG1
#elif defined(PORTG1) && !defined(PG1)
#  define PG1 PORTG1
#endif
#if defined(PG2) && !defined(PORTG2)
#  define PORTG2 PG2
#elif defined(PORTG2) && !defined(PG2)
#  define PG2 PORTG2
#endif
#if defined(PG3) && !defined(PORTG3)
#  define PORTG3 PG3
#elif defined(PORTG3) && !defined(PG3)
#  define PG3 PORTG3
#endif
#if defined(PG4) && !defined(PORTG4)
#  define PORTG4 PG4
#elif defined(PORTG4) && !defined(PG4)
#  define PG4 PORTG4
#endif
#if defined(PG5) && !defined(PORTG5)
#  define PORTG5 PG5
#elif defined(PORTG5) && !defined(PG5)
#  define PG5 PORTG5
#endif
#if defined(PG6) && !defined(PORTG6)
#  define PORTG6 PG6
#elif defined(PORTG6) && !defined(PG6)
#  define PG6 PORTG6
#endif
#if defined(PG7) && !defined(PORTG7)
#  define PORTG7 PG7
#elif defined(PORTG7) && !defined(PG7)
#  define PG7 PORTG7
#endif

/* PORT H */

#if defined(PH0) && !defined(PORTH0)
#  define PORTH0 PH0
#elif defined(PORTH0) && !defined(PH0)
#  define PH0 PORTH0
#endif
#if defined(PH1) && !defined(PORTH1)
#  define PORTH1 PH1
#elif defined(PORTH1) && !defined(PH1)
#  define PH1 PORTH1
#endif
#if defined(PH2) && !defined(PORTH2)
#  define PORTH2 PH2
#elif defined(PORTH2) && !defined(PH2)
#  define PH2 PORTH2
#endif
#if defined(PH3) && !defined(PORTH3)
#  define PORTH3 PH3
#elif defined(PORTH3) && !defined(PH3)
#  define PH3 PORTH3
#endif
#if defined(PH4) && !defined(PORTH4)
#  define PORTH4 PH4
#elif defined(PORTH4) && !defined(PH4)
#  define PH4 PORTH4
#endif
#if defined(PH5) && !defined(PORTH5)
#  define PORTH5 PH5
#elif defined(PORTH5) && !defined(PH5)
#  define PH5 PORTH5
#endif
#if defined(PH6) && !defined(PORTH6)
#  define PORTH6 PH6
#elif defined(PORTH6) && !defined(PH6)
#  define PH6 PORTH6
#endif
#if defined(PH7) && !defined(PORTH7)
#  define PORTH7 PH7
#elif defined(PORTH7) && !defined(PH7)
#  define PH7 PORTH7
#endif

/* PORT J */

#if defined(PJ0) && !defined(PORTJ0)
#  define PORTJ0 PJ0
#elif defined(PORTJ0) && !defined(PJ0)
#  define PJ0 PORTJ0
#endif
#if defined(PJ1) && !defined(PORTJ1)
#  define PORTJ1 PJ1
#elif defined(PORTJ1) && !defined(PJ1)
#  define PJ1 PORTJ1
#endif
#if defined(PJ2) && !defined(PORTJ2)
#  define PORTJ2 PJ2
#elif defined(PORTJ2) && !defined(PJ2)
#  define PJ2 PORTJ2
#endif
#if defined(PJ3) && !defined(PORTJ3)
#  define PORTJ3 PJ3
#elif defined(PORTJ3) && !defined(PJ3)
#  define PJ3 PORTJ3
#endif
#if defined(PJ4) && !defined(PORTJ4)
#  define PORTJ4 PJ4
#elif defined(PORTJ4) && !defined(PJ4)
#  define PJ4 PORTJ4
#endif
#if defined(PJ5) && !defined(PORTJ5)
#  define PORTJ5 PJ5
#elif defined(PORTJ5) && !defined(PJ5)
#  define PJ5 PORTJ5
#endif
#if defined(PJ6) && !defined(PORTJ6)
#  define PORTJ6 PJ6
#elif defined(PORTJ6) && !defined(PJ6)
#  define PJ6 PORTJ6
#endif
#if defined(PJ7) && !defined(PORTJ7)
#  define PORTJ7 PJ7
#elif defined(PORTJ7) && !defined(PJ7)
#  define PJ7 PORTJ7
#endif

/* PORT K */

#if defined(PK0) && !defined(PORTK0)
#  define PORTK0 PK0
#elif defined(PORTK0) && !defined(PK0)
#  define PK0 PORTK0
#endif
#if defined(PK1) && !defined(PORTK1)
#  define PORTK1 PK1
#elif defined(PORTK1) && !defined(PK1)
#  define PK1 PORTK1
#endif
#if defined(PK2) && !defined(PORTK2)
#  define PORTK2 PK2
#elif defined(PORTK2) && !defined(PK2)
#  define PK2 PORTK2
#endif
#if defined(PK3) && !defined(PORTK3)
#  define PORTK3 PK3
#elif defined(PORTK3) && !defined(PK3)
#  define PK3 PORTK3
#endif
#if defined(PK4) && !defined(PORTK4)
#  define PORTK4 PK4
#elif defined(PORTK4) && !defined(PK4)
#  define PK4 PORTK4
#endif
#if defined(PK5) && !defined(PORTK5)
#  define PORTK5 PK5
#elif defined(PORTK5) && !defined(PK5)
#  define PK5 PORTK5
#endif
#if defined(PK6) && !defined(PORTK6)
#  define PORTK6 PK6
#elif defined(PORTK6) && !defined(PK6)
#  define PK6 PORTK6
#endif
#if defined(PK7) && !defined(PORTK7)
#  define PORTK7 PK7
#elif defined(PORTK7) && !defined(PK7)
#  define PK7 PORTK7
#endif

/* PORT L */

#if defined(PL0) && !defined(PORTL0)
#  define PORTL0 PL0
#elif defined(PORTL0) && !defined(PL0)
#  define PL0 PORTL0
#endif
#if defined(PL1) && !defined(PORTL1)
#  define PORTL1 PL1
#elif defined(PORTL1) && !defined(PL1)
#  define PL1 PORTL1
#endif
#if defined(PL2) && !defined(PORTL2)
#  define PORTL2 PL2
#elif defined(PORTL2) && !defined(PL2)
#  define PL2 PORTL2
#endif
#if defined(PL3) && !defined(PORTL3)
#  define PORTL3 PL3
#elif defined(PORTL3) && !defined(PL3)
#  define PL3 PORTL3
#endif
#if defined(PL4) && !defined(PORTL4)
#  define PORTL4 PL4
#elif defined(PORTL4) && !defined(PL4)
#  define PL4 PORTL4
#endif
#if defined(PL5) && !defined(PORTL5)
#  define PORTL5 PL5
#elif defined(PORTL5) && !defined(PL5)
#  define PL5 PORTL5
#endif
#if defined(PL6) && !defined(PORTL6)
#  define PORTL6 PL6
#elif defined(PORTL6) && !defined(PL6)
#  define PL6 PORTL6
#endif
#if defined(PL7) && !defined(PORTL7)
#  define PORTL7 PL7
#elif defined(PORTL7) && !defined(PL7)
#  define PL7 PORTL7
#endif



#  ifndef SPL
#    define SPL (0x3D)
#  endif
#  ifndef SPH
#    define SPH (0x3E)
#  endif
#  ifndef SP
#    define SP (0x3D)
#  endif


/* Status Register */
#ifndef SREG
#  if __AVR_ARCH__ >= 100
#    define SREG (0x3F)
#  else
#    define SREG (0x3F)
#  endif
#endif


/* SREG bit definitions */
#ifndef SREG_C
#  define SREG_C  (0)
#endif
#ifndef SREG_Z
#  define SREG_Z  (1)
#endif
#ifndef SREG_N
#  define SREG_N  (2)
#endif
#ifndef SREG_V
#  define SREG_V  (3)
#endif
#ifndef SREG_S
#  define SREG_S  (4)
#endif
#ifndef SREG_H
#  define SREG_H  (5)
#endif
#ifndef SREG_T
#  define SREG_T  (6)
#endif
#ifndef SREG_I
#  define SREG_I  (7)
#endif


/* AVR 6 Architecture */
#  if __AVR_ARCH__ == 6
#    ifndef EIND
#      define EIND  (0X3C)
#    endif
/* XMEGA Architectures */
#  elif __AVR_ARCH__ >= 100
#    ifndef EIND
#      define EIND  (0x3C)
#    endif
#  endif

/*
 * Only few devices come without EEPROM.  In order to assemble the
 * EEPROM library components without defining a specific device, we
 * keep the EEPROM-related definitions here.
 */

/* EEPROM Control Register */
#  ifndef EECR
#    define EECR   (0x1C)
#  endif

/* EEPROM Data Register */
#  ifndef EEDR
#    define EEDR   (0x1D)
#  endif

/* EEPROM Address Register */
#  ifndef EEAR
#    define EEAR   _SFR_IO16(0x1E)
#  endif
#  ifndef EEARL
#    define EEARL  (0x1E)
#  endif
#  ifndef EEARH
#    define EEARH  (0x1F)
#  endif

/* EEPROM Control Register bits */
#  ifndef EERE
#    define EERE   (0)
#  endif
#  ifndef EEWE
#    define EEWE   (1)
#  endif
#  ifndef EEMWE
#    define EEMWE  (2)
#  endif
#  ifndef EERIE
#    define EERIE  (3)
#  endif


/* RAM Page Z Select Register	*/
#ifndef RAMPZ
#  if     defined(__AVR_HAVE_RAMPZ__) && __AVR_HAVE_RAMPZ__
#    if     __AVR_ARCH__ >= 100
#      define RAMPZ	(0x3B)
#    else
#      define RAMPZ	(0x3B)
#    endif
#  endif
#endif



/*------------ Common Symbols ------------*/

/* 
 * Generic definitions for registers that are common across multiple AVR devices
 * and families.
 */

/* Pointer registers definitions */
#if __AVR_ARCH__ != 1  /* avr1 does not have X and Y pointers */
#  define XL  r26
#  define XH  r27
#  define YL  r28
#  define YH  r29
#endif /* #if __AVR_ARCH__ != 1 */
#define ZL  r30
#define ZH  r31


/* Status Register */
#if defined(SREG)
#  define AVR_STATUS_REG   SREG
#  if __AVR_ARCH__ >= 100
#    define AVR_STATUS_ADDR  _SFR_MEM_ADDR(SREG)
#  else
#    define AVR_STATUS_ADDR  _SFR_IO_ADDR(SREG)
#  endif
#endif

/* Stack Pointer (combined) Register */
#if defined(SP)
#  define AVR_STACK_POINTER_REG   SP
#  if __AVR_ARCH__ >= 100
#    define AVR_STACK_POINTER_ADDR  _SFR_MEM_ADDR(SP)
#  else
#    define AVR_STACK_POINTER_ADDR  _SFR_IO_ADDR(SP)
#  endif
#endif

/* Stack Pointer High Register */
#if defined(SPH)
#  define _HAVE_AVR_STACK_POINTER_HI 1
#  define AVR_STACK_POINTER_HI_REG   SPH
#  if __AVR_ARCH__ >= 100
#    define AVR_STACK_POINTER_HI_ADDR  _SFR_MEM_ADDR(SPH)
#  else
#    define AVR_STACK_POINTER_HI_ADDR  _SFR_IO_ADDR(SPH)
#  endif
#endif

/* Stack Pointer Low Register */
#if defined(SPL)
#  define AVR_STACK_POINTER_LO_REG   SPL
#  if __AVR_ARCH__ >= 100
#    define AVR_STACK_POINTER_LO_ADDR  _SFR_MEM_ADDR(SPL)
#  else
#    define AVR_STACK_POINTER_LO_ADDR  _SFR_IO_ADDR(SPL)
#  endif
#endif

/* RAMPD Register */
#if defined(RAMPD)
#  define AVR_RAMPD_REG   RAMPD
#  if __AVR_ARCH__ >= 100
#    define AVR_RAMPD_ADDR  _SFR_MEM_ADDR(RAMPD)
#  else
#    define AVR_RAMPD_ADDR  _SFR_IO_ADDR(RAMPD)
#  endif
#endif

/* RAMPX Register */
#if defined(RAMPX)
#  define AVR_RAMPX_REG   RAMPX
#  if __AVR_ARCH__ >= 100
#    define AVR_RAMPX_ADDR  _SFR_MEM_ADDR(RAMPX)
#  else
#    define AVR_RAMPX_ADDR  _SFR_IO_ADDR(RAMPX)
#  endif
#endif

/* RAMPY Register */
#if defined(RAMPY)
#  define AVR_RAMPY_REG   RAMPY
#  if __AVR_ARCH__ >= 100
#    define AVR_RAMPY_ADDR  _SFR_MEM_ADDR(RAMPY)
#  else
#    define AVR_RAMPY_ADDR  _SFR_IO_ADDR(RAMPY)
#  endif
#endif

/* RAMPZ Register */
#if defined(RAMPZ)
#  define AVR_RAMPZ_REG   RAMPZ
#  if __AVR_ARCH__ >= 100
#    define AVR_RAMPZ_ADDR  _SFR_MEM_ADDR(RAMPZ)
#  else
#    define AVR_RAMPZ_ADDR  _SFR_IO_ADDR(RAMPZ)
#  endif
#endif

/* Extended Indirect Register */
#if defined(EIND)
#  define AVR_EXTENDED_INDIRECT_REG   EIND
#  if __AVR_ARCH__ >= 100
#    define AVR_EXTENDED_INDIRECT_ADDR  _SFR_MEM_ADDR(EIND)
#  else
#    define AVR_EXTENDED_INDIRECT_ADDR  _SFR_IO_ADDR(EIND)
#  endif
#endif

/*------------ Workaround to old compilers (4.1.2 and earlier)  ------------*/

#ifndef __AVR_HAVE_MOVW__
# if  defined(__AVR_ENHANCED__) && __AVR_ENHANCED__
#  define __AVR_HAVE_MOVW__ 1
# endif
#endif

#ifndef __AVR_HAVE_LPMX__
# if  defined(__AVR_ENHANCED__) && __AVR_ENHANCED__
#  define __AVR_HAVE_LPMX__ 1
# endif
#endif

#ifndef __AVR_HAVE_MUL__
# if  defined(__AVR_ENHANCED__) && __AVR_ENHANCED__
#  define __AVR_HAVE_MUL__ 1
# endif
#endif

#define __AVR_LIBC_VERSION_STRING__ "2.0.0"

/** \ingroup avr_version
 *    Numerical representation of the current library version.
 * 
 *    In the numerical representation, the major number is multiplied by
 *    10000, the minor number by 100, and all three parts are then
 *    added.  It is intented to provide a monotonically increasing
 *    numerical value that can easily be used in numerical checks.
 */
#define __AVR_LIBC_VERSION__        20000UL

/** \ingroup avr_version
 *    String literal representation of the release date. */
#define __AVR_LIBC_DATE_STRING__    "20150208"

/** \ingroup avr_version
 *    Numerical representation of the release date. */
#define __AVR_LIBC_DATE_            20150208UL

/** \ingroup avr_version
 *    Library major version number. */
#define __AVR_LIBC_MAJOR__          2

/** \ingroup avr_version
 *    Library minor version number. */
#define __AVR_LIBC_MINOR__          0

/** \ingroup avr_version
 *    Library revision number. */
#define __AVR_LIBC_REVISION__       0



#define _AVR_FUSE_H_ 1


#ifndef FUSEMEM
#define FUSEMEM  __attribute__((__used__, __section__ (".fuse")))
#endif

#if FUSE_MEMORY_SIZE > 3

typedef struct
{
    unsigned char byte[FUSE_MEMORY_SIZE];
} __fuse_t;


#elif FUSE_MEMORY_SIZE == 3

typedef struct
{
    unsigned char low;
    unsigned char high;
    unsigned char extended;
} __fuse_t;

#elif FUSE_MEMORY_SIZE == 2

typedef struct
{
    unsigned char low;
    unsigned char high;
} __fuse_t;

#elif FUSE_MEMORY_SIZE == 1

typedef struct
{
    unsigned char byte;
} __fuse_t;

#endif

#if !defined(FUSES)
#if defined(__AVR_XMEGA__)
#define FUSES NVM_FUSES_t __fuse FUSEMEM
#else
#define FUSES __fuse_t __fuse FUSEMEM
#endif
#endif




#ifndef LOCKMEM
#define LOCKMEM  __attribute__((__used__, __section__ (".lock")))
#endif

#ifndef LOCKBITS
#define LOCKBITS unsigned char __lock LOCKMEM
#endif

/* Lock Bit Modes */
#if defined(__LOCK_BITS_EXIST)
#define LB_MODE_1  (0xFF)
#define LB_MODE_2  (0xFE)
#define LB_MODE_3  (0xFC)
#endif

#if defined(__BOOT_LOCK_BITS_0_EXIST)
#define BLB0_MODE_1  (0xFF)
#define BLB0_MODE_2  (0xFB)
#define BLB0_MODE_3  (0xF3)
#define BLB0_MODE_4  (0xF7)
#endif

#if defined(__BOOT_LOCK_BITS_1_EXIST)
#define BLB1_MODE_1  (0xFF)
#define BLB1_MODE_2  (0xEF)
#define BLB1_MODE_3  (0xCF)
#define BLB1_MODE_4  (0xDF)
#endif

#if defined(__BOOT_LOCK_APPLICATION_TABLE_BITS_EXIST)
#define BLBAT0 ~_BV(2)
#define BLBAT1 ~_BV(3)
#endif

#if defined(__BOOT_LOCK_APPLICATION_BITS_EXIST)
#define BLBA0 ~_BV(4)
#define BLBA1 ~_BV(5)
#endif

#if defined(__BOOT_LOCK_BOOT_BITS_EXIST)
#define BLBB0 ~_BV(6)
#define BLBB1 ~_BV(7)
#endif


#define LOCKBITS_DEFAULT (0xFF)
