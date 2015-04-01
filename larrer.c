#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define GBUF 4

/* "Elaborate macro" section now follows */
#define CHKARGS(a, x); \
    if(a!=(x)+1) { \
        printf("Error. Pls supply %i %s\n", (x), ((x) == 1)? "argument." : "arguments."); \
        exit(EXIT_FAILURE); \
    }

/* Reminder: a is the array whose size is being incremented, and t is the type, a string  */
#define CONDREALLOC(x, b, c, a1, a2, t); \
    if((x)==((b)-1)) { \
        (b) += (c); \
        (a1)=realloc((a1), (b)*sizeof(t)); \
        (a2)=realloc((a2), (b)*sizeof(t)); \
    }

int main(int argc, char *argv[])
{
    CHKARGS(argc, 1);

    FILE *fp=fopen(argv[1], "r");

    size_t cc=0, lc=0, ic=0, glbuf = GBUF;
    size_t *lcca=calloc(glbuf, sizeof(size_t)); /* Line Char Count array */
    size_t *icca=calloc(glbuf, sizeof(size_t)); /* incidence Char Count array */

    int i, c, ccold='\0';

    while((c=fgetc(fp)) != EOF) {
        if (c == '\n') {
            if(ccold!=cc) {
                CONDREALLOC(ic, glbuf, GBUF, icca, lcca, size_t); /*  this turned out to be a fairly important one */
                icca[ic++]=lc;
                lcca[ic-1]=cc;
            }
            lc++;
            ccold=cc;
            cc=0;
        } else
            cc++;
    }
    fclose(fp);
    icca[ic]=lc;
    icca=realloc(icca, (ic+1)*sizeof(size_t));
    lcca=realloc(lcca, ic*sizeof(size_t));

    /*
    printf("Total lines: %zu. Char count / lineidx range:\n", lc);
    for(i=0;i<ic;++i) 
        (icca[i] != (icca[i+1]-1))? printf("%zu: %zu->%zu; ", lcca[i], icca[i], icca[i+1]-1) : printf("%zu: %zu; ", lcca[i], icca[i]); 
    printf("\n"); 
    */
    printf("Total lines in file: %zu. Line-char count: sequential count pairs:\n", lc);
    for(i=0;i<ic;++i) 
        printf("%zu:%zu ", lcca[i], icca[i+1]-icca[i]);
    printf("\n"); 

    free(lcca);
    free(icca);

    return 0;
}
