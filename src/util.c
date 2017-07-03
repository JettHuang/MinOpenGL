/* \brief
 *      implement 
 *
 *
 */

#include <windows.h>
#include <stdio.h>


static LARGE_INTEGER last;

/* \brief
 *      begine to performance statis
 */
void begin_performance()
{
    QueryPerformanceCounter(&last);
}

/* \brief
 *      end to ...
 */
float end_performance(const char *comment)
{
    LARGE_INTEGER now;
    LARGE_INTEGER freq;
    double interval, sec;

    QueryPerformanceCounter(&now);
    interval = (now.QuadPart - last.QuadPart);
    if (!QueryPerformanceFrequency(&freq))
        return 0.f;

    sec = interval / (double)freq.QuadPart;
    
    if (comment) printf("%s use time %lf seconds\n", comment, sec);

    return 1.f / sec; /* return fps */
}
