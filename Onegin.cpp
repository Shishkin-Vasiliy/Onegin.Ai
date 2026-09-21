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

void Swap(void *value_a, void *value_b, size_t SizeElem);
void Qsort(void *data, int left, int right, size_t SizeElem, int (*how_to_compare)(const void *a, const void *b));
int StrCmpLeft(const void *s1, const void *s2);
int StrCmpRight(const void *s1, const void *s2);
void PrintStrings(char *index[], size_t nlines, const char *REASON);
int ReadFromFileNew(const char *file_name, char **buf, char *index[], struct stat stat_buf, int descr);
int SplitBuf(char **buf, char *index[]);
int OpenFile(const char *file_name, struct stat *stat_buf);
void FreeBuf(char **buf, int file_size);

int main(void)
{
    char *index[MAXLINES] = {};
    char *temp[MAXLINES] = {};
    const char *file_name = "cleaned.txt";
    char *buf = 0;
    int nlines = 0;

    struct stat stat_buf = {};
    int descr = OpenFile(file_name, &stat_buf);

    nlines = ReadFromFileNew(file_name, &buf, index, stat_buf, descr);

    for (size_t i = 0; i < nlines; i++)
    {
        temp[i] = index[i];
    }
    
    Qsort(index, 0, nlines - 1, sizeof(index[0]), StrCmpLeft);
    PrintStrings(index, nlines, "SORTED_FROM_LEFT");

    Qsort(index, 0, nlines - 1, sizeof(index[0]), StrCmpRight);
    PrintStrings(index, nlines, "SORTED_FROM_RIGHT");

    PrintStrings(temp, nlines, "READ");

    FreeBuf(&buf, stat_buf.st_size);

    return 0;
}

int StrCmpLeft(const void *ptr_at_ptr1, const void *ptr_at_ptr2)
{
    const char *s1 = *(const char **)ptr_at_ptr1;
    const char *s2 = *(const char **)ptr_at_ptr2;

    for (; *s1 == *s2 || (tolower(*s1) == tolower(*s2)); s1++, s2++)
    {
        if (*s1 == '\0')
            break;
        if (!isalpha(*s1))
            continue;
    }
    if (*s1 > *s2)
        return 1;
    else if (*s1 < *s2)
        return -1;
    else 
        return 0;
}

int StrCmpRight(const void *ptr_at_ptr1, const void *ptr_at_ptr2)
{
    const char *s1 = *(const char **)ptr_at_ptr1;
    const char *s2 = *(const char **)ptr_at_ptr2;
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char *temp1 = (char *)s1 + len1 - 1;
    char *temp2 = (char *)s2 + len2 - 1;

    while((len1 && len2) && ((*temp1 == *temp2) || (tolower(*temp1) == tolower(*temp2))))
    {
        len1--;
        len2--;
        temp1--;
        temp2--;
        if (!isalpha(*temp1))
            continue;
    } 

    if (*temp1 > *temp2)
        return 1;
    else if (*temp1 < *temp2)
        return -1;
    else 
        return 0;
}

void Swap(void *value_a, void *value_b, size_t SizeElem)
{
    int n = SizeElem;
    uint8_t *a = (uint8_t *)value_a;
    uint8_t *b = (uint8_t *)value_b;
    uint8_t temp = 0;

    do
    {
        n--;

        temp = *(a + n);
        *(a + n) = *(b + n);
        *(b + n) = temp;
    } while (n);
}

void Qsort(void *data, int left, int right, size_t SizeElem, int (*how_to_compare)(const void *a, const void *b))
{
    size_t i = 0;
    size_t last = 0;
    uint8_t *data_ptr = (uint8_t *)data;

    if (left >= right)
        return;
    
    Swap(data_ptr + left * SizeElem, data_ptr + ((left + right) / 2) * SizeElem, SizeElem);

    last = left;

    for (i = left + 1; i <= right; i++)
    {
        if (how_to_compare(data_ptr + i * SizeElem, data_ptr + left * SizeElem) < 0)
        {
            last++;
            Swap(data_ptr + i * SizeElem, data_ptr + last * SizeElem, SizeElem);
        }
    }
    
    Swap(data_ptr + left * SizeElem, data_ptr + last * SizeElem, SizeElem);
    Qsort(data, left, last - 1, SizeElem, how_to_compare);
    Qsort(data, last + 1, right, SizeElem, how_to_compare);
}

void PrintStrings(char *index[], size_t nlines, const char *REASON)
{
    size_t i = 0;

    printf("%s\n\n", REASON);
    for (; i < nlines; i++)
        printf("%s\n", index[i]);
    printf("***************************************\n\n\n\n\n\n\n\n\n\n\n");
}

int ReadFromFileNew(const char *file_name, char **buf, char *index[], struct stat stat_buf, int descr)
{
    int file_size = 0;
    int nlines = 0;

    if (descr == EOF)
        return EOF;

    file_size = stat_buf.st_size;
    
    *buf = (char *)calloc(file_size + 1, 1);
    read(descr, *buf, file_size);

    *(*buf + file_size) = '\0'; 

    nlines = SplitBuf(buf, index);

    return nlines;
}

int SplitBuf(char **buf, char *index[])
{
    const char *delim_windows = "\r\n";
    int len_delim_windows = strlen(delim_windows);
    int i = 0;
    int nlines = 1;

    char *temp = *buf;
    index[i] = temp;
    while (*temp != '\0')
    {
        if (strchr(delim_windows, *temp) != NULL)
        {
          *temp = '\0';
          temp += len_delim_windows;
          i++;
          nlines++;
          while(isspace(*temp))
            temp++;  
          index[i] = temp;
          continue;
        }
        temp++;            
    }
    return nlines;
}

int OpenFile(const char *file_name, struct stat *stat_buf)
{
    int descr = open(file_name, O_RDONLY);
    stat(file_name, stat_buf);
    return descr;
}

void FreeBuf(char **buf, int file_size)
{
    char *pos = *buf;
    for (int i = 0; i < file_size + 1; i++)
    {
        pos[i] = POIZON_CHAR;
    }
    free(*buf);
    *buf = POIZON_PTR;
}       