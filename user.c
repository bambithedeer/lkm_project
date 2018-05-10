#include <sys/socket.h>
#include <linux/list.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct key_value{
	int key;
	int value;
	int size;
	struct list_head hdr;
} key_value;

int main(int argc, char ** argv)
{
	if (argc == 3){
	  key_value msg = {atoi(argv[1]), atoi(argv[2]), 0, NULL};
	  struct sockaddr_nl src;
	  printf("User started\n");

	  int sockID = socket(AF_NETLINK, SOCK_RAW, 20);

	  if (sockID < 0){
	    printf("Socket was not created\n");
	  }
	  else{
	    printf("Socket created successfully\n");
	  }

	  src.nl_family = AF_NETLINK;
	  src.nl_pid = getpid();

	  bind(sockID, (struct sockaddr*)&src, sizeof(src));
	  send(sockID, &msg, sizeof(&msg), MSG_CONFIRM);
	}
	else{
		printf("Usage: ./user key value\n");
	}
}
