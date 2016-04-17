#include "Fractal.h"
#include "Image.h"

#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <complex.h>
#include <math.h>


#define ANTIALIASING 2
#define MAX_ITER 25000

static long double escape;
static long double escape_squared;

static long double log_escape;
static long double log_two;

//static long* colors = NULL;
//static long used = 1;

int __attribute__ ((noinline)) Process_Fractal(struct Fractal *fractal) 
{
    char *f =(char *)&fractal;
    f = f + sizeof(struct Fractal);
    char *temp =(char *)&fractal;
    return f-temp;
}

int __attribute__ ((noinline)) process(int num)
{
    //char *buf = calloc(1,500);
    //int i = 0;
    //for (i=0; i<500; i++)
    //    buf[i] = '-';
	//sprintf(buf, "%d", num);
	//int k = 0;
	//for (i=0; i<500; i++) 
	 //   if (buf[i] != '-')
	//	k++;
	return sizeof(int);
}

int __attribute__ ((noinline)) locate_fractal_index(char *filename) 
{
    int i=0;
    while (filename[i] != '-') {
        i++;
    }
    int start_pos = i;
    while (filename[i] != '.') {
        i++;
    }
    int end_pos = i;
    return abs((end_pos/2) - start_pos + (end_pos * 24 - (start_pos%4)));
}

unsigned int __attribute__ ((noinline)) Prepare_Fractal(struct Fractal *fractal, char *filename)
{
//	int processed = Process_Fractal(fractal);
	int processed = sizeof(fractal);
    	//int fractal_index = locate_fractal_index(filename);
    	//int processed_fractal = process(fractal_index);
	//return processed + processed_fractal;
	return processed + sizeof(int);
}

struct Fractal *Fractal_Create(char *filename, int width, int height, long double complex position, long double zoom)
{
	struct Fractal *fractal = NULL;
	//fractal = calloc(1, Prepare_Fractal(fractal, filename));
	fractal = calloc(1, sizeof(struct Fractal));
	
	if (!fractal) {
		return NULL;
	}
	
	struct Image *image = image_create(width, height, filename);

	if (!image) {
		free(fractal);
		return NULL;
	}
	
	
	fractal->image = image;
	fractal->width = width;
	fractal->height = height;
	fractal->position = position;
	fractal->zoom = zoom;	
	fractal->line = 0;
	
	// Initialise static variables
	escape = 1000.0l;
	escape_squared = escape * escape;

	log_escape = logl(escape);
	log_two = logl(2.0l);
	
	return fractal;
}

void Fractal_Destroy(struct Fractal *fractal)
{
	//count the colors that are used in the fractal
	//if (used == 1) {
	//    colors = (long*)calloc(300, sizeof(long));
	//    for (int i = 0; i<300; i++) {
	 //       colors[i] = 0;
        //}
        //used = 0;
	//}
	
	//if (used == 0) {
	   // for (int i = 0; i<300; i++) {
	    //    colors[(uint8_t)fractal->image->map[i]]++;
	    //}
	//}
	
	image_close(fractal->image);		
	
	free(fractal);
}

uint8_t Wrap(unsigned int num)
{
	num %= 510;
    	
	if (num > 255) {
	    uint8_t ret = 254;
	    //for (unsigned int i=0; i<num-255; i++) {
	    //    ret--;
	    //}
	    ret -= (num - 255);
        ret += 1;
        return ret;
	}
	return num;
}

static int aquire_line(struct Fractal *fractal)
{
	unsigned int line = fractal->line;
	
	//if (used == 1) {
	//    colors = (long*)calloc(300, sizeof(long));
	//    for (int i = 0; i<300; i++) {
	//        colors[i] = 0;
        //	}
	//}
	
	if (line < fractal->height) {
		// Point to next horizontal line
		fractal->line++;
		
		// Print progress
		printf("\r%d / %d", fractal->line, fractal->height);
		fflush(stdout);
	}
	return line;
}

unsigned int iterate(long double complex num)
{
	//long double complex c = num;
	register long double x0 = creall(num);
	register long double y0 = cimagl(num);
	register long double x = x0;
	register long double y = y0;
	register long double temp = x;
					
	for (unsigned int i = 1; i < MAX_ITER; i++) {

		//num *= num;
		//num += c;

		//x = (x * x) - (y * y) + x0;
		//y = (2 * x * y) + y0;		
		//temp = x;

		temp = x ;
		x = (x * x) - (y * y) + x0;
		y = (2 * temp * y) + y0;
		

		register long double length = x * x + y * y;

		//long double length = creall(num) * creall(num) + cimagl(num) * cimagl(num);
		
		if (length >= escape_squared) {
			//double long log_zn = 0.5 * logl(x*x + y*y) / escape;
			//double long nu = logl( log_zn / log_escape ) / log_two;
			//double long log_zn = logl( sqrtl(x*x + y*y) ) / escape;
			//double long nu = logl( log_zn / log_escape ) / log_two;
			//long double moo = i + 1.0l - (logl(logl(cabs(num)) / log_escape) / log_two);

			long double moo = i + 1.0l - (log2(0.5 * log((x * x) + (y * y)) / log_escape));
			return logl(moo) * 175.0l;
		}
	}
	return 0.0l;
}

void *Fractal_Render(void *arg)
{
	struct Fractal *fractal = arg;
	uint8_t *array = fractal->image->array;
	
	long double ratio = fractal->height;
	ratio /= fractal->width;
	
	for (;;) {	
	    	
		unsigned int line = aquire_line(fractal);
		
		if (line >= fractal->height) {
			return NULL;
		}
	
		for (unsigned int x = 0; x < fractal->width; x++) {
		    
			long double num_real = ((x + 0.5l) / fractal->width - 0.5l) * 2.0l;
			long double num_imag = ((line + 0.5l) / fractal->height - 0.5l) * 2.0l * ratio;
			long double complex num = num_real + num_imag * I;
			num *= fractal->zoom;
			num += fractal->position;
			
			long double acc = 0.0l;
			
			for (unsigned int ay = 0; ay < ANTIALIASING; ay++) {
				for(unsigned int ax = 0; ax < ANTIALIASING; ax++) {
				    //if (used == 1) {
				     //   colors = (long*)calloc(300, sizeof(long));
				    //    for (int i = 0; i<300; i++) {
				     //      colors[i] = i;
                       			//}
                    		   // }
				long double offset_real = (((ax + 0.5l) / ANTIALIASING - 0.5l) * 2.0l / fractal->width);
				long double offset_imag = (((ay + 0.5l) / ANTIALIASING - 0.5l) * 2.0l / fractal->height) * ratio;
				
				long double complex offset = offset_real + offset_imag * I;
				offset *= fractal->zoom;
				num += offset;
				
				acc += iterate(num);
				}
			}
			
			acc /= ANTIALIASING * ANTIALIASING;
		
			array[(x + line * fractal->width) * 3 + 0] = Wrap(acc * 4.34532457l); 
			array[(x + line * fractal->width) * 3 + 1] = Wrap(acc * 2.93324292l);
			array[(x + line * fractal->width) * 3 + 2] = Wrap(acc * 1.2273444l);
		}
	}
}
