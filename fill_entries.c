
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include "critbit.h"

#include "json_conv.h"

typedef struct {

  char *fill_ptr;

  char *base;
  size_t len;
  
} cbfill_t;

int entryfill_cb(const char *str, void *extra) {

  cbfill_t *cbfill = (cbfill_t*) extra;

  size_t local_len;

  int retval;
  
  if (str!=NULL) {

    char strbuf[240];

    retval = json_conv(str, strbuf, sizeof(strbuf));
    
    local_len = strlen(strbuf);

    if (cbfill->fill_ptr > cbfill->base + 1) {
      *cbfill->fill_ptr++ = ',';
    }
    
    memcpy(cbfill->fill_ptr, strbuf, local_len);
    cbfill->fill_ptr += local_len;
    
  }
  
  return 1;
  
}

int fill_entries(char *entries_str, size_t len, critbit0_tree *cb) {
  
  char *entries_str_default = "["
	"{\"title\":\"Pay Chess, LLC.\",\"url\":\"https://www.paychess.com/\"}"
	",{\"title\":\"Oath\",\"url\":\"https://www.oath.com/\"}"
	",{\"title\":\"NASA\",\"url\":\"https://www.nasa.gov/\"}"	
	"]";

  cbfill_t cbfill = { .base = entries_str, .len = len };

  cbfill.fill_ptr = cbfill.base;
  *cbfill.fill_ptr++ = '[';
  
  critbit0_allprefixed(cb, "", entryfill_cb, &cbfill);

  *cbfill.fill_ptr++ = ']';

  *cbfill.fill_ptr = 0;
  
  // strcpy(entries_str, entries_str_default);

  return 0;

}
