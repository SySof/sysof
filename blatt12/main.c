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

typedef enum {S0, S1, S2, S3, S4} State;

int main(int argc, char* argv[]){
    cmdname = argv[0];
    if (argc != 2) {
        stralloc usage = {0};
        if (stralloc_copys(&usage, "Usage: ") &&
            stralloc_cats(&usage, cmdname) &&
            stralloc_cats(&usage, " infile \n")) {
            write(2, usage.s, usage.len);
        }
        exit(1);
    }
    char* infile = argv[1];

    int infd = open(infile, O_RDONLY);
    if (infd < 0) die(infile);

    struct stat statbuf;
    if (fstat(infd, &statbuf) < 0) die(infile);

    off_t nbytes = statbuf.st_size;
    char* buf = (char*) mmap(0, nbytes, PROT_READ, MAP_SHARED, infd, 0);
    if (buf == MAP_FAILED) die(infile);



    State state = S0;
    char* bufout = NULL;
    char c = 0;
    stralloc out = {0};
    int print = 0;

    for(ssize_t checked = 0; checked < nbytes; checked++ ){
        c = buf[checked];
        bufout = &buf[checked];
        if(c == '\n' && !print) out.len = 0;
        if(stralloc_append(&out, bufout));
        else exit(2);

        switch (state) {
            case S0:
                if (c == 47) {
                    state = S1;
                    out.len = 0;
                    if(stralloc_append(&out, bufout));
                    else exit(2);
                }
                break;
            case S1:
                if (c == 47) {
                    state = S3;
                    print = 1;
                } else if (c == 42) {
                    state = S2;
                    print = 1;
                } else {
                    state = S0;
                }
                break;
            case S2:
                if (c == 42) {
                    state = S4;
                }
                break;
            case S3:
                if (c == '\n') {
                    state = S0;
                    print = 0;

                }
                break;
            case S4:
                if (c == 47) {
                    state = S0;
                    print = 0;
                    write(1, out.s, out.len);
                    write(1, "\n", 1);
                } else if(c == 42){
                    state = S4;
                } else {
                    state = S2;
                }
                break;
        }
    }
    if (print) write(1, out.s, out.len);

}