//Functions to get duration of a video file

#include "compress.h"

double video_duration_seconds(char* filename) {
	for(int i = 0; i < strlen(filename); i++) {
		if(filename[i] == ';') {
			printf("Invalid filename input (;)\n");
			exit(1);
		}
	}
	int probefile = open("./probe", O_RDWR | O_CREAT, S_IRWXU | S_IROTH);
	char command[256];
	sprintf(command, "ffprobe -i %s -show_entries format=duration -loglevel panic -print_format csv > probe", filename); 
	if(system(command) < 0) {
		printf("Error in probe, status\n");
		return -1;
	}
	fsync(probefile);
	char result[101] = {0};
	int bytesRead = 0;
	while(bytesRead < 100) {
		int count = read(probefile, &result[bytesRead], 100-bytesRead);
		if(count == 0) break;
		bytesRead += count;
	}
	close(probefile);
	return strtod(&result[7], NULL);
}