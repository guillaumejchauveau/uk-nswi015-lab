/*
 * The program reads data from stdin and sends it over to the remote side.
 *
 * Set SO_LINGER socket option and observe the behavior by dumping traffic, e.g.
 * via tcpdump(1).
 *
 * (c) vlada@devnull.cz
 */

#define _XOPEN_SOURCE 700

#include <err.h>
#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void usage(char *argv0) {
  errx(1, "usage: %s [-L <value>] <addr> <port_number>", argv0);
}

int main(int argc, char **argv) {
  char buf[100];
  int fd, error, opt;
  int linger = -1;
  size_t n;
  struct addrinfo *res, *resorig, hints;
  char *argv0 = basename(argv[0]);
  struct linger lstr;

  while ((opt = getopt(argc, argv, "L:")) != -1) {
    switch (opt) {
    case 'L':
      linger = atoi(optarg);
      break;
    case '?':
      warn("unknown option: '%c'", optopt);
      usage(argv0);
      break;
    }
  }

  argv += optind;
  argc -= optind;

  if (argc != 2)
    usage(argv0);

  printf("%s %s\n", argv[0], argv[1]);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((error = getaddrinfo(argv[0], argv[1], &hints, &res)) != 0)
    errx(1, "%s", gai_strerror(error));

  for (resorig = res; res != NULL; res = res->ai_next) {
    if ((fd = socket(res->ai_family, SOCK_STREAM, 0)) == -1)
      err(1, "socket");

    printf("got address\n");
    break;
  }

  if (res == NULL)
    errx(1, "no address");

  if (linger >= 0) {
    printf("setting linger\n");
    lstr.l_onoff = 1;
    lstr.l_linger = linger;

    if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &lstr, sizeof(lstr)) == -1) {
      err(1, "setsockopt");
    }
  }

  if (connect(fd, res->ai_addr, res->ai_addrlen) < 0)
    err(1, "connect");

  freeaddrinfo(resorig);

  while ((n = read(0, buf, sizeof(buf))) > 0) {
    printf("got %zd bytes\n", n);
    write(fd, buf, n);
  }

  printf("closing\n");
  if (close(fd) != 0)
    err(1, "close");

  printf("exiting\n");
  return (0);
}
