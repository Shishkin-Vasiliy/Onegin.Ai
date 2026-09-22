#include "header.h"

int OpenFile(const char *file_name, struct stat *stat_buf)
{
    int descr = open(file_name, O_RDONLY);
    stat(file_name, stat_buf);
    return descr;
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

void PrintStrings(char *index[], size_t nlines, const char *REASON)
{
    size_t i = 0;

    printf("%s\n\n", REASON);
    for (; i < nlines; i++)
        printf("%s\n", index[i]);
    printf("***************************************\n\n\n\n\n\n\n\n\n\n\n");
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