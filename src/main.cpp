#include "header.h"

int main(void)
{
    const char *file_name = "Onegin.txt";
    char *buf = POIZON_PTR; 
    int nlines = 0;

    int descr = OpenFile(file_name);
    int file_size = GetFileSize(file_name);

    char **index = (char **)calloc(file_size + 1, sizeof(char *));
    assert(index);

    char **temp  = (char **)calloc(file_size + 1, sizeof(char *));
    assert(temp);

    nlines = ReadFromFileNew(&buf, index, file_size, descr);

    for (size_t i = 0; i < nlines; i++)
    {
        temp[i] = index[i];
    }
    
    Qsort(index, 0, nlines - 1, sizeof(index[0]), StrCmpLeft);
    PrintStrings(index, nlines, "SORTED_FROM_LEFT");

    Qsort(index, 0, nlines - 1, sizeof(index[0]), StrCmpRight);
    PrintStrings(index, nlines, "SORTED_FROM_RIGHT");

    PrintStrings(temp, nlines, "READ");

    FreeBuf(&buf, file_size);

    return 0;
}
      