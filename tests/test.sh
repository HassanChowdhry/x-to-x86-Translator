#!/bin/sh

if [ ! -x xas ]; then
  make xas 
fi

echo ======================================================
echo ====================== TEST $1 ========================
echo ======================================================
./xas tests/test.$1.xas tests/test.$1.xo > /dev/null
if timeout 10 ./$2/$3 tests/test.$1.xo > tests/test.$1.s; then 
   echo Translated tests/test.$1.s 
elif [ $? -eq 124 ]; then
  echo TIMEOUT
  exit 1
else 
  echo Abnormal program termination: the program crashed
  echo Exit code $?
  exit 1
fi

echo Compiling runner
gcc -o runner runner.c regsdump.s tests/test.$1.s
if [ ! -x runner ]; then
  echo runner not created.
  exit 1
fi

echo Running runner
if timeout 10 ./runner > tests/test.$1.out; then 
  if diff -w tests/test.$1.out tests/test.$1.expected > /dev/null; then
    echo PASSED
  else
    echo FAILED
    echo ======
    echo ____Your_output____ __Expected_Output___
   #echo +++++++++++++++++++ ++++++++++++++++++++
    diff -y -W 80 tests/test.$1.out tests/test.$1.expected 
    echo =====================
    echo Your Output:
    echo ++++++++++++++++
    cat tests/test.$1.out
    echo =====================
    echo Expected Output:
    echo ++++++++++++++++
    cat tests/test.$1.expected
    exit 1
  fi
elif [ $? -eq 124 ]; then
  echo TIMEOUT
  exit 1
else 
  echo Abnormal program termination: runner crashed
  echo Exit code $?
  exit 1
fi
