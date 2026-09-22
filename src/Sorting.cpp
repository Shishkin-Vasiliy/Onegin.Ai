#include "header.h"

int Qsort(void *data, int left, int right, size_t SizeElem, int (*how_to_compare)(const void *a, const void *b))
{
    if (!data)
        return EOF;

    size_t i = 0;
    size_t last = 0;
    uint8_t *data_ptr = (uint8_t *)data;

    if (left >= right)
        return 0;
    
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

    return 0;
}

int Swap(void *value_a, void *value_b, size_t SizeElem)
{
    if ((!value_a) ||  (!value_b))
        return EOF;

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

    return 0;
}

int StrCmpLeft(const void *ptr_at_ptr1, const void *ptr_at_ptr2)
{
    if ((!ptr_at_ptr1) || (!ptr_at_ptr2))
        return EOF;

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
    if ((!ptr_at_ptr1) || (!ptr_at_ptr2))
        return EOF;

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