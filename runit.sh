#!/bin/bash

# USAGE: 
# To run xtra 
#   ./runit.sh <xas file> 

EXE=xtra

if [ ! -x ./xas ]; then
  make xas
  if [ ! -x ./xas ]; then
    echo Error, could not build the assembler, xas.  Contact the professor.
    exit 1
  fi
fi
  
if [ -x $EXE ]; then
        EXECDIR=.
elif [ -x  cmake-build-debug/$EXE ]; then
        EXECDIR=cmake-build-debug
else
        echo Cannot find $EXE
        echo You may need to build it first.
        echo Try running make.
        exit
fi

if [ $1"X" == "X" ]; then
        echo No file to run
        exit 1
fi
BASE=`basename $1 .xas`

echo Assembling $BASE.xas
rm -f $BASE.xo $BASE.s 
./xas $1 $BASE.xo
if [ ! -f $BASE.xo ]; then
  echo $BASE.xo not created.
  exit 1
fi

echo Translating $BASE.xo to $BASE.s
$EXECDIR/$EXE $BASE.xo > $BASE.s
if [ ! -f $BASE.s ]; then
  echo $BASE.s not created.
  exit 1
fi

echo Compiling runner for $BASE.s
rm -f runner
gcc -o runner runner.c regsdump.s $BASE.s
if [ ! -x runner ]; then
  echo runner not created.
  exit 1
fi

echo Running runner for $BASE.s
./runner
