#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

int NowYear; // 2022 - 2027
int NowMonth; // 0 - 11

float NowPrecip; // inches of rain per month
float NowTemp; // temperature this month
float NowHeight; // grain height in inches
int NowNumDeer; // number of deer in the current population
int NowNumGoat; // number of goats in the current population

const float GRAIN_GROWS_PER_MONTH = 8.0;
const float ONE_DEER_EATS_PER_MONTH = 1.0;
const float ONE_GOAT_EATS_PER_MONTH = 0.2;

const float AVG_PRECIP_PER_MONTH = 7.0; // average
const float AMP_PRECIP_PER_MONTH = 6.0; // plus or minus
const float RANDOM_PRECIP = 2.0; // plus or minus noise

const float AVG_TEMP = 60.0; // average
const float AMP_TEMP = 20.0; // plus or minus
const float RANDOM_TEMP = 10.0; // plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;

unsigned int seed = 0;
float Ranf(unsigned int *seedp, float low, float high);
int Ranf(unsigned int *seedp, int ilow, int ihigh); 
void Deer();
void Grain();
void Watcher();
void Goat();

float SQR(float x) { return x * x; }

int main(int argc, char *argv[])
{
    // starting date and time:
    NowMonth = 0;
    NowYear = 2022;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowNumGoat = 3;
    NowHeight = 5.;

    //omp_set_num_threads( 4 ); // same as # of sections
    omp_set_num_threads( 4 );
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Deer( );
        }

        #pragma omp section
        {
            Grain( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            Goat( ); // your own
        }
    }   // implied barrier -- all functions must return in order
        // to allow any of them to get past here
}

float
Ranf( unsigned int *seedp,  float low, float high )
{
    float r = (float) rand_r( seedp ); // 0 - RAND_MAX

    return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;

    return (int)(  Ranf(seedp, low,high) );
}

void
Deer()
{
    while( NowYear < 2028 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)( NowHeight );
        if( nextNumDeer < carryingCapacity )
            nextNumDeer++;
        else
            if( nextNumDeer > carryingCapacity )
                nextNumDeer--;
        
        if( nextNumDeer < 0 )
            nextNumDeer = 0;
        
        // DoneComputing barrier:
        #pragma omp barrier
        NowNumDeer = nextNumDeer;
        
        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .
        
        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }
}

void
Grain()
{
    while( NowYear < 2028 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
        
        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        
        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;

        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
        nextHeight -= (float)NowNumGoat * ONE_GOAT_EATS_PER_MONTH;
        if( nextHeight < 0. ) nextHeight = 0.;

        // DoneComputing barrier:
        #pragma omp barrier
        NowHeight = nextHeight;

        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .
        
        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }
}

void
Watcher()
{
    float temp_c, height_cm, precip_cm;

    while( NowYear < 2028 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        
        
        // DoneComputing barrier:
        #pragma omp barrier
        //. . .
        
        // DoneAssigning barrier:
        #pragma omp barrier
        temp_c = (NowTemp - 32.) * 5. / 9.;
        height_cm = 2.54 * NowHeight;
        precip_cm = 2.54 * NowPrecip;
        
        NowMonth++;
        if (NowMonth == 12)
        {
            NowMonth = 0;
            NowYear++;
        }
        
        //fprintf(stderr, "Month %d : Year %d ; Temperature = %.2f ; Precipitation = %.2f ; Height of grain = %.2f ; Number of deer = %d ; Number of goats = %d\n",
        //       NowMonth, NowYear, temp_c, precip_cm, height_cm, NowNumDeer, NowNumGoat);
        fprintf(stderr, "%d,%.2f,%.2f,%.2f,%d,%d\n", NowMonth, temp_c, precip_cm, height_cm, NowNumDeer, NowNumGoat);
        
        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }
}

/*
 I chose my agent to be goats. The carrying capacity will be defined by 1/3 of the height of
 the grain. The goat population will decrease when they're above carrying capacity.
 Otherwise, the population increases.
 */
void
Goat()
{
    while( NowYear < 2028 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumGoat = NowNumGoat;
        int carryingCapacity = (int)( NowHeight / 3 );
        if( nextNumGoat < carryingCapacity )
            nextNumGoat++;
        else 
            if( nextNumGoat > carryingCapacity )
                nextNumGoat--;
        
        if( nextNumGoat < 0 )
            nextNumGoat = 0;
        
        // DoneComputing barrier:
        #pragma omp barrier
        NowNumGoat = nextNumGoat;
        
        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .
        
        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }
}

