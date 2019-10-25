
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool is_pd(char *s)
{
    char s1[1024] = {0};
    int len = strlen(s), i = 0;
    if (len == 1)
        return true;

    for (i = 0; i < len/2; i++)
    {
        s1[i] = s[len-i-1];
    }
    if (strncmp(s, s1, len/2) == 0)
        return true;
    else
        return false;
}

char * longestPalindrome(char * s)
{
   char s1[1024] = {0};
   char s2[1024] = {0};
   int i = 0, j = 0;
   bool flag = false;
   int max_len = 0, len = 0, cur_len = 0;
   char *lpd = NULL;

   if (!s)
       return NULL;
   len = strlen(s);
   if ((len == 0) || (len == 1))
       return s;

   for (i = 0; i < len-max_len; i++)
   {
        for (j = i+max_len; j < len; j++)
        {
            cur_len = j-i+1;
            memset(s1, 0, sizeof(s1));
            strncpy(s1, s+i, cur_len);
            flag = is_pd(s1);
            //printf("substr %s is a pd ? %d\n", s1, flag);
            if (flag)
            {
                if (cur_len > max_len)
                {
                    max_len = cur_len;
                    memset(s2, 0, sizeof(s2));
                    strncpy(s2, s1, cur_len);
                }
            }
        }
   }
   if (max_len >= 1)
   {
       lpd = (char *)malloc(max_len+1);
       if (lpd == NULL)
       {
            return NULL;
       }
       strncpy(lpd, s2, max_len);
       lpd[max_len] = 0;
   }
   return lpd;
}


int is_pd2(char *s, int left, int right)
{
    int len = strlen(s);
    int l_index = left;
    int r_index = right;

    while ((l_index >= 0) && (r_index < strlen(s)) && (s[l_index] == s[r_index]))
    {
        --l_index;
        ++r_index;
    }

    return r_index - l_index - 1;
}

char * longestPalindrome2(char * s)
{
    int len = 0; 
    int i = 0, j = 0;
    int max_len = 0;
    int max_len2 = 0;
    int len1 = 0;
    int len2 = 0;
    int l_index = 0;
    int r_index = 0;
    int l_index_max = 0;
    int r_index_max = 0;
    char *out_str = NULL;


    if (s == NULL)
        return NULL;

    len = strlen(s);
    for (i = 0; i < len-max_len/2; i++)
    {
        len1 = is_pd2(s, i, i);
        len2 = is_pd2(s, i, i+1);

        if (len2 >= len1)
        {
            max_len2 = len2;
            l_index = i - max_len2/2 + 1;
            r_index = i + max_len2/2;
        }
        else
        {
            max_len2 = len1;
            l_index = i - max_len2/2;
            r_index = i + max_len2/2;
        }

        if (max_len2 > max_len)
        {
            max_len = max_len2;
            l_index_max = l_index;
            r_index_max = r_index;
        }
    }

    out_str = (char *)malloc(max_len+1);
    if (out_str == NULL)
    {
        return NULL;
    }
    for (i = 0; i < max_len; i++)
    {
        out_str[i] = s[l_index_max+i];
    }
    out_str[max_len] = 0;

    return out_str;
}

int main()
{
    char input_str[1024] = {0};
	char *lsd = NULL;
    printf("please input string: \n");
    scanf("%s", input_str);
    printf("Your input string is: %s\n", input_str);

	lsd = longestPalindrome2(input_str);
	printf("longest palindrome is %s\n", lsd);
	if (lsd)
	{
        free(lsd);
	}
    return 0;
}
