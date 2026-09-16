#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cstdint>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define MAXLINES 10000
#define MAXLEN 1000

void Swap(void *value_a, void *value_b, size_t SizeElem);
void Qsort(void *data, int left, int right, size_t SizeElem, int (*how_to_compare)(const void *a, const void *b));
int StrCmpLeft(const void *s1, const void *s2);
int StrCmpRight(const void *s1, const void *s2);
int ReadFromFile(const char *file_name, char *index[]);
void PrintStrings(char *index[], size_t nlines, const char *REASON);


int main(void)
{
    char *index[MAXLINES] = {};
    const char *file = "Onegin_text.txt";
    int nlines = ReadFromFile(file, index);
    
    PrintStrings(index, nlines, "READ");
    Qsort(index, 0, nlines - 1, sizeof(index[0]), StrCmpRight);
    PrintStrings(index, nlines, "SORTED");

    //for (int i = 0; i < nlines; i++)
    //    free(index[i]);

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
        if (!isalpha(*temp1))
            continue;
        len1--;
        len2--;
        temp1--;
        temp2--;
    } 

    if (*temp1 > *temp2)
        return 1;
    else if (*temp1 < *temp2)
        return -1;
    else 
        return 0;
}

int ReadFromFile(const char *file, char *index[])
{
    FILE *file_ptr = fopen(file, "r");
    assert(file);

    char buf[MAXLEN] = "";
    size_t i = 0;
    size_t len = 0;
    

    //
    //petya^M$    - то что может лежать в txt из за винды 
    //vasya^M$      (символы \n или \r)
    //ricardo
    
    while (!feof(file_ptr) && i < MAXLINES)
    {
        fgets(buf, MAXLEN, file_ptr);
        len = strlen(buf);
        while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
        {
            buf[len - 1] = '\0';
            len--;
        }
        index[i] = strdup(buf);
        i++;
    }

    return i;
}

void Swap(void *value_a, void *value_b, size_t SizeElem)
{
    int n = SizeElem;
    uint8_t *a = (uint8_t *)value_a;
    uint8_t *b = (uint8_t *)value_b;
    
    do
    {
        n--;

        uint8_t temp = *(a + n);
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
    printf("***************************************\n\n\n");
}