#!/bin/bash
TIME_LIMIT=30
SECONDS=0
octave -q test.m &
PID=$!
while [ `ps -p $PID --no-headers | wc -l` -gt 0 ]; do
	sleep 1
	if [ $SECONDS -gt $TIME_LIMIT ]; then
		kill -KILL $PID
	fi
done
