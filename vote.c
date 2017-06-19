
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include "critbit.h"

int vote_increment_core(char *hexstr) {

  int retval;

  unsigned int value;
  
  retval = sscanf(hexstr, "%x", &value);

  value++;

  retval = sprintf(hexstr, "%0.6x", value);

  return 0;
  
}

int vote_decrement_core(char *hexstr) {

  int retval;

  unsigned int value;
  
  retval = sscanf(hexstr, "%x", &value);

  value--;

  retval = sprintf(hexstr, "%0.6x", value);

  return 0;
  
}

int vote_increment(char *json_str) {

  char *ptr;

  int retval;
  
  ptr = json_str;
  while (*ptr != ':') {
    ptr++;
  }

  if (*ptr == ':') {

    char *hexstr;

    ptr++;

    hexstr = ptr;

    vote_increment_core(hexstr);
        
  }
  
  return 0;

}

int vote_decrement(char *json_str) {

  char *ptr;

  int retval;
  
  ptr = json_str;
  while (*ptr != ':') {
    ptr++;
  }

  if (*ptr == ':') {

    char *hexstr;

    ptr++;

    hexstr = ptr;

    vote_decrement_core(hexstr);
        
  }
  
  return 0;

}

int rework_voting(const char *str, void *extra) {

  char *hexval;

  char *ptr;

  int retval;

  char *title = "Example";
  char *url = "https://www.example.com/";
  
  if (str!=NULL) {

    ptr = strstr(str, "\"sort\":\"");
    if (ptr != NULL) {

      char insbuf[512];
      char delbuf[512];
      
      ptr += 8;
      hexval = ptr;

      sprintf(delbuf, "{\"sort\":\"%s\",\"title\":\"%s\",\"url\":\"%s\"}", hexval, title, url);
      
      vote_increment_core(hexval);

      sprintf(insbuf, "{\"sort\":\"%s\",\"title\":\"%s\",\"url\":\"%s\"}", hexval, title, url);
      
      critbit0_insert(extra, insbuf);
      critbit0_delete(extra, delbuf);      
      
    }
    
  }
  
  return 0;
  
}

int upvote(critbit0_tree *ls_entries, char *title, char *url) {

  char strbuf[512];

  int retval;
  
  sprintf(strbuf, "{\"title\":\"%s\",\"url\":\"%s\",\"sort\":\"", title, url);

  retval = critbit0_allprefixed(ls_entries, strbuf, rework_voting, ls_entries);
  
  return 0;
  
}