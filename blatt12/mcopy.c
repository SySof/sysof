#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stralloc.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

char* cmdname;

void die(char* filename) {
   stralloc msg = {0};
   if (stralloc_copys(&msg, cmdname) &&
	 stralloc_cats(&msg, ": ") &&
	 stralloc_cats(&msg, strerror(errno)) &&
	 stralloc_cats(&msg, ": ") &&
	 stralloc_cats(&msg, filename) &&
	 stralloc_cats(&msg, "\n")) {
      write(2, msg.s, msg.len);
   }
   exit(1);
}

int main(int argc, char* argv[]) {
   cmdname = argv[0];
   if (argc != 3) {
      stralloc usage = {0};
      if (stralloc_copys(&usage, "Usage: ") &&
	    stralloc_cats(&usage, cmdname) &&
	    stralloc_cats(&usage, " infile outfile\n")) {
	 write(2, usage.s, usage.len);
      }
      exit(1);
   }
   char* infile = argv[1]; char* outfile = argv[2];
    // hlasdidf
   int infd = open(infile, O_RDONLY);
   if (infd < 0) die(infile);
   int outfd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
   if (outfd < 0) die(outfile);
 /*
   struct stat statbuf; if (fstat(infd, &statbuf) < 0) die(infile);
   off_t nbytes = statbuf.st_size;
   char* buf = (char*) mmap(0, nbytes, PROT_READ, MAP_SHARED, infd, 0);
   if (buf == MAP_FAILED) die(infile);
   ssize_t count;****/
   for (ssize_t written = 0; written < nbytes; written += count) {
      count = write(outfd, buf + written, nbytes - written);
      if (count <= 0) die(outfile);
   }
   close(infd);
   if (close(outfd) < 0) die(outfile);
}
