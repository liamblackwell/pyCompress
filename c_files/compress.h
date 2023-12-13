#ifndef COMPRESS_H_
#define COMPRESS_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>

extern double video_duration_seconds(char*);
extern int simple(int, char *, char *, char *, char *);
extern int twoPass(int, char *, char *, char *, double, char *);

#endif