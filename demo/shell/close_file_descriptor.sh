#!/bin/bash

exec 3> log
echo "this is a test line of data" >&3

exec 3>&-
echo "this will not work" >&3

