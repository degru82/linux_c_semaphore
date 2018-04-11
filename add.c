#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


int main (void) {
    const char *filename = "number.txt" ;
    pid_t pid[2] = {0};
    sem_t sem;

    // sem_init (sem, 0, 1); // This is for Linux Systems
    sem = sem_open ("/semaphore", O_CREAT, 0644, 1); // This is for OS X systems
    perror ("sem_open");

    int i = 0;
    for (i=0; i<2; i++) {
        pid[i] = fork();

        printf ("%d\n", pid[i]);
        if (pid[i] > 0)
            continue;

        while (1) {
            sem_wait (sem);

            FILE *fp = NULL;
            char buf [10] = {0};
            int parsed = 0;

            fp = fopen (filename, "r");
            parsed = atoi (fgets (buf, 10, fp));
            fclose (fp);

            parsed ++;

            printf ("%#08x/PID - %d/NUM\n", getpid(), parsed);
            fflush (stdout);

            fp = fopen (filename, "w");
            fprintf (fp, "%d", parsed);
            fclose (fp);

            sem_post (sem);

            sleep(1);
        }
        
    }

    while (1)
        sleep(1);

    return 0;
}