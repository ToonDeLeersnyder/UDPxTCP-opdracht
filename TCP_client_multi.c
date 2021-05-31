//compile
//gcc -Wall -pedantic TCP_client.c -l ws2_32 -lpthread -o TCP_client

#ifdef _WIN32
	#define _WIN32_WINNT _WIN32_WINNT_WIN7
	#include <winsock2.h> //for all socket programming
	#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
	#include <stdio.h> //for fprintf, perror
	#include <unistd.h> //for close
	#include <stdlib.h> //for exit
	#include <string.h> //for memset
	#include <strings.h> 	//for memset
	#include <pthread.h>
	void OSInit( void )
	{
		WSADATA wsaData;
		int WSAError = WSAStartup( MAKEWORD( 2, 0 ), &wsaData ); 
		if( WSAError != 0 )
		{
			fprintf( stderr, "WSAStartup errno = %d\n", WSAError );
			exit( -1 );
		}
	}
	void OSCleanup( void )
	{
		WSACleanup();
	}
	#define perror(string) fprintf( stderr, string ": WSA errno = %d\n", WSAGetLastError() )
#else
	#include <sys/socket.h> //for sockaddr, socket, socket
	#include <sys/types.h> //for size_t
	#include <netdb.h> //for getaddrinfo
	#include <netinet/in.h> //for sockaddr_in
	#include <arpa/inet.h> //for htons, htonl, inet_pton, inet_ntop
	#include <errno.h> //for errno
	#include <stdio.h> //for fprintf, perror
	#include <unistd.h> //for close
	#include <stdlib.h> //for exit
	#include <string.h> //for memset
	#include <pthread.h>
	#include <strings.h> 
	void OSInit( void ) {}
	void OSCleanup( void ) {}
#endif
#define length 500

int initialization();
void execution( int internet_socket );
void cleanup( int internet_socket );
char name[20];
char userSocket[5];
int internet_socket = -1;

char ipAdress[20];
char name[20];
int main( int argc, char * argv[] )
{
	//////////////////
	//Initialization//
	//////////////////

	OSInit();
	printf("give the ip address to connect to: ");
	scanf("%s",ipAdress);
	printf("give the  socket you want to listen to: ");
	scanf("%s",userSocket);
	printf("Enter user name: ");  
	scanf("%s",name);
	system("cls");	
    // fgets(name, 30, stdin);
	// stringTrim(name, strlen(name));
	
	int internet_socket = initialization();

	/////////////
	//Execution//
	/////////////

	execution( internet_socket );


	////////////
	//Clean up//
	////////////

	cleanup( internet_socket );

	OSCleanup();

	return 0;
}
void stringTrim (char* arr, int lengte) 
{
  int i;
  for (i = 0; i < lengte; i++) 
  { // trim \n
    if (arr[i] == '\n') 
	{
      arr[i] = '\0';
      break;
    }
  }
}
void prefix() 
{
  printf("%s", "           - ");
  fflush(stdout);
}
void prefixUser() 
{
  printf("%s", "> ");
  fflush(stdout);
}

 void * sendMsgH()
 {
	 char message[length + 30] = {0};
	 char buffer[length] = {0};
	 while (1)
	 {
		
		fgets(buffer, length, stdin);
		stringTrim(message, length);
		if (strcmp(buffer, "exit") == 0)
		{
			break;
		}
		else
		{
			sprintf(message, " %s: %s \r ", name, buffer);
			//printf("%s\n", message);
			send(internet_socket, message, strlen(message), 0);
		}
		bzero(message, length + 20);
		bzero(buffer, length);
		
	 }
	
	 return 0;
 }
  void  *recieveMsgH(void * arg)
 {
	 char message[length + 20] = {0};
	 while(1) 
	 {
		int recieve = recv(internet_socket, message, length, 0);
		if (recieve > 0) 
		{
			stringTrim(message, length);
			prefix();
			printf("%s", message);
			fprintf( stdout, "\n" );
			
		}
		else if (recieve == 0)
		{
			break;
		} 
		memset(message, 0, sizeof(message));
	}
		
	 	
	 return 0;	 
 }
int initialization()
{
	//Step 1.1
	struct addrinfo internet_address_setup;
	struct addrinfo * internet_address_result;
	memset( &internet_address_setup, 0, sizeof internet_address_setup );
	internet_address_setup.ai_family = AF_UNSPEC;
	internet_address_setup.ai_socktype = SOCK_STREAM;
	int getaddrinfo_return = getaddrinfo( ipAdress, userSocket, &internet_address_setup, &internet_address_result );
	if( getaddrinfo_return != 0 )
	{
		fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( getaddrinfo_return ) );
		exit( 1 );
	}

	
	struct addrinfo * internet_address_result_iterator = internet_address_result;
	while( internet_address_result_iterator != NULL )
	{
		//Step 1.2
		internet_socket = socket( internet_address_result_iterator->ai_family, internet_address_result_iterator->ai_socktype, internet_address_result_iterator->ai_protocol );
		if( internet_socket == -1 )
		{
			perror( "socket" );
		}
		else
		{
			//Step 1.3
			int connect_return = connect( internet_socket, internet_address_result_iterator->ai_addr, internet_address_result_iterator->ai_addrlen );
			if( connect_return == -1 )
			{
				perror( "connect" );
				close( internet_socket );
				exit(0);
			}
			else
			{
				printf("connected to : %s\n", ipAdress);
				break;
			}
		}
		internet_address_result_iterator = internet_address_result_iterator->ai_next;
	}

	freeaddrinfo( internet_address_result );

	if( internet_socket == -1 )
	{
		fprintf( stderr, "socket: no valid socket address found\n" );
		exit( 2 );
	}

	return internet_socket;
}

void execution( int internet_socket )
{
	//Step 2.1
	send (internet_socket, name , 30, 0);
	printf("Welcome to chatroom\n");
	
	
	pthread_t sendMessageT;
	
	if( pthread_create(&sendMessageT, NULL,  sendMsgH, NULL) == -1 )
	{
		perror( "pthread" );
	}
	
	pthread_t recieveMessageT;
	
	if( pthread_create(&recieveMessageT, NULL, recieveMsgH, NULL) == -1 )
	{
		perror( "pthread" );
	}
	
	while(1)
	{
		
		
		
	}
	
	
	
}

void cleanup( int internet_socket )
{
	//Step 3.2
	int shutdown_return = shutdown( internet_socket, SD_SEND );
	if( shutdown_return == -1 )
	{
		perror( "shutdown" );
	}

	//Step 3.1
	close( internet_socket );
}