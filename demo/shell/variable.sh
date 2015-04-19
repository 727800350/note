#!/bin/bash

var="value"

echo "the value of var: $var"
## the value of var: value

echo "this is the $var able"
## this is the value able

echo "this is the $variable"
## this is the 

echo "this is the ${var}iable"
## this is the valueiable

var="new_value"

echo "the value of var: $var"
## the value of var: new_value

echo 'the value of var: $var'
## the value of var: $var
