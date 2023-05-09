#include "orchestrator.h"

// Collects any terminated child processes to prevent them from becoming zombies
void sigchld_handler(int s)
{
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  // Collect any terminated child processes using waitpid() with WNOHANG option
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;

  errno = saved_errno;
}

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Setup socket for connection
int setup_socket(struct addrinfo *servinfo)
{
  int sockfd;
  struct addrinfo *p;
  int yes = 1;

  // Loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    // Create a new socket
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
                         p->ai_protocol)) == -1)
    {
      perror("server: socket");
      continue;
    }

    // Allow the socket to reuse the address
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                   sizeof(int)) == -1)
    {
      perror("setsockopt");
      return 1;
    }

    // Bind the socket to the specified address and port number
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      // if the bind fails, close the socket and print an error message
      close(sockfd);
      perror("server: bind");
      continue;
    }

    // If we successfully bind to an address, break out of the loop
    break;
  }

  // Retunr 1 if the bind failed
  if (p == NULL)
  {
    fprintf(stderr, "server: failed to bind\n");
    return 1;
  }

  // Return the socket file descriptor
  return sockfd;
}

// Create a shared memory segment
int create_shared_memory_buffer()
{
  int shmid = shmget(IPC_PRIVATE, SESSION_TOKEN_LENGTH + 1, IPC_CREAT | 0666);

  if (shmid < 0)
  {
    perror("Error creating shared memory segment");
    return -1;
  }

  // Assign the shared memory segment to expected_session_token global variable
  expected_session_token = (char *)shmat(shmid, NULL, 0);

  if (expected_session_token == (char *)-1)
  {
    perror("Error assigning shared memory segment to variable");
    return -1;
  }
  return 0;
}
