#include <cloud.h>

int main(int argc, char** argv) {

	if(argc < 11){
		fprintf(stderr, "main: invalid argument!\n");
		printf("usage: %s [a] [b] [c] [e1] [e2] [origin x] [origin y] [origin z] [r0] [r1]..\n", argv[0]);
		printf("give r0 = 0 and r1 = 1 to gener a normal superquadrics\n");
		return -1;
	}

	float a_t = atof(argv[1]);
	float b_t = atof(argv[2]);
	float c_t = atof(argv[3]);
	float e1_t = atof(argv[4]);
	float e2_t = atof(argv[5]);
	int xo = atoi(argv[6]);
	int yo = atoi(argv[7]);
	int zo = atoi(argv[8]);
	double r0 = atof(argv[9]);
	double r1 = atof(argv[10]);

	int size;
	float** cloud = generate_cloud_point(70, 70, a_t, b_t, c_t, e1_t, e2_t, &size, xo, yo, zo, r0, r1);

	float* init_param = initial_parameters(cloud, size);

	printf("e1 = %f, e2 = %f\n", init_param[0], init_param[1]);
	printf("tx = %f, ty = %f, tz = %f\n", init_param[2], init_param[3], init_param[4]);
	printf("angle1 = %f, angle2 = %f, angle3 = %f\n", init_param[5], init_param[6], init_param[7]);
	printf("a = %f, b = %f, c = %f\n", init_param[8], init_param[9], init_param[10]);

	float* init_size = get_size_parameters(cloud, size, init_param[2], init_param[3], init_param[4]);
	printf("-------------------------------------------------\n");
	printf("a = %f, b = %f, c = %f\n", init_size[0], init_size[1], init_size[2]);
	
	return 0;
}
