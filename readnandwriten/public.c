#include<stdio.h>
#include<errno.h>
#include"public.h"
ssize_t readn(int fd,void* buf,int count)
{
  int leftSize=count;
  char* p=(char*)buf;
  while(leftSize>0)
  {
    int r=read(fd,p,leftSize);
    if(r==-1)
    {
      if(errno==EINTR)
      {
        continue;
      }
      else 
        return -1;
    }
    if(r==0) 
    {
      return count-leftSize;
    }
    p+=r;
    leftSize-=r;
  }
  return count;
}
ssize_t writen(int fd,void* buf,int count)
{
  int leftSize=count;
  char* p=(char*)buf;
  while(leftSize>0)
  {
    int r=write(fd,p,leftSize);
    if(r==0)
    {
      return count-leftSize;
    }
    if(r==-1)
    {
      if(errno==EINTR)
      {
        continue;
      }
      else 
      {
        return -1;
      }
    }
    leftSize-=r;
    p+=r;
  }
  return count;
}
