#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

int ipcheck(int argc, char **argv);
int portcheck(int argc, char **argv, int portNumber);
int senddata(int sd, char *bufferOut, struct sockaddr_in server_address);		       

int main(int argc, char *argv[])
{
  struct sockaddr_in server_address;  /* structures for addresses */
  char bufferOut [100];
  char serverIP[20]; // provided by the user on the command line */
  int sd; /* the socket descriptor */
  int portNumber = 0; // provided by the user on the command line

  //Error check to see if the right number of parameters were entered.
  if (argc < 3){
    printf("Usage is: ./client0 <IP Address> <Portnumber>\n");
    exit(1);
  }

  //Setting up the convertion for the Portnumber.
  portNumber = strtol(argv[2], NULL, 10); 

  //Calling functions to check both the IP address and Portnumbers used when calling the client.
  ipcheck(argc, argv);
  portcheck(argc, argv, portNumber);

  //Creating the socket.
  sd = socket(AF_INET, SOCK_DGRAM, 0);

  //Checks for errors with the socket.
  if (sd < 0){
    perror("Error, there is a problem with the socket().\n");
    exit(1);
    }

  strcpy(serverIP, argv[1]); /* copy the ip address */
  server_address.sin_family = AF_INET; /* use AF_INET addresses */
  server_address.sin_port = htons(portNumber); /* convert port number */
  server_address.sin_addr.s_addr = inet_addr(serverIP); /* convert IP addr */

  //Clears out the buffer prior to setting our message.
  memset (bufferOut, 0, 100);
  
  //Setting the message into the buffer.
  sprintf (bufferOut, "hello world");

  //Calling the function to send the data and then closing the socket.
  senddata(sd, bufferOut, server_address);

  //Closing the socket.
  close(sd);

  return 0;
}

int ipcheck(int argc, char **argv){
  //Structure for checking addresses. 
  struct sockaddr_in inaddr;

  //Error checking the IP address for invalid entry.
  if (!inet_pton(AF_INET, argv[1], &inaddr)){
    printf("Error, you entered an invalid IP address.\n");
    exit (1);
  }
  return 0;
}

int portcheck(int argc, char **argv, int portNumber){
  //Used as a loop variable.
  int i;

  //Error checking the portnumber that all numbers are digits.
  for (i=0;i<strlen(argv[2]); i++){
    if (!isdigit(argv[2][i]))
      {
	      printf("Error, the portnumber isn't a complete number!\n");
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

int senddata(int sd, char *bufferOut, struct sockaddr_in server_address){
  //Sets the initial return code to 0.
  int rc = 0;

  //This function will send the data to the client.
  rc = sendto(sd, bufferOut, strlen(bufferOut), 0, (struct sockaddr *) &server_address, sizeof(server_address));

  //Check for errors in the sendto() and will print error message.
  if (rc < 0)
    perror("There is a problem with sendto().\n");
    exit(1);

  //This check is to make sure everything was sent over and was recived, if not it will pring out an error message.
  if (rc < strlen(bufferOut)){
    perror("There is a problem with the bytes sent.\n");
    exit(1);
  }
  return 0;
}