#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

// setting the number of threads:
#ifndef NUMT
#define NUMT      8
#endif

// setting the number of nodes:
#ifndef NUMNODES
#define NUMNODES  128
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTIMES
#define NUMTIMES  20
#endif

float Height( int, int ); // function prototype

int main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
    fprintf(stderr, "No OpenMP support detected.\n");
    return 1;
#endif

    omp_set_num_threads( NUMT );

    double maxPerformance = 0.;
    double megaHeightsPerSecond = 0.;
    float volume = 0.;

    // the area of a single full-sized tile:
    // (not all tiles are full-sized, though)

    float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
                            ( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

    for( int times = 0; times < NUMTIMES; times++ )
    {
        volume = 0.;
        double time0 = omp_get_wtime( );

        // sum up the weighted heights into the variable "volume"
        // using an OpenMP for-loop and a reduction:
        #pragma omp parallel for collapse(2) default(none), shared(fullTileArea), reduction(+:volume) 
        for( int iv = 0; iv < NUMNODES; iv++ )
        {
            for( int iu = 0; iu < NUMNODES; iu++ )
            {
                float z = Height( iu, iv );
                
                bool _iu_edge = iu == 0 || iu == NUMNODES - 1; // True if iu is on an edge
                bool _iv_edge = iv == 0 || iv == NUMNODES - 1; // True if iv is on an edge
                
                if ( _iu_edge && _iv_edge ) // height-pins on a corner
                    volume += z * (fullTileArea / 4.);
                else if ( _iu_edge || _iv_edge ) // height-pins on an edge
                    volume += z * (fullTileArea / 2.);
                else // height-pins on a full tile
                    volume += z * fullTileArea;
            }
        }
        
        double time1 = omp_get_wtime( );
        double megaHeightsPerSecond = (double)(NUMNODES * NUMNODES) / ( time1 - time0 ) / 1000000.;
        if ( megaHeightsPerSecond > maxPerformance )
            maxPerformance = megaHeightsPerSecond; 
    }

    // fprintf(stderr, "%2d threads : %8d nodes ; volume = %6.2f ; megaheights/sec = %6.2lf\n", 
    // NUMT, NUMNODES * NUMNODES, volume, maxPerformance);
    fprintf(stderr, "%2d,%8d,%6.2f,%6.2lf\n", 
            NUMT, NUMNODES * NUMNODES, volume * 2, maxPerformance);
}

// The code to evaluate the height at a given iu and iv is:

const float N = 2.5f;
const float R = 1.2f;

float
Height( int iu, int iv ) // iu,iv = 0 .. NUMNODES-1
{
    float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1); // -1. to +1.
    float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1); // -1. to +1.

    float xn = pow( fabs(x), (double)N );
    float yn = pow( fabs(y), (double)N );
    float rn = pow( fabs(R), (double)N );
    float r = rn - xn - yn;
    if( r <= 0. )
        return 0.;
    float height = pow( r, 1./(double)N );
    return height;
}

