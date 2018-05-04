#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct key_value{
	int key;
	int value;
	struct key_value * prev;
	struct key_value * next;
} key_value;

int main(int argc, char * argv)
{
  key_value msg = {10, 10, NULL, NULL};
	key_value * msgPtr  = &msg;
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
  send(sockID, msgPtr, sizeof(msgPtr), MSG_CONFIRM);
}
