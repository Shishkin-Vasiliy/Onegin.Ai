#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cstdint>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINES 10000
#define MAXLEN 1000

#define POIZON_PTR (char *) 13
#define POIZON_CHAR '@'

int Qsort(void *data, int left, int right, size_t SizeElem, int (*how_to_compare)(const void *a, const void *b));
int Swap(void *value_a, void *value_b, size_t SizeElem);
int StrCmpLeft(const void *s1, const void *s2);
int StrCmpRight(const void *s1, const void *s2);

int OpenFile(const char *file_name);
int GetFileSize(const char *file_name);
int ReadFromFileNew(char **buf, char **index, int file_size, int descr);
int SplitBuf(char **buf, char **index);
void PrintStrings(char **index, size_t nlines, const char *REASON);
int FreeBuf(char **buf, int file_size);

#endif