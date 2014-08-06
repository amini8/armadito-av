#include <libumwsu/scan.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static void usage(int argc, char **argv)
{
  fprintf(stderr, "usage: %s [-r] FILE|DIR ...\n", argv[0]);
  exit(1);
}

int main(int argc, char **argv)
{
  struct umw *u;
  int argp = 1;
  int recurse = 0;
  
  if (argc < 2)
    usage(argc, argv);

  if (!strcmp(argv[argp], "-r")) {
    argp++;
    recurse = 1;
  }

  u = umw_open();

  while (argp < argc) {
    struct stat sb;
    enum umw_status status;

    if (stat(argv[argp], &sb) == -1) {
      perror("stat");
      exit(EXIT_FAILURE);
    }

    if (S_ISDIR(sb.st_mode))
      umw_scan_dir(u, argv[argp], recurse);
    else
      status = umw_scan_file(u, argv[argp]);

    argp++;
  }

  umw_close(u);
}
