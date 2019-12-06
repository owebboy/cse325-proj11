
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream> // i hate c++ strings
#include <string>
#include <string.h>

#define BSIZE 2048

std::string slurp(std::ifstream& in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == 0) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(8989); // htons is necessary to convert a number to
                                    // network byte order
  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 10) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Grab a connection from the queue
  auto addrlen = sizeof(sockaddr);
  int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
  if (connection < 0) {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  struct hostent *host_info = gethostbyaddr
  (
    (const char *) &sockaddr.sin_addr.s_addr,
    sizeof(sockaddr.sin_addr.s_addr), 
    AF_INET
  );
  if (host_info == NULL)
  {
    perror( "gethostbyaddr" );
    exit( 6 );
  }
  char * host = inet_ntoa( sockaddr.sin_addr );
  if (host == NULL)
  {
    perror( "inet_ntoa" );
    exit( 7 );
  }

  printf( "Server connected to %s (%s)\n\n", host_info->h_name, host );

  while(1)

  {

      // Read from the connection
      char buffer[BSIZE];
      bzero( buffer, BSIZE );

      auto bytesRead = read(connection, buffer, 100);
      std::cout << "The message was: " << buffer << " and read " << bytesRead << " bytes" << std::endl;

      
      if(strcmp(buffer, "quit") == 0)
      {
         // Close the connections
        close(connection);
        close(sockfd);
        return 0;
      }

      // open the buffered filename
      std::ifstream infile(buffer);

      std::string res = slurp(infile);

      send(connection, res.c_str(), res.size(), 0);

  }

  close(connection);
  close(sockfd);
  return 0;

}