//Automatically tests fucntionality of compress.c
//Must be in the same directory as compress.c and a video named test.mp4

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

int testSimpleCompress() 
{
    char * input = "./test.mp4";
    char * outputPath = ".";
    char * outputName = "testOut";
    char * preset = "medium";
    char * output = "./testOut.mp4";
    struct stat statbuf;

    stat(input, &statbuf);
    int inputSize = statbuf.st_size;
    char command[256];
    sprintf(command, "./compress %s %s %s %s", input, outputPath, outputName, preset);
    printf("Test: Calling ./compress\n");
    system(command);
    puts("Test: System call done");
    if(access(output, F_OK) != 0) {
        printf("Test failed - Output not created.\n");
        return 0;
    }
    stat(output, &statbuf);
    int outputSize = statbuf.st_size;
    char * result = (outputSize > inputSize) ? "larger" : "smaller";
    printf("\t-Output %iKB %s than input!\n", (outputSize - inputSize)/1000, result);
    remove(output);
    return 1;
}

int main()
{
    time_t start;
    time_t end;
    printf("Make-ing.\n");
    system("make");
    printf("Testing simpleCompress...\n");
    start = clock();
    char * result = testSimpleCompress() ? "passed" : "failed";
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("simpleCompress test %s in %.4fs.\n", result, duration);
    
    return 1;
}