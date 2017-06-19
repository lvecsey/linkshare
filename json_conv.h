#ifndef JSON_CONV_H
#define JSON_CONV_H

#include <unistd.h>

typedef struct {
  char linkshare_title[120];
  char linkshare_url[120];
  char linkshare_sort[20];
} json_conv_t;

int fill_json_conv(json_conv_t *json_conv, char *query);

int json_fwd_print(json_conv_t *json_conv, char *json, size_t len);
int json_rev_print(json_conv_t *json_conv, char *json, size_t len);

#endif

