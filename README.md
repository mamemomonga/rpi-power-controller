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

