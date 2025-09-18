# Lab 2

In this lab, I automated a way to get the IP of the RPi, so I can control it even when it is running in a headless configuration (with no monitor).

## Auto Push of Config Files to Github

### Initial attempt: network daemon
Initially, I tried making an `if-up.d` script to be run by Linux DHCP hooks system. However, after many attempts, this didn't end up working.


### Second attempt: systemd

I was able to get the automatic IP publisher script working through a systemd service.

I've done something similar with RasPis before, so I used an old systemd service specification file from a [previous project](https://github.com/EricYale/CubeSatOS/blob/main/email-notify/email-notify-up.service). Based on this, I made the `copy_config.service` file.

I then followed [this tutorial](https://medium.com/@benmorel/creating-a-linux-service-with-systemd-611b5c8b91d6) to add my service to be run by systemd.

It was as simple as typing
```bash
sudo cp copy_config.service /etc/systemd/system
sudo systemctl enable copy_config
```

Initially, it didn't work, but that's because I forgot to `chmod` execute permissions to `copy_config.sh`.

## Auto SCP

First, I generated a keypair on my RasPi. I added the public key to my `.ssh/authorized_keys` file (although I will later remove it because it makes me uncomfortable)

I enabled "Remote Login" under macOS settings.

I set my editor to nano because f*ck vim...
```bash
export EDITOR="nano"
```

and then I edited my crontab (on the pi).
```bash
crontab -e
```

I added the following line to the crontab
```
0,10,20,30,40,50 * * * * scp ~/CPSC3340/pi_config/ip.md yooniverse@10.66.69.213:~/Desktop/ip.md
```
