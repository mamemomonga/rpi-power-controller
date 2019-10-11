#ifndef _MAIN_H_
#define _MAIN_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// -----------------------------------

// /RST |     | VCC
// PB3  |     | PB2
// PB4  |     | PB1
// GND  |     | PB0

// PiがONである(IN:Active High)
// dtoverlay=gpio-poweroff,gpiopin=27,active_low="y"
#define PI_ON ( 1<<PB3 )
#define PI_ON_INIT { DDRB &=~ PI_ON; PORTB &=~PI_ON; }
#define PI_ON_IS_H ( PINB & PI_ON )
#define PI_IS_ON   ( PI_ON_IS_H )
#define PI_IS_OFF  ( ! PI_ON_IS_H )

// MOSFETスイッチ(OUT:Active High)
#define PWR ( 1<<PB4 )
#define PWR_INIT DDRB  |=  PWR
#define PWR_H    PORTB |=  PWR 
#define PWR_L    PORTB &=~ PWR 
#define PWR_I    PORTB ^=  PWR
#define PWR_ON   PWR_H
#define PWR_OFF  PWR_L

// LED (OUT:Active High)
#define LED ( 1<<PB2 )
#define LED_INIT DDRB  |=  LED
#define LED_H    PORTB |=  LED
#define LED_L    PORTB &=~ LED
#define LED_I    PORTB ^=  LED

// トグルスイッチ(IN:Active Low)
#define SW ( 1<<PB1 )
#define SW_INIT { DDRB &=~ SW; PORTB |= SW; } // PULLUP
#define SW_IS_H ( PINB & SW )
#define SW_ON  ( ! SW_IS_H ) // スイッチオンはLOW
#define SW_OFF ( SW_IS_H ) // スイッチオンはHIGH

// PI状態 (IN:Active High)
// シャットダウン指令 (OUT:Active High)
#define PI_ST ( 1<<PB0 )
#define PI_ST_IN   { DDRB &=~ PI_ST; PORTB &=~PI_ST; }
#define PI_ST_IS_H ( PINB & PI_ST )
#define PI_ST_OUT DDRB  |=  PI_ST
#define PI_ST_H   PORTB |=  PI_ST
#define PI_ST_L   PORTB &=~ PI_ST
#define PI_ST_I   PORTB ^=  PI_ST

// -----------------------------------

// PWMの周波数
#define PWM_HZ 75

// 点滅速度
// perl -E 'foreach(1..75) { say if (75 % $_ == 0) }'

// 点滅
uint8_t led_blink;
#define LED_ON        { led_blink=0;  timer_led=0; LED_H; }
#define LED_OFF       { led_blink=0;  timer_led=0; LED_L; }
#define LED_SLOW      { led_blink=75; timer_led=0; }
#define LED_FAST      { led_blink=15; timer_led=0; }
#define LED_ULTRAFAST { led_blink=3;  timer_led=0; }

// LEDカウンタ
uint8_t timer_led;

// 1秒カウンタ
uint8_t timer_onesec;

// タイムアウトカウンタ
uint8_t timeout;

// タイムアウト(秒) 最大255秒
#define TIMEOUT 60

// タイムアウトリセット
#define TIMEOUT_RESET { timeout=0; timer_onesec=0; }

// 状態
uint8_t action;
#define ACTION_SLEEP   0
#define ACTION_ST      1
#define ACTION_WA      2
#define ACTION_WA_PI   3
#define ACTION_OK_PI   4
#define ACTION_WA_STDN 6

#endif

