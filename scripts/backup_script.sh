#!/bin/bash
mkdir backup
while [ true ]; do
  tar --exclude backup . $(date + %Y%m%d%H%M%S)
  sleep 180000
done