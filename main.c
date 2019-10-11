#include "main.h"

// タイマ割り込み
ISR(TIM0_COMPA_vect) {
	// LED点滅
	if(led_blink != 0) {
		if(timer_led == led_blink) {
			LED_I;
			timer_led=0;
		} else {
			timer_led++;
		}
	}
	// 1秒
	if(timer_onesec == PWM_HZ) {
		// タイムアウト
		timeout++;
		timer_onesec=0;
	} else {
		timer_onesec++;
	}
}

// INT0割り込み
// これ書いておかないときちんと動かない
ISR(INT0_vect) {}

// タイマ開始
void start_timer(void) {
	// clk(I/O)/1024
	TCCR0B = (1<<CS02)|(1<<CS00);

	// Waveform Generation Mode: 1 PWM (Phase Correct)
	TCCR0A = (1<<WGM01);

	// Timer/Counter0 Overflow Interrupt Enable
	TIMSK0 = (1<<OCIE0A);  

	// トップ値を設定
	OCR0A = 0x7d;

	GIMSK = 0; // INT割り込み無効
}

// アクション
void do_action(void) {
	switch(action) {

	// スリープ
	case ACTION_SLEEP:
		// 状態無効
		action = ACTION_SLEEP;
	
		TIMSK0 = 0; // タイマ停止
		LED_OFF;
		PWR_OFF;
	
		cli();
		GIMSK = (1<<INT0); // INT0割り込み有効
		MCUCR &=~ ((1<<ISC01)|(1<<ISC00)); // INT0がLOWで再開
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sei();
		sleep_enable();
		sleep_cpu();
	
		// 再開
		// スイッチが本当にONなのか複数確認する
		uint8_t ng=0;
		for(uint8_t i=0;i<32;i++) {
			if(SW_OFF) {
				ng=1;
			}
			_delay_ms(1);
		}
		// 気のせいだった
		if(ng) {
			return;
		}

		start_timer();
		LED_OFF;
		GIMSK = 0; // INT割り込み無効
		action = ACTION_ST; // 起動開始
		return;
	
		break;

   	// 起動開始
	case ACTION_ST:
		LED_ULTRAFAST;
		// Pi電源通電
		_delay_ms(500);
		PWR_ON;
		// Pi電源が入るまでPIOKが不定なので遅延
		_delay_ms(100);
		action = ACTION_WA;
		return;
		break;

	// 起動待機
	case ACTION_WA:
		// Piの電源が入った
		if(PI_IS_ON) {
			action = ACTION_WA_PI;
			LED_FAST;
			// PI_STを入力にする
			_delay_ms(100);
			PI_ST_IN;
			return;
		}
		// トグルスイッチOFFなら電源遮断
		if(SW_OFF) {
			LED_ULTRAFAST;
			_delay_ms(500);
			action = ACTION_SLEEP;
			return;
		}
		break;

	// Piの起動を待機
	case ACTION_WA_PI:
		if(!PI_ST_IS_H) {
			// PI_STを出力にする
			PI_ST_OUT;
			PI_ST_L;
			LED_ON;
			action = ACTION_OK_PI;
			return;
		}
		// トグルスイッチOFFなら電源遮断
		if(SW_OFF) {
			LED_ULTRAFAST;
			_delay_ms(500);
			action = ACTION_SLEEP;
			return;
		}
		break;

	// Piが稼働中
	case ACTION_OK_PI:
		// トグルスイッチOFFなら電源遮断開始
		if(SW_OFF) {
			LED_SLOW;
			// Piにシャットダウンを指示
			PI_ST_H;
			// タイムアウトをリセット
			TIMEOUT_RESET;
			action = ACTION_WA_STDN;
			return;
		}

		// Piの電源が切れている
		if(PI_IS_OFF) {
			// 電源オフ
			PWR_OFF;
			LED_FAST;
			_delay_ms(5000);
			// トグルスイッチがONのままならば、5秒後に再度電源が入る
			action = ACTION_SLEEP;
			return;
		}
		break;

	// シャットダウン中
	case ACTION_WA_STDN:
		// Piの電源オフを検知
		if(PI_IS_OFF) {
			// 1秒まって電源オフ
			_delay_ms(1000);
			action = ACTION_SLEEP;
			return;
		}
		// タイムアウト発生
		if(timeout > TIMEOUT) {

			// 5秒高速点灯して電源オフ
			PWR_OFF;
			LED_ULTRAFAST;
			_delay_ms(5000);
			action = ACTION_SLEEP;

			return;
		}
		break;
	}
}

int main(void) {

	// 初期化
	cli();

	LED_INIT;
	PWR_INIT;
	PI_ON_INIT;
	SW_INIT;

	PWR_OFF;
	LED_L;

	sei();

	// タイマ開始
	start_timer();

	LED_ULTRAFAST;
	_delay_ms(1000);
	LED_L;

	if(SW_ON) {
		action = ACTION_ST;
	} else {
		action = ACTION_SLEEP;
	}

	for(;;){
		do_action();
	}

	return 0;
}
