#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>

#include      <unistd.h>
#include      <sys/types.h>
#include      <sys/socket.h>
#include      <arpa/inet.h>

#define MAXLINE 4096

void str_echo(int sockfd){
     ssize_t n;
     char    buf[MAXLINE];

   
}

int main(int argc, char **argv) {
      int     listenfd, connfd;
      pid_t   childpid;
      socklen_t clilen;
      struct sockaddr_in cliaddr, servaddr;

      listenfd = socket (AF_INET, SOCK_STREAM, 0);

     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
     servaddr.sin_port = htons (atoi(argv[1]));

     bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

     listen(listenfd, 1024);

     for ( ; ; )  {
         clilen = sizeof(cliaddr);
         connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

         if ( (childpid = fork()) == 0) { /* child process */
             close(listenfd);    /* close listening socket */
             str_echo(connfd);   /* process the request */
             exit (0);
         }
         close(connfd);          /* parent closes connected socket */
     }
 }
