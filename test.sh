#!/bin/bash
make
for i in {7..18}
do
./randmst $((2**i)) 5 2
done