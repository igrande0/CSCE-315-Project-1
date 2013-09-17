#!/bin/bash

echo make parsertest
make parsertest
sleep 1
echo parsertest < input.txt &> output.txt
parsertest < input.txt 2>&1 > output.txt

