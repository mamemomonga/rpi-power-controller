#!/bin/bash
set -eu

GPIOPORT=22

echo $GPIOPORT > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio$GPIOPORT/direction
echo 0 > /sys/class/gpio/gpio$GPIOPORT/value
sleep 1

echo in > /sys/class/gpio/gpio$GPIOPORT/direction
while true; do
        if [ "$(cat /sys/class/gpio/gpio$GPIOPORT/value)" == "1" ]; then
                poweroff
                exit
        fi
        sleep 1
done
