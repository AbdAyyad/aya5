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

void list(int sockfd){
	char sendline[MAXLINE], recvline[MAXLINE];
	strcpy(sendline,"list");
        write(sockfd, sendline, strlen (sendline));
        read(sockfd, recvline, MAXLINE);
	puts("list of files:");
	puts(recvline);
}

void upload(int sockfd){
	char sendline[MAXLINE], recvline[MAXLINE];
	char * ptr;
	strcpy(sendline,"upload");
        write(sockfd, sendline, strlen (sendline));
	sync();
	printf("enter file path: ");
	scanf("%s",sendline);
	//puts(sendline);
	fflush(stdout);	
	FILE* file = fopen(sendline,"r");
	ptr = strrchr(sendline, '/');
	//puts(ptr);	
	strcpy(sendline,ptr+1);
	//puts(sendline);
        write(sockfd, sendline, strlen (sendline));
 	sync();
	while(fgets(sendline,MAXLINE,file) != NULL){
                printf("read: %s",sendline);
		write(sockfd, sendline, strlen (sendline));
		bzero(sendline,MAXLINE);
		sync();
	}
	bzero(sendline,MAXLINE);
	bzero(recvline,MAXLINE);
	strcpy(sendline,"./././././././././././././././././");
	write(sockfd, sendline, strlen (sendline));
	//sync();
	bzero(sendline,MAXLINE);	        
	read(sockfd, recvline, MAXLINE);
	puts(recvline);
}

void download(int sockfd){
	char sendline[MAXLINE], recvline[MAXLINE];
	strcpy(sendline,"download");
        write(sockfd, sendline, strlen (sendline));
	read(sockfd, recvline, MAXLINE);
	// // // // // // // // // // //
        //    error message           //
	// // // // // // // // // // // 
	FILE* file = fopen(sendline,"w");
	read(sockfd, recvline, MAXLINE);
	while(fgets(recvline,MAXLINE,file) != NULL){
		if(strcmp(recvline,"./././././././././././././././././")==0){
			fclose(file);			
			break;		
		}                
		puts(recvline);
		fprintf(file,"%s",recvline);
	}
	puts("download complete");
}

int main(int argc, char **argv){
     	int     sockfd;
     	struct sockaddr_in servaddr;
	int choice;
             
	
     while(1){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

     	bzero(&servaddr, sizeof(servaddr));
     	servaddr.sin_family = AF_INET;
     	servaddr.sin_port = htons(atoi(argv[2]));
     	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	int n = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	//printf("n %d\n",n);
	write(sockfd, argv[3], strlen (argv[3]));     
	show_menue();
	scanf("%d",&choice);
        switch(choice){
           case 1:
                list(sockfd);
  		break;
           case 2:
		upload(sockfd);
	   	break;
	   case 3:
		download(sockfd);
	   	break;
	   case 4:
		exit(0);
	        break;
	   default :
		puts("wrong choice");
        }
	close(sockfd);
     }   
 }
