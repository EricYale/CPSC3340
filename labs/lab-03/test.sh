#!/bin/bash

sudo pinctrl set 2 op
sudo pinctrl set 3 ip pd
sudo pinctrl set 4 ip pd
sudo pinctrl set 17 ip pu

while true
do
  tput cup 1 0
  sudo pinctrl get 3
  sudo pinctrl get 4
  sudo pinctrl get 17
done

