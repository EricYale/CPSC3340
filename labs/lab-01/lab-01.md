# Lab 1

## Setting Up Github Access on Pi
Instead of using an SSH key, I decided to use a Personal Access Token instead.

I first created a PAT on Github under [Developer Settings](https://github.com/settings/apps).

Then, I configured Git on the RPi to memorize my credentials:
```bash
git config credential.helper store
```

Finally, I did a `git push`, and when prompted for my password, I pasted in my PAT.

Since Git is configured to memorize my credentials, we can automate Git actions without needing a user to authenticate.

## Setting up SSH

I already had an SSH keypair for my laptop, so adding the public key to the Pi was as easy as editing the `.ssh/authorized_keys` file.
