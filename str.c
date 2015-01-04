void int_to_str(unsigned int n, char *str)
{
  int i;
  
  for (i = 9; i >= 0; i--) {
    str[i] = n%10 + 48;
    n /= 10;
  }
  str[10] = '\0';

  /*x = 0;
  i = 9;
  for (i=9;i>=0;i--) {
    if (res[i] == '0')
      break;
    str[x++] = res[i];
  }
  str[x] = '\0';*/
}

void int_to_hex (unsigned int n, unsigned char *str) {
  int i;
  
  for (i = 9; i >= 2; i--) {
    str[i] = n%16 + 48;
    if (str[i] >= 58)
      str[i] += 7;
    n /= 16;
  }
  
  str[0] = '0';
  str[1] = 'x';
  
  str[10] = '\0';
}

unsigned int string_length (char *string) {
  char *str = string;
  while (*str != '\0') {
    str++;
  }
  return str - string;
}