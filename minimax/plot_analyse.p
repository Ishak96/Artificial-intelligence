set key left

set xrange [0:4.2]
set yrange [-1000:9500]
set tics font ", 25"
set key font ",20"

set xlabel "Depth" font "Helvetica, 30" offset 0,-1,0
set ylabel "The number of nodes (kilo nodes)"  font "Helvetica, 30" offset -6,0,0

set ytics (0,1000,2000,3000,4000,5000,6000,7000,8000,9000)
set xtics (0,1,2,3,4)

set lmargin 18

set title "The curve of the number of nodes to explore according to the depth of analysis" font "Helvetica, 30"
set terminal png size 3000,1500;
set output 'analyse_experimentale.png'

plot 'data_minimax_analyse.dat' using 1:2 title "Minimax" w l lw 4, \
	 'data_minimax_alphabeta_analyse_s.dat' using 1:2 title "Minimax with alpha/beta plugin" w l lw 4, \
	 'data_minimax_alphabeta_analyse_a.dat' using 1:2 title "Minimax with alpha/beta plugin with amelioration" w l lw 4, \
	 'data_negamax_analyse.dat' using 1:2 title "Negamax" w l lw 4, \
	 'data_negamax_alphabeta_analyse_s.dat' using 1:2 title "Negamax with alpha/beta plugin" w l lw 4, \
	 'data_negamax_alphabeta_analyse_a.dat' using 1:2 title "Negamax with alpha/beta plugin with amelioration" w l lw 4