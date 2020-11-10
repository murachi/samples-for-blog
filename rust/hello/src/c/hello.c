#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hello()
{
  printf("%s\n", "C言語の世界へようこそ…。");
}

char const * static_str()
{
  static char const* text = "静的領域に配置した文字列";
  return text;
}

char const * dynamic_str()
{
  char * text = (char *)malloc(BUFSIZ);
  strncpy(text, "ヒープに配置した文字列", BUFSIZ);
  return text;
}
