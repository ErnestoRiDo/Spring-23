#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

int portcheck(int argc, char **argv, int portNumber);
int bindsocket(int sd, int rc, struct sockaddr_in server_address);
int recvdata(int sd, int rc, char *bufferReceived, socklen_t fromLength);

int main(int argc, char *argv[])
{
  struct sockaddr_in server_address; /* my address */ /* address of sender */
  socklen_t fromLength;
  fromLength = sizeof(struct sockaddr_in);
  char bufferReceived[1000]; // used in recvfrom
  int sd; /* socket descriptor */
  int portNumber; // get this from command line
  int rc = 0; // always need to check return codes!

  /* first, decide if we have the right number of parameters */
  if (argc < 2){
    printf("Usage is: ./server0 <Portnumber>\n");
    exit (1);
  }
  //Setting up the convertion for the Portnumber.
  portNumber = strtol(argv[1], NULL, 10);

  //Calling functions to check Portnumbers used when calling the server.
  portcheck(argc, argv, portNumber);

  //Creates a socket.
  sd = socket(AF_INET, SOCK_DGRAM, 0); 

  //Checks for errors when creating the socket.
  if (sd == -1){ 
    perror ("Error, there has been en problem with the socket().\n");
    exit(1);
  }
  server_address.sin_family = AF_INET; /* use AF_INET addresses */
  server_address.sin_port = htons(portNumber); /* convert port number */
  server_address.sin_addr.s_addr = INADDR_ANY; /* any adapter */
  
  //Calls out the function to bind the socket from the client to our server.
  bindsocket(sd, rc, server_address);

  //Zeros out the buffer before receiving the message. Then calls the function to retrieve the message.
  memset (bufferReceived, 0, 1000);
  recvdata(sd, rc, bufferReceived, fromLength);

  //Prints out what was sent over by the client.
  printf("The server received the following message: '%s'\n", bufferReceived);

  return 0;
}

int portcheck(int argc, char **argv, int portNumber){
  //Used as a loop variable.
  int i;

  //Error checking the portnumber that all numbers are digits.
  for (i=0;i<strlen(argv[1]); i++){
    if (!isdigit(argv[1][i]))
      {
	      printf ("Error, the Portnumber isn't a complete number!\n");
	      exit(1);
      }
  }

  //Error checking the portnumber for invalid numbers.
  if ((portNumber > 65535) || (portNumber < 0)){
    printf("Error, you entered an invalid portnumber.\n");
    exit (1);
  }
  return 0;
}

int bindsocket(int sd, int rc, struct sockaddr_in server_address){
  //This is going to bind the socket.
  rc = bind(sd, (struct sockaddr *)&server_address, sizeof(struct sockaddr));

  //Checks for errors with the binding of the socket.
  if (rc < 0){
    perror("Error, there is an issue with bindsocket().\n");
    exit (1);
  }
  return 0;
}

int recvdata(int sd, int rc, char *bufferReceived, socklen_t fromLength){
  //Structures for our address
  struct sockaddr_in from_address; 

  //Flag set for future implementation in the recvfrom function.
  int flags = 0;

  //This is where we recieve the message from the client.
  rc = recvfrom(sd, bufferReceived, 1000, flags, (struct sockaddr *)&from_address, &fromLength);

  //Checks for errors in the recvdata.
  if (rc < 0){
    printf ("Error, there is a problem with recvfrom().\n");
    exit (1);
  }
  return 0;
}