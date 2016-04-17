#include "Image.h"

#include <sys/mman.h>
#include <unistd.h>
#include <sys/fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

static long artifacts = 0;

static void readHeader(FILE *fp, int *width, int *height, const char *filename)
{
	char ch;
	int  maxval;

	if (fscanf(fp, "P%c\n", &ch) != 1 || ch != '6') {
		fprintf(stderr, "File %s is not in ppm raw format!\n", filename);
		exit(1);
	}

	ch = getc(fp);
	while (ch == '#')
	{
		do {
		ch = getc(fp);
		} while (ch != '\n');
		ch = getc(fp);
	}

	if (!isdigit(ch)) {
		fprintf(stderr, "Cannot read header information from %s!\n", filename);
		exit(1);
	}

	ungetc(ch, fp);

	fscanf(fp, "%d %d\n%d\n", width, height, &maxval);

	if (maxval != 255) {
		fprintf(stderr, "image %s is not true-color (24 bit)", filename);
		exit(1);
	}

	if (*width < 1 || *width > 4000 || *height < 1 || *height > 4000) {
		fprintf(stderr, "image %s contains unreasonable dimensions!\n", filename);
		exit(1);
	}
}

struct Image *image_load(const char *filename)
{
	struct Image *image = malloc(sizeof(*image));
	int width, height;
	unsigned num;

	if (!image) {
		return NULL;
	}
	FILE *fp = fopen(filename, "rb");

	if (!fp) {
		fprintf(stderr, "Error loading file %s\n", filename);
		free(image);
		exit(1);
	}
	readHeader(fp, &width, &height, filename);
	image->fd = 0;
	image->height = height;
	image->width  = width;
	image->array_size = width * height * 3;
	image->array = malloc(image->array_size);

	if (!image->array) {
		fprintf(stderr, "Memory error!\n");
		exit(1);
	}
	num = fread(image->array, 1, image->array_size, fp);
	if (num != image->array_size) {
		fprintf(stderr, "Cannot read image data from %s\n", filename);
		exit(1);
	}
	fclose(fp);

	return image;
}

struct Image *image_create(size_t width, size_t height, const char *filename)
{
	struct Image *image = calloc(1,sizeof(*image));
	
	if (!image) {
		return NULL;
	}
	
	// Open file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	
	if (fd == -1) {
		perror("Error opening file");
		free(image);	
		return NULL;
	}
	
	// Create PPM image header
	char *header;
	int header_length = asprintf(&header, "P6\n%d %d\n255\n",(int) width,(int) height);

	if (header_length == -1) {
		perror("Failed to allocate header");
		close(fd);
		free(image);
		return NULL;
	}
	
	write(fd, header, header_length);
	
	//for (int i=0; i<header_length; i++) {
	  //  header[i] = 0;
	//}
	//header = (char *) calloc (header_length, sizeof(char));
	free(header);
	header = NULL;
	
	// Expand file to the full image length
	size_t array_size = width * height * 3;
	size_t file_size = header_length + array_size;
	
	// Map file to memory
	char *map = calloc(file_size , sizeof(char));
	
	if (map == NULL) {
		perror("Error mapping file");
		close(fd);
		free(image);
		return NULL;
	}
	
	image->width = width;
	image->height = height;
	image->fd = fd;
	image->array_size = image->width * image->height * 3;
	image->file_size = header_length + array_size;
	image->map = map;
	image->array =(uint8_t *) map + header_length;
	
	//unsigned int i=0;
	  //  while (i != image->width * image->height * 3) {
		//image->array[i] = 0;
		//i++;
	  //  }
	
	return image;
}

void image_close(struct Image *image)
{
    // Before saving, check the image for 0 values
    //for (unsigned int i=0; i<image->file_size; i++) {
    //}
    
	for (unsigned int i=0; i<image->array_size; i++) {
	    if (image->map[i] == 0) 
	        artifacts++;
	    //write(image->fd, image->array + i, sizeof(char));	    
	}
    	write(image->fd, image->array, image->array_size * sizeof(char));
	
	close(image->fd);
	free(image);
}
