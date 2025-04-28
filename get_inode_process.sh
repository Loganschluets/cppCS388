#!/bin/bash
INODE=$1
echo $1
for pid in $(ls /proc/ | grep '^[0-9]'); do
  ls -l /proc/$pid/fd 2>/dev/null | grep "socket:\[$INODE\]" && echo "PID: $pid"
done