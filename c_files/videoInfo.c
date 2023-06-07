//Functions to get duration of an MP4 video file
//Taken from https://geekthis.net/post/c-get-mp4-duration/

#include "compress.h"

char* videoinfo_mem(char* haystack, unsigned int sizehaystack, char* needle, unsigned int sizeneedle) {
	int i = 0;
	int end = sizehaystack - sizeneedle;
	for(i = 0; i < end; ++i) {
		if(memcmp(haystack+i,needle,sizeneedle) == 0) {
			return haystack+i;
		}
	}
	return NULL;
}

void* videoinfo_find(char* filename, void* find, int size, int resultSize) {
	FILE* fp = NULL;
	char* buffer = NULL;
	char* result = NULL;
	char* pos = NULL;
	unsigned int bufferSize = 2048;
	double filesize = 0;
	double split = 16;
	double splitsize = 0;
	double start = 0;
	double end = 0;
	double i = 0;
	unsigned int read = 0;

	if(resultSize > bufferSize) {
		resultSize = bufferSize;
	}

	buffer = malloc(bufferSize);
	if(buffer == NULL) {
		return NULL;
	}

	fp = fopen(filename,"rb");
	if(fp == NULL) {
		free(buffer);
		return NULL;
	}

	fseek(fp,0,SEEK_END);
	filesize = ftell(fp);
	rewind(fp);

	split = (long)(filesize / 100000);
	splitsize = (long)(filesize/split);

	for(i = split-1; i >= 0; --i) {
		start = (i*splitsize);
		end = start + splitsize;
		fseek(fp,start,SEEK_SET);

		while((read = fread(buffer,1,bufferSize,fp)) != 0) {
			if((pos = videoinfo_mem(buffer,bufferSize,find,size)) != NULL) {
				result = malloc(resultSize);
				memcpy(result,pos,resultSize);
				i = -1;
				break;
			}

			if(read != bufferSize || ftell(fp) >= end) {
				break; // go onto next split
			}
		}

	}

	fclose(fp);
	free(buffer);
	return result;
}

unsigned long videoinfo_flip(unsigned long val) {
	unsigned long new = 0;
	new += (val & 0x000000FF) << 24;
	new += (val & 0xFF000000) >> 24;
	new += (val & 0x0000FF00) << 8;
	new += (val & 0x00FF0000) >> 8;

   return new;
}

unsigned long videoinfo_duration(char* filename) {
	unsigned long duration = 0;
	char version = 0;
	void* data = NULL;
	char* pos = NULL;
	unsigned long timescale = 0;
	unsigned long timeunits = 0;
	int bytesize = 4;

	data = videoinfo_find(filename,"mvhd",4,64);
	if(data == NULL) {
		goto clean;
	}

	pos = (char*)data;
	pos += 4; // skip mvhd

	version = *pos++;
	pos+=3; //skip flags

	if(version == 1) {
		bytesize = 8;
	}else{
		bytesize = 4;
	}

	pos += bytesize; // skip created date
	pos += bytesize; // skip modified date

	memcpy(&timescale,pos,4);
	memcpy(&timeunits,pos+4,bytesize);

	timescale = videoinfo_flip(timescale);
	timeunits = videoinfo_flip(timeunits);

	if(timescale > 0 && timeunits > 0) {
		duration = timeunits/timescale;
	}

	clean:
	free(data);
	return duration;
}