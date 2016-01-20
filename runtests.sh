#!/bin/bash
#
# Build and run tests

GENBIN="src/test/zzrndgen"
HBTESTBIN="src/test/hbtest"
BPTESTBIN="src/test/bptest"

N="1000000"
GENP="605462909807314587353179"
P="335688069665150755269371147819668813122841983204197482918576337"
FILE="zz.dat"
LENGTH=0
MAXLOAD=0

set -e

build() {
  mkdir -p build
  cd build
  cmake ../
  make
  cd ..
}

operation() {
  echo "$*" | bc
}

generate_rnd_zz() {
  $GENBIN -n $N -p $GENP -o $FILE 
}

guess_max_load() {
  operation "$1 * 4 / 3"
}

# Run tests (some tests)
run_tests() {
  set +e
  for l in $1
  do
    LENGTH=$l
    MAXLOAD=$(guess_max_load $(operation $N/$l))
    for halg in MH3
    do
      $HBTESTBIN -a $halg -k $LENGTH -l $MAXLOAD -i $FILE
    done
  done
  set -e
}

build
cd build

# HB test
# generate_rnd_zz
# $HBTESTBIN -a MH3    -k 16536 -l 128 -i $FILE
# $HBTESTBIN -a SHA1   -k 16536 -l 128 -i $FILE
# $HBTESTBIN -a SHA256 -k 16536 -l 128 -i $FILE

# BP test
# P="604462909807314587353111" generate_rnd_zz
$BPTESTBIN -a MH3    -k 16536 -l 128 -p 210 -i $FILE
