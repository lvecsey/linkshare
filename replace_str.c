
int replace_str(char *str, char inc, char outc) {

  while (*str) {

    if (str[0] == inc) str[0] = outc;
    
    str++;
    
  }

  return 0;
  
}
