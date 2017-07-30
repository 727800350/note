#!/bin/bash
source ~/.bashrc

javac -classpath $CLASSPATH:. reader_kv.java
java  -classpath $CLASSPATH:. reader_kv $1

