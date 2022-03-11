#!/usr/bin/env bash

set -ex

BASEDIR=$(dirname "$0")
ENV=$BASEDIR/'../.env' 

ARGV=""

declare -a KEYS=("WEB_PATH" "PORT" "N_THREADS" "N_BUFFS")

for i in "${KEYS[@]}"
do
  ARGV+="$(grep $i $ENV | cut -d '=' -f2) "
done

echo -e $ARGV
