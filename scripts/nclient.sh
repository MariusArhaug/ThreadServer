#!/usr/bin/env bash

SEQ=/bin/seq
CURL=/bin/curl
IP=localhost
PORT=8080
PATH=/doc/index.html

URL=$IP:$PORT$PATH

END=10

if [ -z "$1" ]
then
  echo "Request number not specified, setting it to 10 requests"
else
  END=$1
fi 

for i in `$SEQ $END`
do
  echo "i:$i curl -S -s -o /dev/null $URL"
  $CURL -S -s -o /dev/null $URL &
done