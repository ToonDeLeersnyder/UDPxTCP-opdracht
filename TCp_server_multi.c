//gcc -Wall -pedantic TCp_server.c -l ws2_32 -o TCPserver
#ifdef _WIN32
	#define _WIN32_WINNT _WIN32_WINNT_WIN7
	#include <winsock2.h> //for all socket programming
	#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
	#include <stdio.h> //for fprintf, perror
	#include <unistd.h> //for close
	#include <stdlib.h> //for exit
	#include <string.h> //for memset
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
	void OSInit( void ) {}
	void OSCleanup( void ) {}
#endif
#define lengthName 20
int initialization();
int connection( int internet_socket );
void* execution( void * buffer_pointer );
void cleanup( int internet_socket, int client_internet_socket );
char userSocket[5];
int socketCLientsConnected[5];

int main( int argc, char * argv[] )
{
	//////////////////
	//Initialization//
	//////////////////

	OSInit();
	printf("give the socket you want to listen to: ");
	scanf("%s",userSocket);

	int internet_socket = initialization();

	//////////////
	//Connection//
	//////////////

	int client_internet_socket = connection( internet_socket );

	/////////////
	//Execution//
	/////////////



	////////////
	//Clean up//
	////////////

	cleanup( internet_socket, client_internet_socket );

	OSCleanup();

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
	internet_address_setup.ai_flags = AI_PASSIVE;
	
	
	int getaddrinfo_return = getaddrinfo( NULL, userSocket, &internet_address_setup, &internet_address_result );
	if( getaddrinfo_return != 0 )
	{
		fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( getaddrinfo_return ) );
		exit( 1 );
	}

	int internet_socket = -1;
	struct addrinfo * internet_address_result_iterator = internet_address_result;
	while( internet_address_result_iterator != NULL )
	{
		
		internet_socket = socket( internet_address_result_iterator->ai_family, internet_address_result_iterator->ai_socktype, internet_address_result_iterator->ai_protocol );
		if( internet_socket == -1 )
		{
			perror( "socket" );
		}
		else
		{
			
			int bind_return = bind( internet_socket, internet_address_result_iterator->ai_addr, internet_address_result_iterator->ai_addrlen );
			if( bind_return == -1 )
			{
				perror( "bind" );
				close( internet_socket );
			}
			else
			{
				
				int listen_return = listen( internet_socket, 5 );
				if( listen_return == -1 )
				{
					close( internet_socket );
					perror( "listen" );
				}
				else if (listen_return == 0)
				{
					printf("listening\n");
					break;
				}
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

int connection( int internet_socket )
{
	struct sockaddr_storage client_internet_address;
	socklen_t client_internet_address_length = sizeof client_internet_address;
	int con = 0;
	int client_socket = 0;
	pthread_t ClientThread;
	for (;;)
	{
		
		
		
		
		
		client_socket = accept( internet_socket, (struct sockaddr *) &client_internet_address, &client_internet_address_length );
		if( client_socket == -1 )
		{
			perror( "accept" );
			close( internet_socket );
			exit( 3 );
		}
		
			socketCLientsConnected[con]= client_socket;
			con++;
			printf("connection accepted: %d", con);		
			pthread_create(&ClientThread, NULL, execution, &client_socket);	
			
		
		
		
		
	}
	
}

void stringTrim (char* array, int length) 
{
  int i;
  for (i = 0; i < length; i++) 
  { // change \n to \0
    if (array[i] == '\n') 
	{
      array[i] = '\0';
      break;
    }
  }
}

void prefix() // prefix
{ 
    printf("\r%s", "- ");
    fflush(stdout);
}
void * execution( void * buffer_pointer )
{
	int leave_flag = 0;
	int number_of_bytes_received = 0;
	char messageBuffer[200];
	char bufferName[1000];
	int clientSockT = *((int *)buffer_pointer);
	while( (number_of_bytes_received = recv( clientSockT, bufferName, ( sizeof bufferName ) - 1, 0 )) > 0)
	{
		
		
		// number_of_bytes_received = recv( clientSockT, bufferName, ( sizeof bufferName ) - 1, 0 );
		// if( number_of_bytes_received == -1 )
		// {
			
			// perror( "recv" );
		// }
		
		
			bufferName[number_of_bytes_received] = '\0';
			printf( " %s\n", bufferName );
			char user[20];
			strcpy(user, bufferName);
			
			for (int i = 0; i < 2; i++)
				{
					if (!(clientSockT == socketCLientsConnected[i]))
					{
						send( socketCLientsConnected[i], (char *) bufferName, strlen(bufferName), 0 );
					}
					
				}
				prefix();
				stringTrim(messageBuffer, strlen(messageBuffer));
				
				
		

	
	
		// if (leave_flag) 
		// {
			// break;
		// }
		
		// int message_recieved = recv( clientSockT, messageBuffer, ( sizeof messageBuffer ) - 1, 0 );
		// if( message_recieved == -1 )
		// {
		
		// if( message_recieved <= -1 )
		// {
			// sprintf(messageBuffer, "%s has left\n", bufferName);
			// printf("lol");
			// perror( "recv" );
		// }
		// else if (message_recieved == 0 || (strstr(messageBuffer, "#exit")) != 0)
		// {
			// sprintf(messageBuffer, "%s has left\n", bufferName);
			// printf("%s", messageBuffer);
			// printf("lol2");
			// send( clientSockT, messageBuffer, strlen(messageBuffer), 0 );
			// break;
		// }
		// else if (message_recieved > 0)
		// {
			// if(strlen(messageBuffer) > 0){
				
				
				// for (int i = 0; i < 5; i++)
				// {
					
					// send( socketCLientsConnected[i], messageBuffer, strlen(messageBuffer), 0 );
					
				// }
				// prefix();
				// stringTrim(messageBuffer, strlen(messageBuffer));
				
				// printf("%s \n", messageBuffer);
			// }
			
		// }
		
		
		// }
		
	}
	
				
	
	
	bzero(bufferName, lengthName );
	return 0;
}

void cleanup( int internet_socket, int client_internet_socket )
{
	//Step 4.2
	// int shutdown_return = shutdown( client_internet_socket, SD_RECEIVE );
	// if( shutdown_return == -1 )
	// {
		// perror( "shutdown" );
	// }

	//Step 4.1
	close( client_internet_socket );
	close( internet_socket );
}