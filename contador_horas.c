#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h> 

unsigned long get_current_time_in_millis() {
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    return (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
}

int main() {
    unsigned long firstTime = 0;
    int horas   = 0;
    int minutos = 0;
    int segundo = 0;
    
    while (true) {
        unsigned long atualTime = get_current_time_in_millis();

        if (atualTime - firstTime >= 1000) {
            firstTime = atualTime;
            
            segundo++;
            if (segundo == 60) {
                segundo = 0;
                minutos++;
            }
            if (minutos == 60) {
                minutos = 0;
                horas++;
            }
            if (horas == 24) {
                horas = 0;
            }
            system("clear");
            
            printf("%d:%d:%d\n", horas, minutos, segundo);
        }
    }
    return 0;
}
