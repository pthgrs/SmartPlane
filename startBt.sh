#!/bin/bash

sudo systemctl start bluetooth

sleep 1

echo -e "power on\ndiscoverable yes\nquit\n" | bluetoothctl

