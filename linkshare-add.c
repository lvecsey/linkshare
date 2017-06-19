
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "mcast_config.h"

#include "replace_str.h"

int main(int argc, char *argv[]) {

  char *linkshare_title = argc>1 ? argv[1] : "Default+Title";
  char *linkshare_url = argc>2 ? argv[2] : "https://www.example.com/";

  char strbuf[480];
  
  struct sockaddr_in addr;
  int fd, cnt;
  struct ip_mreq mreq;

  size_t len;
  
  if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=inet_addr(MCAST_GROUP);
  addr.sin_port=htons(MCAST_PORT);
     
  {

    replace_str(linkshare_title, ' ', '+');
    
    sprintf(strbuf, "{\"sort\"=\"000000\",\"title\"=\"%s\",\"url\"=\"%s\"}", linkshare_title, linkshare_url);
    
    len = strlen(strbuf);
    
    if (sendto(fd,strbuf,len,0,(struct sockaddr *) &addr,
	       sizeof(addr)) < 0) {
      perror("sendto");
      exit(1);
    }
  }

}

