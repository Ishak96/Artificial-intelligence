set xrange [0:250]
set yrange [0:0.2]
set tics font ", 25"
set key font ",20"

set xlabel "Iteration" font "Helvetica, 30" offset 0,-1,0
set ylabel "The error rate"  font "Helvetica, 30" offset -6,0,0

set ytics 0.01
set xtics 10

set lmargin 18

set title "Error rate graph during learning" font "Helvetica, 30"
set terminal png size 3000,1500;
set output 'analyse_error_A_first.png'

plot 'A_first/data_A.dat' using 1:2 title "Curve of A learning error" w l lw 4, \
	 'A_first/data_C.dat' using 1:2 title "Curve of C learning error" w l lw 4