
#include "fcgi_stdio.h"
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <pthread.h>
#include <errno.h>

#define __USE_MISC
#define __USE_GNU

#include <netinet/in.h>
#include <arpa/inet.h>

#include <uriparser/UriBase.h>
#include <uriparser/Uri.h>

#include <string.h>

#include "mcast_config.h"

#include "mcast_listener.h"

#include "critbit.h"

#include "mcast.h"

#include "vote.h"

#include "json_conv.h"

int main(int argc, char *argv[]) {

  critbit0_tree ls_entries = { .root = NULL };

  pthread_t mc_listen;
  
  long int count = 0;

  long int debug = 0;

  int retval;

  struct sockaddr_in addr;
  int fd, cnt;
  struct ip_mreq mreq;
  char *env_SERVER_NAME;

  unsigned char buf[4096];
  unsigned char *adv_p;
  size_t remaining, len = sizeof(buf);
  ssize_t bytes_read;

  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  int log_fd = open("/tmp/linkshare.log", O_RDWR | O_CREAT | O_APPEND, mode);

  long int pass;

  ssize_t bytes_written;

  UriUriA uri;
  UriQueryListA *queryList;
  int itemCount;

  char email_address[240];

  size_t email_len;

  mcast_t mcast = { .ls_entries = &ls_entries, .state = 0, .init = 1 };

  char *entries_str;

  entries_str = malloc(8192);
  if (entries_str==NULL) {
    perror("malloc");
    return -1;
  }
  
  if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=inet_addr(MCAST_GROUP);
  addr.sin_port=htons(MCAST_PORT);

  critbit0_insert(&ls_entries, "{\"sort\":\"000000\",\"title\":\"Pay Chess, LLC\",\"url\":\"https://www.paychess.com/\"}");

  critbit0_insert(&ls_entries, "{\"title\":\"Pay Chess, LLC\",\"url\":\"https://www.paychess.com/\",\"sort\":\"000000\"}");  

  critbit0_insert(&ls_entries, "{\"sort\":\"000000\",\"title\":\"Example\",\"url\":\"https://www.example.com/\"}");

  critbit0_insert(&ls_entries, "{\"title\":\"Example\",\"url\":\"https://www.example.com/\",\"sort\":\"000000\"}");  
  
  retval = pthread_create(&mc_listen, NULL, mcast_listener, &mcast);
  if (retval != 0) {
    fprintf(stderr, "%s: Trouble with call to pthread_cast for multicast listener.\n", __FUNCTION__);
    return -1;
  }
  
  while(FCGI_Accept() >= 0) {

    env_SERVER_NAME = getenv("SERVER_NAME");

    memset(buf, 0, sizeof(buf));

    remaining = len;
    adv_p = (unsigned char*) buf;
    while (remaining>0) {
      bytes_read = fread(adv_p + len - remaining, 1, remaining, stdin);
      if (bytes_read <= 0) break;
      bytes_written = write(log_fd, adv_p+len-remaining, bytes_read);
      //fwrite(adv_p + len - remaining, 1, bytes_read, stdout);
      remaining -= bytes_read;
    }

    if (buf[0] != '{' || !strncmp(buf, "linkshare_", 10) || !strncmp(buf, "sort=", 5)) {

      UriQueryListA *uqla;

      long int n;

      char json_str[512];

      size_t json_str_len;
      
      json_conv_t json_conv;
      
      retval = fill_json_conv(&json_conv, buf);

      sprintf(json_conv.linkshare_sort, "%0.6x", 0x0);
      
      retval = json_fwd_print(&json_conv, json_str, sizeof(json_str));

      json_str_len = strlen(json_str);
      
      if (sendto(fd,json_str,json_str_len,0,(struct sockaddr *) &addr, sizeof(addr)) < 0) {
	perror("sendto");
	return -1;
      }
      
      printf("Content-type: text/html; charset=utf-8\r\n"
	     "\r\n"
	     "<HTML><HEAD>\n"
	     "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"1; URL=/?%s\">\n"
	     "<title>FastCGI linkshare submission!</title>"
	     "Request number %ld running on host <i>%s</i>\n"
	     ,pass ? "success" : "failure"
	     ,++count, env_SERVER_NAME!=NULL ? env_SERVER_NAME : "NULL");

      printf("<pre>");
      if (debug) puts(buf);
      printf("</pre>");
      printf("</body></html>");

      }
      
    pass = 1;

    if (buf[0] == '{') {
      
      size_t entries_len;
      
      if (sendto(fd,buf,len,0,(struct sockaddr *) &addr, sizeof(addr)) < 0) {
	perror("sendto");
	return -1;
      }
      
      retval = fill_entries(entries_str, 8192, &ls_entries);
      
      entries_len = strlen(entries_str);      
      
      printf("Content-type: application/javascript; charset=utf-8\r\n"
	   "Content-length: %ld\r\n"
	   "\r\n"
	   , entries_len);

      fwrite(entries_str, 1, entries_len, stdout);

      continue;

    }    

  }

  return 0;

}


