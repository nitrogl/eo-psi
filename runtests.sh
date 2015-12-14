#!/bin/bash
#
# Build and run tests

GENBIN="src/zzrndgen"
TESTBIN="src/hbtest"

N="1000000"
P="924319124044731127256386643194362517987965988436268856863172801"
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
  $GENBIN -n $N -p $P -o $FILE 
}

guess_max_load() {
  operation "$1 * 4 / 3"
}

# Run tests
run_tests() {
  set +e
  for l in $1
  do
    LENGTH=$l
    MAXLOAD=$(guess_max_load $(operation $N/$l))
    for halg in MH3
    do
      $TESTBIN -a $halg -k $LENGTH -l $MAXLOAD -i $FILE
    done
  done
  set -e
}

build
cd build
# generate_rnd_zz

run_tests "
  $(operation $N/256)
  $(operation $N/1024)
  $(operation $N/16384)
  $(operation $N/65536)
  "