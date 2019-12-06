#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h> // For read
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>


#include <string.h>
#include <sys/types.h>

#define BSIZE 2048

int sd = socket( AF_INET, SOCK_STREAM, 0 );


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

int connectToServer(std::string hostname, int port);
int sendRecieve(std::string remote_file, std::string client_file);


int main(int argc, char *argv[])
{
    std::string host;
    int port = 0;\

    if(argc < 3)
    {
        std::cout << "usage: ./proj11.client hostname port" << std::endl;
        exit(-1);
    }

    if (sd < 0)
    {
        perror( "socket" );
        exit( 2 );
    }

    for(int i = 0; i < argc; i++)
    {
        if(i==1)
            host = argv[i];
        else if(i==2)
            port = atoi(argv[i]);
    }

    // connect to server
    connectToServer(host, port);

    // begin infinite loop
    while(1)
    {
        std::string cmd;

        std::cout << "olivernet> ";

        std::getline (std::cin,cmd);

        // tokenize string
        std::vector<std::string> results = split(cmd, ' ');

        if(results[0] == "quit")
        {
            sendRecieve("quit", "null");
            exit(0);
        }
            
        else if(results[0] == "get")
        {
            if(results.size() == 3)
            {
                std::cout << results.size() << std::endl;

                // okay connect to the server with our files
                sendRecieve(results[1], results[2]);


            }
            else
            {
                std::cout << "usage: get remote-file local-file" << std::endl;

            }
        }
        
        


    }
   close( sd );

}

int connectToServer(std::string hostname, int port)
{
    
    /*LECTURE CODE*/
   

    struct hostent * server = gethostbyname( hostname.c_str() );
    if (server == NULL)
    {
        printf( "Error: no such host %s\n", hostname.c_str() );
        exit( 3 );
    }

    struct sockaddr_in saddr;

    bzero( &saddr, sizeof(saddr) );
    saddr.sin_family = AF_INET;
    bcopy( server->h_addr, &saddr.sin_addr.s_addr, server->h_length );
    saddr.sin_port = htons( port );

    int stat = connect( sd, (struct sockaddr *) &saddr, sizeof(saddr) );
    if (stat < 0)
    {
        perror( "connect" );
        exit( 4 );
    }

    printf( "Server connected on fd %d\n\n", sd );

    return 0;
}

int sendRecieve(std::string remote_file, std::string client_file)
{
    int nsend = send( sd, remote_file.c_str(), strlen(remote_file.c_str()), 0 );
    if (nsend < 0) 
    {
        perror( "send" );
        exit( 5 );
    }

    if(client_file == "null")
        return 0;

    char buffer [BSIZE];
    bzero( buffer, BSIZE );
    int nrecv = recv( sd, buffer, BSIZE, 0 );
    if (nrecv < 0) 
    {
        perror( "recv" );
        exit( 4 );
    }

    std::string response = buffer;

    std::ofstream out(client_file);
    out << response;
    out.close();


    printf( "Client received %d bytes\n", nrecv );
    printf( "Message: %s\n", buffer );

 
    return 0;
}