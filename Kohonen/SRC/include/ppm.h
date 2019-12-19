#ifndef PPM_H
#define PPM_H

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define BIT_COLOR 255

typedef struct {
     unsigned char r,g,b;
}Pixel;

typedef struct {
     int x, y;
     Pixel *data;
}Image;


void exit_on_error(const char *name_of_file, const char* name_of_function, int numero_of_line, const char * message);

#define EXIT_ON_ERROR(...) exit_on_error(__FILE__, __func__, __LINE__,__VA_ARGS__)

Image * readPPM(const char * filename);
void writePPM(const char *filename, Image *img);

#endif