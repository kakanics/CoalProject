; This example program displays different types of data on a 16x2 LCD.
;
; Data Pin Connections for 16x2 LCD
; [LCD pins]          [Arduino UNO Pins]
; RS   ---------------  8 (PB0)
; E    ---------------  9 (PB1)
; D4   ---------------  4 (PD4)
; D5   ---------------  5 (PD5)
; D6   ---------------  6 (PD6)
; D7   ---------------  7 (PD7)
; A    ---------------  13 (PB5)	; Anode pin of LCD Backlight LED

.include "m328pdef.inc"
.include "delay_Macro.inc"
.include "1602_LCD_Macros.inc"
.include "UART_Macros.inc"

.cseg
.org 0x0000

SBI DDRB, PB3            ; Set PB3 pin for Output to LED
CBI PORTB, PB3           ; LED OFF

SBI DDRB, PB4            ; Set PB4 pin for Output to LED
CBI PORTB, PB4           ; LED OFF

.def A  = r16            ; Rename or attach a label to the register
.def AH = r17	

; ADC config
LDI   A, 0b11000111      ; [ADEN ADSC ADATE ADIF ADIE ADIE ADPS2 ADPS1 ADPS0]
STS   ADCSRA, A          
LDI   A, 0b01100000      ; [REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0]
STS   ADMUX, A           ; Select ADC0 (PC0) pin
SBI   PORTC, PC0         ; Enable Pull-up Resistor

Serial_begin
LCD_init                 ; Initialize the 16x2 LCD
LCD_backlight_OFF        ; Turn Off the LCD Backlight
delay 1000
LCD_backlight_ON         ; Turn On the LCD Backlight
delay 300

; Initialize variables for strings and lengths
LDI ZL, LOW (2 * dark_string)
LDI ZH, HIGH (2 * dark_string)
LDI R20, DarkString_len
LCD_send_a_string
delay 500

loop:
    LDS  A, ADCSRA        ; Start Analog to Digital Conversion
    ORI  A, (1<<ADSC)
    STS  ADCSRA, A

    wait:
        LDS  A, ADCSRA    ; Wait for ADC conversion to complete
        sbrc A, ADSC
        rjmp wait

    LDS  A, ADCL          ; Must Read ADCL before ADCH
    LDS  AH, ADCH
    delay 100              ; Delay 100ms

    cpi  AH, 200           ; Compare LDR reading with the desired threshold value (e.g., 200)
    brlo LED_OFF           ; Jump if AH < 200
    SBI  PORTB, PB4        ; LED ON
    CBI  PORTB, PB3        ; LED OFF
    rjmp LED_ON_End        ; Jump to skip LED_OFF condition

LED_OFF:
    CBI  PORTB, PB4        ; LED OFF
    SBI  PORTB, PB3        ; LED ON

LED_ON_End:
    ; LCD Configuration
    LCD_send_a_command 0x80
    delay 20

    ; Scroll text on LCD
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14
    LCD_send_a_command 0x14

    ; Send data to UART and LCD
    Serial_writeReg AH   ; Send register data to UART
    delay 50

    LCD_send_a_register AH ; Send register data to LCD
    delay 50

    ; Display spaces on LCD
    LDI ZL, LOW (2 * spaces)
    LDI ZH, HIGH (2 * spaces)
    LDI R20, SpacesString_len
    LCD_send_a_string
    delay 50

rjmp loop

dark_string: .db "Darkness:", 0
spaces:      .db "   ", 0

; Calculate string lengths
len: .equ DarkString_len   = (2 * (len - dark_string)) - 1
Spacelen: .equ SpacesString_len = (2 * (len - spaces)) - 1

; It is recommended to define constants (arrays, strings, etc.) at the end of the code segment
; .db directive is used to declare constants
; The length of the string must be an even number of bytes

; ***************************************************************************
; * Code written by:                                                      *
; * Affan, Areej, Ramsha, Hamna                                            *
; * Students, University of Engineering and Technology Lahore, Pakistan    *
; * 24-December-2023                                                      *
; ***************************************************************************


(ye code improve kr k Diya isne comment wagera daal k ye dekhlo(wese mujja to wesa ka wesa hi lgra)