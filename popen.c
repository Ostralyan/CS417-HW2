#include <stdio.h>
#define BSIZE 1024 /* buffer size */
         main(int argc, char **argv) {
             FILE *fp;
             char buf[BSIZE];
             if ((fp = popen("ls -lL /etc", "r")) != 0) {
                  /* read the output from the command */
                  while (fgets(buf, BSIZE, fp) != 0)
                      fputs(buf, stdout);
}
             fclose(fp);
         }
