set title  "RNN Fitness over Time"
set xlabel "Generation"
set ylabel "Fitness"

set key center right

plot                                                  \
   "data.csv" using 1:3 with linespoint title "MaxF", \
   "data.csv" using 1:2 with linespoint title "AvgF"
pause -1

