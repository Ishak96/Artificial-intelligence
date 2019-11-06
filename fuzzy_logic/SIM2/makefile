#File:     Makefile for Khepera Simulator (example 1)
#Author:   Olivier MICHEL <om@alto.unice.fr>
#Date:     Tue Feb 13 11:29:50 MET 1996

#   Copyright (c) 1995, 1996
#   Olivier MICHEL
#   MAGE team, i3S laboratory,
#   CNRS, University of Nice - Sophia Antipolis

#   Permission is hereby granted to copy all or any part of
#   this program for free distribution.   The author's name
#   and this copyright notice must be included in any copy.
#   Commercial use is forbidden.


LIBS   = -L/usr/lib -lX11 -lm

CC = gcc $(LIBS)

sim:
	gcc -L/usr/lib -lX11 -lm SRC/robot.c SRC/world.c SRC/graphics.c SRC/khep_serial.c USER/user.c SRC/sim.c -o sim -lX11 -lm


clean:
	rm -f sim OBJ/*.o

