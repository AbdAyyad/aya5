#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>

#include      <unistd.h>
#include      <sys/types.h>
#include      <sys/socket.h>
#include      <arpa/inet.h>

#define MAXLINE 4096

void show_menue(){
	puts("1: List the file names on the server");
	puts("2: Upload a given text file ");
	puts("3: Download a given text file");
	puts("4: Quit");
	printf("your choice : ");
}

void str_cli(FILE *fp, int sockfd) {
     char    sendline[MAXLINE], recvline[MAXLINE];

     while (fgets(sendline, MAXLINE, fp) != NULL) {

         write(sockfd, sendline, strlen (sendline));

         fputs(recvline, stdout);
     }
}

void list(int sockfd){
	char sendline[MAXLINE], recvline[MAXLINE];
	strcpy(sendline,"list");
        write(sockfd, sendline, strlen (sendline));
        read(sockfd, recvline, MAXLINE);
	puts(recvline);
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

     int choice;
     while(1){
        write(sockfd, argv[3], strlen (argv[3]));        
	show_menue();
	scanf("%d",&choice);
        switch(choice){
           case 1:
                list(sockfd);
  		break;
           case 2:
	   break;
	   case 3:
	   break;
	   case 4:
		exit(0);
	        break;
	   default :
		puts("wrong choice");
        }
     }

     str_cli(stdin, sockfd);     /* do it all */

     
 }
