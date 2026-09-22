#include "header.h"

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
      