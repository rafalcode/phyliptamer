#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define GBUF 4
#define NMBUF 10

/* "Elaborate macro" section now follows */
#define CHKARGS(a, x); \
    if(a!=(x)+1) { \
        printf("Error. Pls supply %i %s\n", (x), ((x) == 1)? "argument." : "arguments."); \
        exit(EXIT_FAILURE); \
    }

/* Reminder: a is the array whose size is being incremented, and t is the type, a string  */
#define CONDREALLOC(x, b, c, a, t); \
    if((x)==((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
    }

int main(int argc, char *argv[])
{
    CHKARGS(argc, 1);

    int i, c;
    FILE *fp=fopen(argv[1], "r");
    unsigned txa, cols, conli=0 /* char on line index */, li=0 /* line number index */;
    fscanf(fp, "%u%u\n", &txa, &cols);
    printf("txa=%u/cols=%u\n", txa, cols);
    char **nms=malloc(txa*sizeof(char*));
    for(i=0;i<txa;++i) 
        nms[i]=calloc(NMBUF, sizeof(char));
    char cold='\0';
    char seen1spa=0;

    printf("l_%u:", li);
    unsigned sli=0;/* sequence on line index */
    while((c=fgetc(fp)) != EOF) {
        if(c == ' ') {
            if(!seen1spa) {
                seen1spa=1;
                sli=0;
            }
        } else if(c != '\n') {
            if(seen1spa) 
                printf("%i ", sli);
            else if(li<txa) {
                printf("%i", conli);
               nms[li][conli]=c;
            }
            conli++;
            sli++;
        } else if ( (c == '\n') && (cold != '\n') ) {
            conli=0;
            li++;
            printf("\nl_%u:", li);
            seen1spa=0;
        }
        cold=c;
    }
    fclose(fp);

    printf("Total lines in file: %u.\n", li);

    for(i=0;i<txa;++i) 
        printf("%s ", nms[i]);
    printf("\n"); 
    for(i=0;i<txa;++i) 
        free(nms[i]);
    free(nms);
    return 0;
}
