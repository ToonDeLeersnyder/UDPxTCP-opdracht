# UDPxTCP-opdracht
## compile

UDP client : gcc -Wall -pedantic UDP_client.c -l ws2_32 -o UDP_client  
UDP server : gcc -Wall -pedantic UDP_server.c -l ws2_32 -o UDP_server (linux)  
/  
TCP client : gcc -Wall -pedantic TCP_client.c -l ws2_32 -lpthread -o TCP_client  
TCP server : gcc -Wall -pedantic TCp_server.c -l -o TCPserver (linux)
