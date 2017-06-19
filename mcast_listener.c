
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <uriparser/UriBase.h>
#include <uriparser/Uri.h>

#include "mcast_config.h"

#include "critbit.h"

#include "mcast.h"

#include "json_conv.h"

void *mcast_listener(void *extra) {

  void *ret = NULL;
  
  mcast_t *mcast = (mcast_t*) extra;
  
  struct sockaddr_in addr;
  int fd, nbytes,addrlen;
  struct ip_mreq mreq;
  char buf[MSGBUFSIZE];

  u_int yes=1;

  char json_str[512];
  
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

	upvote(mcast->ls_entries, "Example", "https://www.example.com/");

      }

      else if (!strncmp(buf+1, "\"linkshare_", 11)) {
      
	char prebuf[512];
      
	memset(prebuf,0,sizeof(prebuf));
      
	sprintf(prebuf, "sort=%0.6x&%s", 0x0, buf);
      
	{
	  json_conv_t json_conv;
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
