
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include "critbit.h"

typedef struct {

  char *fill_ptr;

  char *base;
  size_t len;

  long int num_entries;
  long int max_entries;
  
} cbfill_t;

int entryfill_cb(const char *str, void *extra) {

  cbfill_t *cbfill = (cbfill_t*) extra;

  size_t local_len;

  int retval;
  
  if (str!=NULL && cbfill->num_entries < cbfill->max_entries) {

    if (cbfill->fill_ptr > cbfill->base + 1) {
      *cbfill->fill_ptr++ = ',';
    }

    local_len = strlen(str);
    memcpy(cbfill->fill_ptr, str, local_len);
    cbfill->fill_ptr += local_len;

    cbfill->num_entries++;
    
  }
  
  return 1;
  
}

int fill_entries(char *entries_str, size_t len, critbit0_tree *cb) {
  
  cbfill_t cbfill = { .base = entries_str, .len = len };

  cbfill.num_entries = 0;
  cbfill.max_entries = 100;
  
  cbfill.fill_ptr = cbfill.base;
  *cbfill.fill_ptr++ = '[';
  
  critbit0_allprefixed(cb, "{\"sort\":", entryfill_cb, &cbfill);

  *cbfill.fill_ptr++ = ']';

  *cbfill.fill_ptr = 0;

  return 0;

}
