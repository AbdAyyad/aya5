#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>

#include      <unistd.h>
#include      <sys/types.h>
#include      <sys/socket.h>
#include      <arpa/inet.h>

#define MAXLINE 4096

void str_cli(FILE *fp, int sockfd) {
     char    sendline[MAXLINE], recvline[MAXLINE];

     while (fgets(sendline, MAXLINE, fp) != NULL) {

         write(sockfd, sendline, strlen (sendline));

         fputs(recvline, stdout);
     }
 }

int main(int argc, char **argv){
     int     sockfd;
     struct sockaddr_in servaddr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(atoi(argv[2]));
     inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

     connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

     str_cli(stdin, sockfd);     /* do it all */

     exit(0);
 }
