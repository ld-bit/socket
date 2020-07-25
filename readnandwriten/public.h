#include<unistd.h>
typedef struct{
  int len;
  char contex[1024];
}msg_t;
ssize_t readn(int fd,void* buf,int count);
ssize_t writen(int fd,void* buf,int count);

