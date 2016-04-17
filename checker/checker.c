#include "../Image.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	int i, num_failed = 0;
	char delta, delta_b, delta_g, delta_r;
	double perc_failed;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <reference image> <target image>\n", argv[0]);
		return 1;
	}
	struct Image *ref_img = image_load(argv[1]);
	struct Image *tst_img = image_load(argv[2]);
	if (ref_img->width != tst_img->width) {
		fprintf(stderr, "Images have different width: %ld vs %ld\n",
				ref_img->width, tst_img->width);
		return 1;
	}
	if (ref_img->height != tst_img->height) {
		fprintf(stderr, "Images have different height: %ld vs %ld\n",
				ref_img->height, tst_img->height);
		return 1;
	}
	for (i = 0; i < ref_img->array_size; i += 3) {
		delta_r = ref_img->array[i] - tst_img->array[i];
		delta_g = ref_img->array[i+1] - tst_img->array[i+1];
		delta_b = ref_img->array[i+2] - tst_img->array[i+2];
		delta = delta_r + delta_g + delta_b;
		if (delta < 0)
			delta = -delta;
		if (delta > 3)
			num_failed++;
	}
	perc_failed = ((float) num_failed / (ref_img->width * ref_img->height)) * 100;
	fprintf(stdout, "Number of incorrect pixels: %d\n", num_failed);
	fprintf(stdout, "Percentage of incorrect pixels: %lf%%\n", perc_failed);
	if (perc_failed > 5) {
		fprintf(stdout, "You have FAILED the test!\n");
        return 1;
    } else {
		fprintf(stdout, "You have PASSED the test!\n");
        return 0;
    }
	return 0;
}
