# Raspberry Piパワーコントローラ

* トグルスイッチでラズベリーパイのON/OFFを行う。
* スイッチオンでブート開始、オフでシャットダウンシーケンス開始。

![回路図](graffle/images.png)

Controller | 方向   | Raspberry Pi
-----------|--------|---------------
+5V        | 出力   | +5V
+3.3V      | 入力   | +3.3V
GND        |        | GND
PI_ON      | 入力   | GPIO 27
PI_ST      | 入出力 | GPIO 22

# Raspberry Pi側の設定

Raspbian用

* [boot-config.txt](rpi/boot-config.txt) を /boot/config.txt に追記
* [state-toggle.sh](rpi/state-toggle.sh]) を実行権限をつけて適当なところにおいて、/etc/rc.local などからバックグラウンドで起動する。

