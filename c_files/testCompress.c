//Automatically tests fucntionality of compress.c
//Must be in the same directory as compress.c and a video named test.mp4

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>

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
    char command[256];
    sprintf(command, "./compress %s %s %s %s", input, outputPath, outputName, preset);
    puts("Test: Calling ./compress");
    if(system(command) == -1) {
        puts("Test: Error in system call");
        return 0;
    }
    puts("Test: System call done");

    if(access(output, F_OK) != 0) {
        printf("Test failed - Output not created.\n");
        return 0;
    }
    stat(output, &statbuf);
    long outputSize = statbuf.st_size;
    char * result = (outputSize > inputSize) ? "larger" : "smaller";
    printf("\t-Output %liKB %s than input!\n", (outputSize - inputSize)/1000, result);
    remove(output);
    puts("Test: output file deleted.");
    return 1;
}

//TODO
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
    long targetSize = inputSize / 2;

    //Execute compress
    char command[256];
    sprintf(command, "./compress -t %li %s %s %s %s", targetSize, input, outputPath, outputName, preset);
    puts("Test: Calling ./compress");
    if(system(command) == -1) {
        puts("Test: Error in system call");
        return 0;
    }
    puts("Test: System call done");

    if(access(output, F_OK) != 0) {
        printf("Test failed - Output not created.\n");
        return 0;
    }

    stat(output, &statbuf);
    long outputSize = statbuf.st_size;
    printf("\t-Output %liKB smaller than target\n", (targetSize - outputSize)/1000);
    remove(output);
    puts("Test: output file deleted.");
    return 1;
}

int main()
{
    //Create time benchmarking structs
    struct timeval mainStart;
    struct timeval testStart;
    struct timeval end;
    gettimeofday(&mainStart, NULL);

    //Compile files to be tested with makefile
    printf("Make-ing.\n");
    if(system("make") == -1) { //catch makefile exiting w/ error
        printf("\tTest encountered error in compilation - exiting.\n");
        exit(EXIT_FAILURE);
    }

    //Test simpleCompress
    printf("Testing simpleCompress...\n");
    gettimeofday(&testStart, NULL);
    char * result = testSimpleCompress() ? "passed" : "failed";
    gettimeofday(&end, NULL);
    double duration = ((double)end.tv_sec + (double)end.tv_usec/1000000) - ((double)testStart.tv_sec + (double)testStart.tv_usec/1000000); 
    printf("\tsimpleCompress test %s in %.4fs.\n", result, duration);

    //Test normal compress
    printf("Testing 2-pass compress...\n");
    gettimeofday(&testStart, NULL);
    result = testCompress() ? "passed" : "failed";
    gettimeofday(&end, NULL);
    duration = ((double)end.tv_sec + (double)end.tv_usec/1000000) - ((double)testStart.tv_sec + (double)testStart.tv_usec/1000000); 
    printf("\t2-pass compress test %s in %.4fs.\n", result, duration);
    
    gettimeofday(&end, NULL);
    duration = ((double)end.tv_sec + (double)end.tv_usec/1000000) - ((double)mainStart.tv_sec + (double)mainStart.tv_usec/1000000); 
    printf("All tests done in %.4fs.\n", duration);
    return 1;
}