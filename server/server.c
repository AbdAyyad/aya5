#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>
#include      <dirent.h>

#include      <unistd.h>
#include      <sys/types.h>
#include      <sys/socket.h>
#include      <sys/stat.h>
#include      <arpa/inet.h>

#define MAXLINE 4096
char cwd[MAXLINE];

void list(int sockfd,char*path){
	DIR * dir = opendir(path);
	struct dirent * de = NULL;
	char result[MAXLINE];
        bzero(result,MAXLINE);
	while ((de = readdir(dir)) != NULL){
		//printf("%s\n", de->d_name);
		strcat(result,de->d_name);
		strcat(result,"\n");	
	} 
        write(sockfd,result,MAXLINE);
	sync();     
	closedir(dir);
}

void upload(int sockfd,char*path){
	char buf[MAXLINE];
	bzero(buf,MAXLINE);
	read(sockfd, buf, MAXLINE);
	printf("buf: %s",buf);
	strcat(path,buf);
	//printf("path:%s\n",path);
	FILE* file = fopen(path,"w");
	
	while(read(sockfd, buf, MAXLINE) != 0){
		printf("buf:%s\n",buf);
		if(strcmp(buf,"./././././././././././././././././") != 0){
			fprintf(file,"%s",buf);	
			fflush(file);	
		} else {
			bzero(buf,MAXLINE);
			puts("end uploading");
			strcpy(buf,"upload done");
			write(sockfd,buf,MAXLINE);
			sync();
			return;
		}
		bzero(buf,MAXLINE);
	}
}

void str_echo(int sockfd){
     	ssize_t n;
     	char    buf[MAXLINE], usr[MAXLINE], path[MAXLINE];
	bzero(buf,MAXLINE);
     	read(sockfd, buf, MAXLINE);
     	
	strcpy(usr,buf);
	
	strcat(path,cwd);
	strcat(path,"/files/");
	strcat(path,usr);
	strcat(path,"/");
	
	DIR *dr = opendir(path); 
  
    	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
   	{ 
  	        mkdir(path, 0777);
		closedir(dr);       
    	}
	bzero(buf,MAXLINE);
	read(sockfd, buf, MAXLINE);
     	
	printf("buf %s\n",buf);
	if(strcmp(buf,"list") == 0){
		list(sockfd,path);	
	} if(strcmp(buf,"upload") == 0){
		upload(sockfd,path);	
	}
	   
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
      getcwd(cwd, sizeof(cwd));
      //printf("%s",cwd);
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
