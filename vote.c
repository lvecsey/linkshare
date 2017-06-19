
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include "critbit.h"

int vote_increment_core(char *hexstr) {

  int retval;

  unsigned int value;
  
  retval = sscanf(hexstr, "%x", &value);

  if (value < 0xffffff) {
    value++;
  }
    
  retval = sprintf(hexstr, "%0.6x", value);

  return 0;
  
}

int vote_decrement_core(char *hexstr) {

  int retval;

  unsigned int value;
  
  retval = sscanf(hexstr, "%x", &value);

  if (value > 0) {
    value--;
  }
  
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

typedef struct {

  char *title;
  char *url;

  char vote_direction;
  
  critbit0_tree *ls_entries;
  
} rework_t;

int rework_voting(const char *str, void *extra) {

  rework_t *rework = (rework_t*) extra;
  
  char *hexval;

  char *ptr;

  int retval;

  if (str!=NULL) {

    ptr = strstr(str, "\"sort\":\"");
    if (ptr != NULL) {

      char insbuf[512];
      char delbuf[512];
      
      ptr += 8;
      hexval = ptr;

      sprintf(delbuf, "{\"sort\":\"%.6s\",\"title\":\"%s\",\"url\":\"%s\"}", hexval, rework->title, rework->url);

      switch(rework->vote_direction) {
      case 'U':
      case 'u': vote_increment_core(hexval); break;
      case 'D':
      case 'd': vote_decrement_core(hexval); break;
      }
	
      sprintf(insbuf, "{\"sort\":\"%.6s\",\"title\":\"%s\",\"url\":\"%s\"}", hexval, rework->title, rework->url);
      
      critbit0_insert(rework->ls_entries, insbuf);
      critbit0_delete(rework->ls_entries, delbuf);      
      
    }
    
  }
  
  return 0;
  
}

int upvote(critbit0_tree *ls_entries, char *title, char *url) {

  char strbuf[512];

  rework_t rework = { .title = title, .url = url, .ls_entries = ls_entries, .vote_direction = 'U' };
  
  int retval;
  
  sprintf(strbuf, "{\"title\":\"%s\",\"url\":\"%s\",\"sort\":\"", title, url);
  
  retval = critbit0_allprefixed(ls_entries, strbuf, rework_voting, &rework);
  
  return 0;
  
}

int downvote(critbit0_tree *ls_entries, char *title, char *url) {

  char strbuf[512];

  rework_t rework = { .title = title, .url = url, .ls_entries = ls_entries, .vote_direction = 'D' };
  
  int retval;
  
  sprintf(strbuf, "{\"title\":\"%s\",\"url\":\"%s\",\"sort\":\"", title, url);
  
  retval = critbit0_allprefixed(ls_entries, strbuf, rework_voting, &rework);
  
  return 0;
  
}
