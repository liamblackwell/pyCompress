//Helper program for use in tandem with pyCompress.py to quickly access
// and manipulate video file details and ffmpeg calls.

#include "compress.h"

//Calls ffmpeg to compress with preset 'preset' without providing target size, just requires one call/pass
int simple(int quiet, char * input, char * outputPath, char * outputName, char * preset) 
{
	char command[256];
	if(quiet) sprintf(command, "ffmpeg -y -nostats -loglevel warning -i %s -c:v libx264\
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
int twoPass(int quiet, char * input, char * outputPath, char * outputName, double targetSize, char * preset) 
{
	//get video duration in seconds
	unsigned long videoLength = video_duration_seconds(input); 
	//TODO maybe
	long audioBitrate = 128; 
	//minimum size of video in megabytes
	double minSize = (audioBitrate * videoLength) / 8000;

	if(targetSize < minSize) {
		printf("Target size %.2fMB below minimum size %.2fMB\n", targetSize, minSize);
		return -1;
	}

	//Get output video bitrate by converting size given in MB to kilobits, divide by length, and account for audio bitrate
	printf("outputBitrate = (%.2f * 8000 / %lu) - %li", targetSize, videoLength, audioBitrate);
	long outputBitrate = (targetSize * 8000 / videoLength) - audioBitrate;

	char firstPass[1024];
	char secondPass[1024];
	char * quietOpts = (quiet ? "-nostats -loglevel warning" : " ");
	sprintf(firstPass, "ffmpeg -y %s -i %s -max_muxing_queue_size 9999 -c:v libx264 \
		-preset %s -b:v %lik -pass 1 -vsync cfr -f null /dev/null && \\", quietOpts, input, preset, outputBitrate);
	sprintf(secondPass, "ffmpeg %s -i %s -max_muxing_queue_size 9999 -c:v libx264 \
		-preset %s -b:v %lik -pass 2 -c:a aac -b:a %lik %s/%s.mp4", quietOpts, input, preset, outputBitrate, audioBitrate, outputPath, outputName);
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