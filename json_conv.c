
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include <uriparser/UriBase.h>
#include <uriparser/Uri.h>

#include "json_conv.h"

int fill_json_conv(json_conv_t *json_conv, char *query) {

  char *buf;
  size_t len;
  
  UriUriA uri;
  UriQueryListA *queryList;
  int itemCount;

  UriQueryListA *uqla;
  
  int retval;

  size_t query_len = strlen(query);

  size_t linkshare_len;

  long int n;

  if (uriDissectQueryMallocA(&queryList, &itemCount, query, query+query_len) != URI_SUCCESS) {

  }

  uqla = queryList;
      
  for (n = 0; n < itemCount; n++) {

    buf = json_conv->linkshare_url;
    len = sizeof(json_conv->linkshare_url);
    if (uqla->value != NULL && !strncmp(uqla->key, "linkshare_url", 13)) {
      memset(buf, 0, len);
      linkshare_len = strlen(uqla->value);
      if (linkshare_len > len) {
	linkshare_len = len - 1;
      }
      strncpy(buf, uqla->value, linkshare_len);
    }

    buf = json_conv->linkshare_title;
    len = sizeof(json_conv->linkshare_title);
    if (uqla->value != NULL && !strncmp(uqla->key, "linkshare_title", 15)) {
      memset(buf, 0, len);
      linkshare_len = strlen(uqla->value);
      if (linkshare_len > len) {
	linkshare_len = len - 1;
      }
      strncpy(buf, uqla->value, linkshare_len);
    }

    buf = json_conv->linkshare_sort;
    len = sizeof(json_conv->linkshare_sort);    
    if (uqla->value != NULL && !strncmp(uqla->key, "sort", 4)) {
      memset(buf, 0, len);
      linkshare_len = strlen(uqla->value);
      if (linkshare_len > len) {
	linkshare_len = len - 1;
      }
      strncpy(buf, uqla->value, linkshare_len);
    }
    
    uqla = uqla->next;
	
  }

  uriFreeQueryListA(queryList);      
    
  return 0;

}

int json_fwd_print(json_conv_t *json_conv, char *json, size_t len) {

  int retval;
  
  retval = sprintf(json, "{\"sort\":\"%s\",\"title\":\"%s\",\"url\":\"%s\"}", json_conv->linkshare_sort, json_conv->linkshare_title, json_conv->linkshare_url);

  return 0;
  
}

int json_rev_print(json_conv_t *json_conv, char *json, size_t len) {

  int retval;
  
  retval = sprintf(json, "{\"title\":\"%s\",\"url\":\"%s\",\"sort\":\"%s\"}", json_conv->linkshare_title, json_conv->linkshare_url, json_conv->linkshare_sort);

  return 0;
  
}
