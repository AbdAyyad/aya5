#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>

#include      <unistd.h>
#include      <sys/types.h>
#include      <sys/socket.h>
#include      <sys/stat.h>
#include      <arpa/inet.h>

#define MAXLINE 4096

//mkdir("/home/me/test.txt", 0777);

/*
struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(dr);     
*/

void str_echo(int sockfd){
     ssize_t n;
     char    buf[MAXLINE];
     read(sockfd, buf, MAXLINE);
     puts(buf);
     read(sockfd, buf, MAXLINE);
     puts(buf);

   
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
