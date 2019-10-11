# 動作デバイス ( リスト: avr-as -mlist-devices )
DEVICE := attiny13

# 動作クロック(Hz)
CLOCK := 9600000

# プログラマ ( リスト: avrdude -c ? )
PROGRAMMER := -c avrisp2

# ヒューズ
# Int RC OSC. 9.6MHz
# Brown-out detection 4.3V
FUSES := -U lfuse:w:0x7a:m -U hfuse:w:0xff:m

# 使用オリジナルライブラリ
MLIBS =

include ./build.mk

