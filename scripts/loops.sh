#!/bin/bash
# shellcheck disable=SC2045
for f in $( ls /var/ ); do
  echo $f
done

COUNT=6
while [ $COUNT -gt 0 ]; do
  echo Count : $COUNT
  # shellcheck disable=SC2219
  let COUNT=COUNT-1
done