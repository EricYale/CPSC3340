#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
IP_ADDR=$(ifconfig | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' | grep -Eo '([0-9]*\.){3}[0-9]*' | grep -v '127.0.0.1')

cp /boot/firmware/config.txt "$SCRIPT_DIR/config.txt"

cat > "$SCRIPT_DIR/ip.md" <<'EOF'
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

Hello, Eric.

EOF

echo "My IP is \`$IP_ADDR\`." >> "$SCRIPT_DIR/ip.md"

cd "$SCRIPT_DIR"
git pull
git add .
git commit -m "[Auto commit] Config backup"
git push
