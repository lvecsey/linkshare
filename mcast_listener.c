
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define __USE_MISC
#define __USE_GNU
#include <netinet/in.h>
#include <arpa/inet.h>

#include <uriparser/UriBase.h>
#include <uriparser/Uri.h>

#include "mcast_config.h"

#include "critbit.h"

#include "mcast.h"

#include <json.h>

#include "json_conv.h"

void *mcast_listener(void *extra) {

  void *ret = NULL;
  
  mcast_t *mcast = (mcast_t*) extra;
  
  struct sockaddr_in addr;
  int fd, nbytes,addrlen;
  struct ip_mreq mreq;
  char buf[MSGBUFSIZE];

  uint32_t yes = 1;

  char json_str[512];

  json_conv_t json_conv;
  
  ssize_t bytes_read;

  long int n;

  int retval;
  
  if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    perror("socket");
    exit(1);
  }


  /**** MODIFICATION TO ORIGINAL */
  /* allow multiple sockets to use the same PORT number */
  if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
    perror("Reusing ADDR failed");
    exit(1);
  }
  /*** END OF MODIFICATION TO ORIGINAL */

  /* set up destination address */
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
  addr.sin_port=htons(MCAST_PORT);
     
  /* bind to receive address */
  if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
     
  /* use setsockopt() to request that the kernel join a multicast group */
  mreq.imr_multiaddr.s_addr=inet_addr(MCAST_GROUP);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
    perror("setsockopt");
    exit(1);
  }

  for (;;) {

    addrlen=sizeof(addr);

    memset(buf, 0, sizeof(buf));
    
    if ((bytes_read=recvfrom(fd,buf,MSGBUFSIZE,0, (struct sockaddr *) &addr,&addrlen)) < 0) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }

    if (buf[0] == '{') {

      if (!strncmp(buf+1, "\"vote", 5)) {

	struct json_object *jobj;

	struct json_object *jobj_vote;
	struct json_object *jobj_title;
	struct json_object *jobj_url;

	// {"vote":"UP","title":"Example","url":"https://www.example.com/"}
      
	jobj = json_tokener_parse(buf);

	jobj_vote = json_object_object_get(jobj, "vote");      
	jobj_title = json_object_object_get(jobj, "title");
	jobj_url = json_object_object_get(jobj, "url");      
      
	if (jobj_vote != NULL) {

	  char *vote = json_object_get_string(jobj_vote);
	  char *title = json_object_get_string(jobj_title);
	  char *url = json_object_get_string(jobj_url);

	  if (vote!=NULL) {
	    if (vote[0] == 'U' || vote[0] == 'u') {
	      upvote(mcast->ls_entries, title, url);
	    }
	    else if (vote[0] == 'D' || vote[0] == 'd') {
	      downvote(mcast->ls_entries, title, url);
	    }

	  }
	    
	}

      }

      else if (!strncmp(buf+1, "\"sort", 5)) {

	retval = sscanf(buf,"{\"sort\":\"%[^\"]\",\"title\":\"%[^\"]\",\"url\":\"%[^\"]\"}", json_conv.linkshare_sort, json_conv.linkshare_title, json_conv.linkshare_url);

	if (retval == 3) {

	  retval = json_fwd_print(&json_conv, json_str, sizeof(json_str));
	  critbit0_insert(mcast->ls_entries, json_str);

	  retval = json_rev_print(&json_conv, json_str, sizeof(json_str));
	  critbit0_insert(mcast->ls_entries, json_str);
	  
	}
	
      }
      
      else if (!strncmp(buf+1, "\"linkshare_", 11)) {
      
	char prebuf[512];
      
	memset(prebuf,0,sizeof(prebuf));
      
	sprintf(prebuf, "sort=%0.6x&%s", 0x0, buf);
      
	{
	  retval = fill_json_conv(&json_conv, prebuf); 

	  retval = json_fwd_print(&json_conv, json_str, sizeof(json_str));
	  critbit0_insert(mcast->ls_entries, json_str);

	  retval = json_rev_print(&json_conv, json_str, sizeof(json_str));
	  critbit0_insert(mcast->ls_entries, json_str);
	
	}

      }
	
    }
    
  }

  return ret;
  
}
