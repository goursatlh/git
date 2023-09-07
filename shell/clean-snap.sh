#!/bin/bash
# Removes old revisions of snaps
# CLOSE ALL SNAPS BEFORE RUNNING THIS

# find the biggest directiory in the system
echo "The biggest directory : "
du -aBM -d 1 / | sort -nr | head -20


# 1, clean the unused snap files
set -eu
snap list --all | awk '/disabled/{print $1, $3}' |
while read snapname revision; do
snap remove "$snapname" --revision="$revision"
done

# 2, clean the unused module files
sudo apt remove $(dpkg-query --show 'linux-modules-*' | cut -f1 | grep -v "$(uname -r)")







