#!/bin/bash
INODE=485952
for pid in $(ls /proc | grep -E '^[0-9]+$'); do
  for fd in /proc/$pid/fd/*; do
    if [ -L "$fd" ]; then
      link=$(readlink "$fd")
      if [[ "$link" == "socket:[$INODE]" ]]; then
        echo "Process $pid is using socket inode $INODE"
        ps -p $pid -o pid,comm,user
      fi
    fi
  done
done