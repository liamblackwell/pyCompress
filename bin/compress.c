//Helper program for use in tandem with pyCompress.py to quickly access
// and manipulate video file details and ffmpeg calls.

#include "compress.h"

//Calls ffmpeg to compress with preset 'preset' without providing target size, just requires one call/pass
void simpleCompress(char * input, char * outputPath, char * outputName, char * preset) 
{
    clock_t start = clock();
	char command[256];
	sprintf(command, "ffmpeg -y -nostats -loglevel warning -i %s -c:v libx264 -preset %s -crf 22 -c:a copy %s/%s.mp4", input, preset, outputPath, outputName);
	printf("Compressing video...\n");
	//Create a child process to run ffmpeg
	if(fork() == 0) {
		char *args[] = {"ffmpeg", command, NULL};
		execv("ffmpeg", args);
		exit(0);
	}
	//Have parent process wait for it to finish
	else wait(NULL);
	//system(command);
	clock_t end = clock();
	printf("Video compressed in %.4fs, hopefully :)\n", (double)(end-start)/CLOCKS_PER_SEC);
}

//Calls ffmpeg to compress with preset 'preset' to target size (MB), using two passes
void compress(char * input, char * outputPath, char * outputName, char * preset, double targetSize) {

}

int main(int argc, char **argv)
{
	//Handle option flags with getopt
	int tFlag = 0;
	double targetSize = 0.0;
	int flag;

	while ((flag = getopt(argc, argv, "abc:")) != -1) {
		switch (flag) {
		case 't':
			tFlag = 1;
			targetSize = atoi(optarg);
			break;
		case '?':
			if(optopt == 't') fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if(isprint(optopt)) fprintf (stderr, "Unknown option '-%c'.\n", optopt);
			else fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			abort();
		}
	}

	//Validate arg count
	if(argc - optind != 4) {
		fprintf(stderr, "Error: program expected %i arguments, but instead received %i\n", argc-optind, argc-1);
		exit(EXIT_FAILURE);
	}

	//Assign names to non-opt args
	char * input = argv[optind];
	char * outputPath = argv[optind+1];
	char * outputName = argv[optind+2];
	char * preset = argv[optind+3];
	printf("compress args: %s %s %s %s\n", input, outputPath, outputName, preset);

	//Call appropriate function
	if(!tFlag) simpleCompress(input, outputPath, outputName, preset);
	else compress(input, outputPath, outputName, preset, targetSize);

	return 0;
}