//Automatically tests fucntionality of compress.c
//Must be in the same directory as compress.c and a video named test.mp4

#include "compress.h"
#include <sys/stat.h>
#include <sys/time.h>

int qFlag = 0; //Flag for setting quiet output
int kFlag = 0; //Flag for keeping output file

//Test the execution of the simpleCompress function in compress.c
int testSimpleCompress() 
{
    //Setup variables for function call
    char * input = "./testdir/test.mp4";
    char * outputPath = "./testdir";
    char * outputName = "testOut";
    char * preset = "ultrafast";
    char * output = "./testdir/testOut.mp4";
    struct stat statbuf; //for use of stat to test input and output size
    stat(input, &statbuf);
    long inputSize = statbuf.st_size;
    //Execute compress
    simple(qFlag, input, outputPath, outputName, preset);
    if(access(output, F_OK) != 0) {
        printf("Test failed - Output not created.\n");
        return 0;
    }
    stat(output, &statbuf);
    long outputSize = statbuf.st_size;
    char * diff = (outputSize > inputSize) ? "larger" : "smaller";
    printf("\t-Output %liKB %s than input!\n", (outputSize - inputSize)/1000, diff);
    remove(output);
    puts("Test: output file deleted.");
    return 1;
}

int testCompress() {
    //Setup variables for function call
    char * input = "./testdir/test.mp4";
    char * outputPath = "./testdir";
    char * outputName = "testOut";
    char * preset = "ultrafast";
    char * output = "./testdir/testOut.mp4";
    struct stat statbuf; //for use of stat to test input and output size
    stat(input, &statbuf);
    long inputSize = statbuf.st_size;
    long targetSize = 1; //(long)((double)inputSize / 2000000.0);
    printf("inputbytes %li targetMB %li\n", inputSize, targetSize);
    //Execute compress
    twoPass(qFlag, input, outputPath, outputName, targetSize, preset);
    puts("Test: Calling compress");
    if(access(output, F_OK) != 0) {
        printf("Test failed - Output not created.\n");
        return 0;
    }
    stat(output, &statbuf);
    long outputSize = statbuf.st_size;
    printf("targetSize %li outputSize %li\n", targetSize, outputSize);
    printf("\t-Output %liKB smaller than target, %liKB smaller than input\n", (targetSize*1000000 - outputSize)/1000, (inputSize - outputSize)/1000);
    if(!kFlag) {
        remove(output);
        puts("Test: output file deleted.");
    }
    return 1;
}

int main(int argc, char ** argv)
{
    //Handle option flags with getopt
	int flag;
	while ((flag = getopt(argc, argv, "qk")) != -1) {
		switch (flag) {
		case 'k':
			kFlag = 1;
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
	if(argc - optind != 0) {
		fprintf(stderr, "Error: test program expected 0 non-opt arguments, but instead received %i\n", argc-optind);
		exit(EXIT_FAILURE);
	}
    //Create time benchmarking structs
    struct timeval mainStart;
    struct timeval testStart;
    struct timeval end;
    gettimeofday(&mainStart, NULL);
    //Compile files to be tested with makefile
    printf("Make-ing.\n");
    if(system("make exe") == -1) { //catch makefile exiting w/ error
        printf("\tTest encountered error in compilation - exiting.\n");
        exit(EXIT_FAILURE);
    }
    //Test simpleCompress
    printf("Testing simpleCompress...\n");
    gettimeofday(&testStart, NULL);
    char * result = testSimpleCompress() ? "PASSED" : "FAILED";
    gettimeofday(&end, NULL);
    double duration = ((double)end.tv_sec + (double)end.tv_usec/1000000) - ((double)testStart.tv_sec + (double)testStart.tv_usec/1000000); 
    printf("\tsimpleCompress test %s in %.4fs.\n", result, duration);
    //Test normal compress
    printf("Testing 2-pass compress...\n");
    gettimeofday(&testStart, NULL);
    result = testCompress() ? "PASSED" : "FAILED";
    gettimeofday(&end, NULL);
    duration = ((double)end.tv_sec + (double)end.tv_usec/1000000) - ((double)testStart.tv_sec + (double)testStart.tv_usec/1000000); 
    printf("\t2-pass compress test %s in %.4fs.\n", result, duration);
    gettimeofday(&end, NULL);
    duration = ((double)end.tv_sec + (double)end.tv_usec/1000000) - ((double)mainStart.tv_sec + (double)mainStart.tv_usec/1000000); 
    printf("All tests done in %.4fs.\n", duration);
    return 1;
}