#include <stdio.h>
 
int a = 1, b = 2;
int add (int x,int y)
{
  return x + y;
}
 
 
int main()
{
  int c;
  c = add(a, b); 
  printf("%d\n", c);
  return 0;
}
