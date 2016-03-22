#!/bin/bash
#
# Build and run tests

GENBIN="src/test/zzrndgen"
HBTESTBIN="src/test/hbtest"
BPTESTBIN="src/test/bptest"
SIMULBIN="src/test/eopsisimul"
CHPARBIN="src/test/cherpars"

N="1000000"
GENP="32"
P="113"
PROBEXP="-40"
FILE="zz.dat"
LENGTH=0
MAXLOAD=0

POWS2="1"

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

fill_pow2() {
  i=1
  p=1
  while [ $i -le $1 ]
  do
    p=$((2*$p))
    POWS2="$POWS2 $p"
    i=$(($i + 1))
  done
}

pow2() {
  j=$(($1 + 1))
  echo $POWS2 | cut -d ' ' -f$j
}

pows2() {
  j=$(($1 + 1))
  k=$(($2 + 1))
  echo $POWS2 | cut -d ' ' -f$j-$k
}

run_simulations() {
  PREFIX="$3"
  etot=$(($(echo $1 | wc -w) * $(echo $2 | wc -w)))
  e=0
  for l in $1
  do
    for n in $2
    do
      e=$(($e + 1))
    
      k=$($CHPARBIN -q -l $l -n $n -p $PROBEXP)
      LOGFILE="${PREFIX}k${k}_l${l}_n${n}_b${GENP}_p${P}_errs.log"
      OUTFILE="${PREFIX}k${k}_l${l}_n${n}_b${GENP}_p${P}.dat"
      pn=$(pow2 $n)
      
      printf " ------ Experiment \"$e/$etot\" launched.\n"
      echo $SIMULBIN -k $k -l $l -n $pn -r $(($pn / 4)) -b $GENP -p $P
      printf "Please, be patient..."
      $SIMULBIN -k $k -l $l -n $pn -r $(($pn / 4)) -b $GENP -p $P > "$OUTFILE" 2> "$LOGFILE"
      printf " completed.\n"
      printf " ------ See output file: $OUTFILE\n"
      printf " ------ See log file: $LOGFILE\n"
      printf "\n\n"
    done
  done
}

build
cd build

# Generation
# generate_rnd_zz

# HB test
# $HBTESTBIN -a MH3    -k 16536 -l 128 -i $FILE
# $HBTESTBIN -a SHA1   -k 16536 -l 128 -i $FILE
# $HBTESTBIN -a SHA256 -k 16536 -l 128 -i $FILE

# BP test
# P="604462909807314587353111" generate_rnd_zz
# $BPTESTBIN -a MH3    -k 16536 -l 128 -p $P -i $FILE

# Simulations
fill_pow2 20
#run_simulations "20 50 100 200" "17" "$(date +"%s")_experiments-size-vs-time_"
#run_simulations "120 140 160 180" "17" "$(date +"%s")_experiments-size-vs-time_"
# run_simulations "30 300 400 500" "17" "$(date +"%s")_experiments-size-vs-time_"
run_simulations "100 120" "10 13 15 17 20" "$(date +"%s")_experiments-size-vs-time_"
# run_simulations "20 50 100 110 119 120 121 125 130 140 160 180 200 300 400 500" "10" "$(date +"%s")_experiments-size-vs-time_"




