#!/bin/bash
foo="hello"
foo="$foo world"
echo $foo
a="hello"
b="world"
c=$a$b
echo $c
echo ${#foo}