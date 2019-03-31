# my ftp server

to compile the code
gcc client.c -o client.o

gcc server.c -o client.o

running the code
first you need to run the server
./server.o <port>

then the client
./client.o <ip> <port> <username>

note
when uploading files to server it will save them inside <server dir>/files/<username>

when entering path to upload in the client you need to write the full path ex: /home/aayyad/Desktop/assignment5/client/file.xml

look at preview.png to see how it works
