//Helper program for use in tandem with pyCompress.py to quickly access
// and manipulate video file details and ffmpeg calls.

#include "compress.h"

//Define flags - redundant for use as library
int tFlag = 0;
double tSize = 0.0;
int qFlag = 0;

//Calls ffmpeg to compress with preset 'preset' without providing target size, just requires one call/pass
int simple(char * input, char * outputPath, char * outputName, char * preset) 
{
	char command[256];
	if(qFlag) sprintf(command, "ffmpeg -y -nostats -loglevel warning -i %s -c:v libx264\
	 -preset %s -crf 22 -c:a copy %s/%s.mp4", input, preset, outputPath, outputName);
	else sprintf(command, "ffmpeg -y -i %s -c:v libx264 -preset %s -crf 22 -c:a copy %s/%s.mp4", input, preset, outputPath, outputName);
	
	printf("Compressing video...\n");
	if(system(command) < 0) {
		printf("Error in simplecompress\n");
		return -1;
	}
	return 1;
}

//Calls ffmpeg to compress with preset 'preset' to target size (MB), using two passes
int twoPass(char * input, char * outputPath, char * outputName, double targetSize, char * preset) 
{
	//get video duration
	unsigned long videoLength = videoinfo_duration(input); 
	//TODO maybe
	long audioBitrate = 128; 
	//minimum size of video
	double minSize = (audioBitrate * videoLength) / 8000;

	if(targetSize < minSize) {
		printf("Target size %.2fMB below minimum size %.2fMB\n", targetSize, minSize);
		return -1;
	}

	//Get output video bitrate by converting size given in MB to kilobits, divide by length, and account for audio bitrate
	long outputBitrate = (targetSize * 8000 / videoLength) - audioBitrate;

	char firstPass[256];
	char secondPass[256];
	char * quiet = (qFlag ? "-nostats -loglevel warning" : " ");
	sprintf(firstPass, "ffmpeg -y %s -i %s -max_muxing_queue_size 9999 -c:v libx264 \
		-preset %s -b:v %lik -pass 1 -vsync cfr -f null /dev/null && \\", quiet, input, preset, outputBitrate);
	sprintf(secondPass, "ffmpeg %s -i %s -max_muxing_queue_size 9999 -c:v libx264 \
		-preset %s -b:v %lik -pass 2 -c:a aac -b:a %lik %s/%s.mp4", quiet, input, preset, outputBitrate, audioBitrate, outputPath, outputName);

	printf("Compressing video...\n");
	printf("\tRunning %s\n", firstPass);
	if(system(firstPass) < 0) {
		printf("Error in first pass\n");
		return -1;
	}
	printf("\tRunning %s\n", secondPass);
	if(system(secondPass) < 0) {
		printf("Error in second pass\n");
		return -1;
	}
	return 1;
}

//Main function redundant for library use
int main(int argc, char **argv)
{
	//Handle option flags with getopt
	int flag;

	while ((flag = getopt(argc, argv, "qt:")) != -1) {
		switch (flag) {
		case 't':
			tFlag = 1;
			tSize = atoi(optarg);
			break;
		case 'q':
			qFlag = 1;
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
	if(!tFlag) simple(input, outputPath, outputName, preset);
	else twoPass(input, outputPath, outputName, tSize, preset);

	return 0;
}