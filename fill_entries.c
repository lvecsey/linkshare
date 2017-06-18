
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include "critbit.h"

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

    if (cbfill->fill_ptr > cbfill->base + 1) {
      *cbfill->fill_ptr++ = ',';
    }

    local_len = strlen(str);
    memcpy(cbfill->fill_ptr, str, local_len);
    cbfill->fill_ptr += local_len;
    
  }
  
  return 1;
  
}

int fill_entries(char *entries_str, size_t len, critbit0_tree *cb) {
  
  cbfill_t cbfill = { .base = entries_str, .len = len };

  cbfill.fill_ptr = cbfill.base;
  *cbfill.fill_ptr++ = '[';
  
  critbit0_allprefixed(cb, "", entryfill_cb, &cbfill);

  *cbfill.fill_ptr++ = ']';

  *cbfill.fill_ptr = 0;

  return 0;

}
