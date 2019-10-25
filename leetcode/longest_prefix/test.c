

#if 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lsd_prefix(char *s1, int len1, char *s2, int len2)
{
   int i = 0;
   int len = (len1 > len2) ? len2 : len1;
   printf("%s/%d s1 %s/%d, s2 %s/%d\n", __FUNCTION__, __LINE__, s1, len1, s2, len2);
   for (i=0; i<len; i++)
   {
       if (s1[i] != s2[i])
       {
           printf("%s/%d longest prefix len %d\n", __FUNCTION__, __LINE__, i);
           return i;
       }
   }
   printf("%s/%d longest prefix len %d\n", __FUNCTION__, __LINE__, len);
   return len;
}

void lsd(char *s1, int len1, char *s2, int len2)
{
    int i = 0, j = 0, k = 0;
    int max_len = 0, len = 0;
    char suffix[1024] = {0};
    char suffix2[1024] = {0};
    char *lsdset[16] = {0};
    int lsdset_num = 0;
    char *lsd = NULL;

    printf("%s/%d s1 %s/%d, s2 %s/%d\n", __FUNCTION__, __LINE__, s1, len1, s2, len2);
    for (i = 0; (i < len1)&&((len1-i) > max_len); i++)
    {
        strncpy(suffix, s1+i, len1-i); 
        suffix[len1-i] = 0;
        for (j = 0; (j < len2)&&((len2-j) > max_len); j++)
        {
            strncpy(suffix2, s2+j, len2-j);
            suffix2[len2-j] = 0;
            len = lsd_prefix(suffix, len1-i, suffix2,len2-j);
            if (len >= max_len)
            {
                max_len = len;
                if ((len > max_len) && (lsdset_num > 0))
                {
                    for (k = 0; k < lsdset_num; k++)
                    {
                        if (lsdset[k] != NULL)
                        {
                            free(lsdset[k]);
                        }
                    }
                    lsdset_num = 0;
                }
                lsd = malloc(len+1);
                if (lsd == NULL)
                {
                   return;
                }
                strncpy(lsd, s1+i, len);
                lsd[len] = 0;
                lsdset[lsdset_num] = lsd;
                ++lsdset_num;
            }
        }
    }
    if (lsdset_num > 0)
    {
        for (k = 0; k < lsdset_num; k++)
        {
            printf("lsd is %s\n", lsdset[k]);
            free(lsdset[k]);
        }
    }
}

int main()
{
    char input_str[1024] = {0};
    int max_len = 0;
    int cur_len = 0;
    int left_index = 0;
    int right_index = 0;
    int i,j;
    printf("please input string: \n");
    scanf("%s", input_str);
    printf("Your input string is: %s\n", input_str);

    if (strlen(input_str) == 1)
        max_len = 1;
    else 
    {
        max_len = 1;
#if 0
        for (i = 0; i < strlen(input_str); i++)
        {
            for (j = i+max_len; j < strlen(input_str); j++)
            {
                int ii = i, jj = j;
                cur_len = 0;
                //printf("i %d j %d\n", i, j);
                while (jj >= ii)
                {
                    //printf(" %d %c %d %c\n", ii, input_str[ii], jj, input_str[jj]);
                    if (input_str[ii] == input_str[jj])
                    {
                        if (ii == jj)
                        {
                            cur_len++;
                        }
                        else
                        {
                            cur_len += 2;
                        }
                        ii++;
                        jj--;
                        //printf(" cur_len %d\n", cur_len);
                        continue;
                    }
                    else
                    {
                        cur_len = 0;
                        //printf(" clean cur_len %d\n", cur_len);
                        break;
                    }
                }
                if (cur_len > max_len)
                {
                    max_len = cur_len;
                    left_index = i;
                    right_index = j;
                }
                if (max_len == 0)
                {
                    max_len = 1;
                    left_index = right_index = 0;
                }
            }
        }
#endif
        {
            int len = 0;
            char rev_str[1024] = {0};
            char tmp_str1[1024] = {0};
            int str_len = strlen(input_str);
            for (i = 0; i < str_len; i++)
            {
                rev_str[i] = input_str[str_len-i-1];
            }
            lsd(input_str, strlen(input_str), rev_str, strlen(rev_str));
        }
    }
    printf("the longest plalindrome string len %d, %d %d\n", max_len, left_index, right_index);
    return 0;
}

#endif

