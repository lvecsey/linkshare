
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include <uriparser/UriBase.h>
#include <uriparser/Uri.h>

int json_conv(char *query, char *json, size_t len) {

  char linkshare_title[120];
  char linkshare_url[120];

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

    if (uqla->value != NULL && !strncmp(uqla->key, "linkshare_url", 13)) {
      memset(linkshare_url, 0, sizeof(linkshare_url));
      linkshare_len = strlen(uqla->value);
      if (linkshare_len > sizeof(linkshare_url)) {
	linkshare_len = sizeof(linkshare_url) - 1;
      }
      strncpy(linkshare_url, uqla->value, linkshare_len);
    }

    if (uqla->value != NULL && !strncmp(uqla->key, "linkshare_title", 15)) {
      memset(linkshare_title, 0, sizeof(linkshare_title));
      linkshare_len = strlen(uqla->value);
      if (linkshare_len > sizeof(linkshare_title)) {
	linkshare_len = sizeof(linkshare_title) - 1;
      }
      strncpy(linkshare_title, uqla->value, linkshare_len);
    }
    
    uqla = uqla->next;
	
  }

  uriFreeQueryListA(queryList);      
  
  retval = sprintf(json, "{\"title\":\"%s\",\"url\":\"%s\"}", linkshare_title, linkshare_url);
  
  return 0;

}
