set key autotitle columnhead

set xtics axis
set ytics axis
set border 0
set grid back ls 12

set xrange [1:2]
set yrange [1000:190000]
set tics font ", 25"
set key font ",20"

set xtics 1

set lmargin 12

set xlabel "Size of depth" font "Helvetica, 30" offset 0,-1,0
set ylabel "The number of nodes"  font "Helvetica, 30" offset -2,0,0

set title "The curve of the number of nodes to explore according to the depth of analysis" font "Helvetica, 30"
set terminal png size 3000,1500;
set output 'analyse_experimentale.png'

plot 'data_minimax_analyse.dat' using 1:2 title "Minimax" w l lw 4, \
	 'data_minimax_alphabeta_analyse_a.dat' using 1:2 title "Minimax with alpha/beta plugin" w l lw 4, \
	 'data_minimax_alphabeta_analyse_s.dat' using 1:2 title "Minimax with alpha/beta plugin with amelioration" w l lw 4, \
	 'data_negamax_analyse.dat' using 1:2 title "Negamax" w l lw 4, \
	 'data_negamax_alphabeta_analyse_a.dat' using 1:2 title "Negamax with alpha/beta plugin" w l lw 4, \
	 'data_negamax_alphabeta_analyse_s.dat' using 1:2 title "Negamax with alpha/beta plugin with amelioration" w l lw 4