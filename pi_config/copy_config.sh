#!/bin/bash
IP_ADDR=$(ifconfig | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' | grep -Eo '([0-9]*\.){3}[0-9]*' | grep -v '127.0.0.1')

cd ~/Desktop/CPSC3340/config_files

cp /boot/firmware/config.txt .

cat > ip.md <<'EOF'
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

Hello, Eric. <3

EOF

echo "My IP is \`$IP_ADDR\`." >> ip.md

git pull
git add .
git commit -m "[Auto commit] Config backup"
git push
