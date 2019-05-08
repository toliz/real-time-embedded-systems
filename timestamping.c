#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_RESET    "\x1b[0m"

#define ANSI_BOLD   "\e[1m"
#define ANSI_NORMAL "\e[0m"
#define ANSI_ITALIC "\e[3m"

int n = 0;
float duration, interval, *timestamps;

struct itimerval timer;
struct timeval start, end;

void timestamper(int signum) {
    gettimeofday(&end, NULL);

    timestamps[n++] = (end.tv_sec - start.tv_sec)* 1000 + (float) (end.tv_usec - start.tv_usec) / 1000;
}


void timer_handler (int signum)
{
 static int count = 0;
 printf ("timer expired %d times\n", ++count);
}

int main (int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, ANSI_COLOR_RED "\nIncorrect usage!\n" ANSI_COLOR_RESET "Please use: " );
        fprintf(stderr, ANSI_BOLD "%s" ANSI_ITALIC " <method> <duration> <interval-size>\n\n", argv[0]);
        return -1;
    }
    
    /* Initialization */
    duration = atof(argv[2]);
    interval = atof(argv[3]);
    timestamps = (float*) malloc(sizeof(float) * (int) (duration/interval + 1));

    /* Start counting... */
    
    if (strncasecmp(argv[1], "method-1", 8) == 0) {
        /* Method 1 */
        printf("\nStarted counting...\n\n");

        gettimeofday(&start, NULL);

        while (1) {
            usleep( (__useconds_t) (interval * 1000000) );

            gettimeofday(&end, NULL);
            timestamps[n] = (end.tv_sec - start.tv_sec)* 1000 + (float) (end.tv_usec - start.tv_usec) / 1000;

            if (timestamps[n++] > duration * 1000)
                break;
        }
    } else if (strncasecmp(argv[1], "method-2", 8) == 0) {
        /* Method 2 */
        printf("\nStarted counting...\n\n");

        gettimeofday(&start, NULL);

        struct sigaction sa;
        memset (&sa, 0, sizeof (sa));
        sa.sa_handler = &timestamper;
        sigaction (SIGALRM, &sa, NULL);

        timer.it_value.tv_sec = (__useconds_t) interval;
        timer.it_value.tv_usec = (__useconds_t) ((interval - (int) interval) * 1000000);

        timer.it_interval.tv_sec = (__useconds_t) interval;
        timer.it_interval.tv_usec = (__useconds_t) ((interval - (int) interval) * 1000000);

        setitimer (ITIMER_REAL, &timer, NULL);
        
        while (timestamps[n-1] < duration*1000) {
            sleep((int) interval+1);
        }

        // set zero timer to cancel it
        timer.it_interval.tv_usec = 0;
        setitimer (ITIMER_REAL, &timer, NULL);
    } else {
        fprintf(stderr, ANSI_COLOR_RED "\nIncorrect method!\n" ANSI_COLOR_RESET "Please use either: ");
        fprintf(stderr, ANSI_ITALIC "method-1" ANSI_NORMAL " or " ANSI_ITALIC "method-2\n\n");
        return -1;
    }

    /* Save in CSV format */
    printf("Saving in report.txt...\n\n");

    FILE *file;
    if (strncasecmp(argv[1], "method-1", 8) == 0) {
        file = fopen("timestamping-method-1.txt", "w");
    } else {
        file = fopen("timestamping-method-2.txt", "w");
    }

    fprintf(file, "%.3f, ", timestamps[0]);
    for (int i = 1; i < n-1; i++) {
        fprintf(file, "%.3f, ", timestamps[i] - timestamps[i-1]);
    }
    fprintf(file, "%.3f", timestamps[n-1] - timestamps[n-2]);

    fclose(file);

    printf("Finished! Saved %d timestamps\n\n", n);

    return 0;
}