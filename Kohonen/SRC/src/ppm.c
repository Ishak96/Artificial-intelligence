#include "ppm.h"

void exit_on_error(const char *name_of_file, const char* name_of_function, int numero_of_line, const char * message)
{
	fprintf(stderr,RED"%s \nIn file %s function=%s line=%d\n"RESET,message,name_of_file,name_of_function,numero_of_line);
	fflush(stderr);
	raise(SIGABRT);
	exit(EXIT_FAILURE);
}

Image * readPPM(const char * filename)
{
	char buffer[16];
	char c ;
	int img_color_bits;
	FILE * img_input = NULL ;
	Image * img = NULL;

	img_input = fopen(filename, "rb");
	if (img_input == NULL ) EXIT_ON_ERROR("Failed to open img");

	img = (Image *) malloc(sizeof(Image));
	if (img == NULL) EXIT_ON_ERROR("Failed to alloc img");

	if (!fgets(buffer, sizeof(buffer), img_input)) EXIT_ON_ERROR("Failed to read img header");
	if (buffer[0] != 'P' || buffer[1] != '6') EXIT_ON_ERROR("Invalid image format (must be 'P6')");

    //check for comments and move pointer after
    c = getc(img_input);
    while (c == '#')
    {
	    while (getc(img_input) != '\n')
	         c = getc(img_input);
    }
    ungetc(c, img_input);

    if (fscanf(img_input, "%d %d", &img->x, &img->y) != 2) EXIT_ON_ERROR("Invalid img size"); // size
    if (fscanf(img_input, "%d", &img_color_bits) != 1) EXIT_ON_ERROR("Invalid rgb parameter");
    if (img_color_bits!= BIT_COLOR) EXIT_ON_ERROR("img is not 8-bits"); // 8 bit img ?

    img->data = (Pixel*)malloc(img->x * img->y * sizeof(Pixel));
    if (img->data == NULL) EXIT_ON_ERROR ("Failed to allocate memory");

    while (fgetc(img_input) != '\n');
    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, img_input) != img->y) EXIT_ON_ERROR("Error while loading image");
    fclose(img_input);
    return img;
}

void writePPM(const char *filename, Image *img)
{
    FILE * output_img;
    output_img = fopen(filename, "wb");
    if (output_img == NULL) EXIT_ON_ERROR("Failed to open file");
    fprintf(output_img, "P6\n"); // type
    fprintf(output_img, "%d %d\n",img->x,img->y); // size
    fprintf(output_img, "%d\n",BIT_COLOR); // 8bit img
    fwrite(img->data, 3 * img->x, img->y, output_img); // data
    fclose(output_img);
}

/*int main()
{
	Image * img = NULL;
    img = readPPM("45.ppm");
    writePPM("sortie.ppm",img);
    if (img != NULL)
    {
		free(img->data);
        img->data = NULL;
        free(img);
        img = NULL ;
    }
}*/