#!/bin/bash

# Only run for wlan0 interface
if [ "$interface" != "wlan0" ]; then
    exit 0
fi

CONFIG_DIR="/Users/yooniverse/Documents/Git/CPSC3340/pi_config"

cp /boot/firmware/config.txt "$CONFIG_DIR/config.txt"

cat > "$CONFIG_DIR/ip.md" <<'EOF'
```
                                        /$$
                                       |__/
  /$$$$$$  /$$$$$$   /$$$$$$$  /$$$$$$  /$$
 /$$__  $$|____  $$ /$$_____/ /$$__  $$| $$
| $$  \__/ /$$$$$$$|  $$$$$$ | $$  \ $$| $$
| $$      /$$__  $$ \____  $$| $$  | $$| $$
| $$     |  $$$$$$$ /$$$$$$$/| $$$$$$$/| $$
|__/      \_______/|_______/ | $$____/ |__/
                             | $$          
                             | $$          
                             |__/          
```

Hello, Eric <3

EOF

echo "My IP is \`$new_ip_address\`." >> "$CONFIG_DIR/ip.md"

cd "$CONFIG_DIR"
git pull
git add .
git commit -m "[Auto commit] Config backup"
git push
