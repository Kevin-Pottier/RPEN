/*
 * fft.c
 *
 *  Created on: 16 juin 2020
 *      Author: Nirgal
 */


/*
 * Xavier Gourdon : Sept. 99 (xavier.gourdon.free.fr)
 *
 * FFT.c : Very basic FFT file.
 *         The FFT encoded in this file is short and very basic.
 *         It is just here as a teaching file to have a first
 *         understanding of the FFT technique.
 *
 * A lot of optimizations could be made to save a factor 2 or 4 for time
 * and space.
 *  - Use a 4-Step (or more) FFT to avoid data cache misses.
 *  - Use an hermitian FFT to take into account the hermitian property of
 *    the FFT of a real array.
 *  - Use a quad FFT (recursion N/4->N instead of N/2->N) to save 10 or
 *    15% of the time.
 *
 *  Informations can be found on
 *    http://xavier.gourdon.free.fr/Constants/constants.html
 */
 #include "config.h"
 #if USE_FFT
 
#include "fft.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932384626

#define MAX_LENGTH	512




double FFTSquareWorstError;


static Complex OmegaFFT[MAX_LENGTH/2];
static Complex ArrayFFT0[MAX_LENGTH];
static Complex ArrayFFT1[MAX_LENGTH];
static Complex ComplexCoef[MAX_LENGTH];

void FFT_demo(void)
{
	Real Coef[128] = {	1.370632899,1.649395177,1.854765545,1.97089946,1.989159797,1.90887103,1.737370023,
			1.489349003,1.185549417,0.850923283,0.512426139,0.196638513,-0.072571899,-0.276138893,-0.401451113,
			-0.443422903,-0.404887562,-0.296278488,-0.134627336,0.058030605,0.256700651,0.435739208,0.571223346,
			0.643166994,0.637385006,0.546837803,0.372335693,0.122539207,-0.186745303,-0.533912581,-0.893428232,
			-1.238054902,-1.541236586,-1.779428543,-1.934165134,-1.99368121,-1.953942507,-1.818993204,-1.600589725,
			-1.317153557,-0.99213658,-0.651944831,-0.323605978,-0.032388598,0.200414417,0.359340718,0.436086422,
			0.430188993,0.349004685,0.206983191,0.024304978,-0.174996253,-0.365257517,-0.521557477,-0.62202179,
			-0.649856128,-0.59492451,-0.454731147,-0.234717347,0.052153776,0.386488422,0.744132223,1.098270829,
			1.421773861,1.689573392,1.880864923,1.98093464,1.98245004,1.886098871,1.70051947,1.441528814,1.130717232,
			0.793535238,0.457043225,0.147524807,-0.111823306,-0.302918924,-0.414358799,-0.442380625,-0.391137241,
			-0.272259168,-0.103745212,0.091719044,0.288863947,0.462171061,0.588231669,0.647917018,0.628165645,
			0.523227354,0.335251951,0.074169383,-0.243128715,-0.594233655,-0.953162095,-1.292623309,-1.586409306,
			-1.811694797,-1.951041964,-1.993931284,-1.937681947,-1.787679828,-1.556892774,-1.264716535,-0.935254388,
			-0.595184005,-0.271401914,0.011344306,0.232624936,0.378104817,0.440769594,0.421492536,0.328891864,
			0.178492015,-0.00873558,-0.208321118,-0.394559622,-0.542885794,-0.632147584,-0.646574599,-0.577264731,
			-0.423055423,-0.190700999,0.105660539,0.445704288,0.804672583,1.155530394,1.47133002,1.727574357
	};
	Complex FFT_result[128] = {0};

	InitializeFFT();
//	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN,1);
	FFT(Coef,128, FFT_result, 128);
//	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN,0);
	for(uint16_t i=0; i<128; i++)
	{
		debug_printf("%ld,%ld\r\n", (uint32_t)(FFT_result[i].R*1000), (uint32_t)(FFT_result[i].I*1000));
		HAL_Delay(100);
	}
}



void InitializeFFT(void)
{
  long i;
  Real Step;

  Step = 2.*PI/(double) MAX_LENGTH;
  for (i=0; 2*i<MAX_LENGTH; i++) {
    OmegaFFT[i].R = cos(Step*(double)i);
    OmegaFFT[i].I = sin(Step*(double)i);
  }
  FFTSquareWorstError=0.;
}

void RecursiveFFT(Complex * Coef, Complex * FFT, long Length, long Step,
		  long Sign)
{
  long i, OmegaStep;
  Complex * FFT0, * FFT1, * Omega;
  Real tmpR, tmpI;

  if (Length==2) {
    FFT[0].R = Coef[0].R + Coef[Step].R;
    FFT[0].I = Coef[0].I + Coef[Step].I;
    FFT[1].R = Coef[0].R - Coef[Step].R;
    FFT[1].I = Coef[0].I - Coef[Step].I;
    return;
  }

  FFT0 = FFT;
  RecursiveFFT(Coef     ,FFT0,Length/2,Step*2,Sign);
  FFT1 = FFT+Length/2;
  RecursiveFFT(Coef+Step,FFT1,Length/2,Step*2,Sign);

  Omega = OmegaFFT;
  OmegaStep = MAX_LENGTH/Length;
  for (i=0; 2*i<Length; i++, Omega += OmegaStep) {
    /* Recursion formula for FFT :
       FFT[i]          <-  FFT0[i] + Omega*FFT1[i]
       FFT[i+Length/2] <-  FFT0[i] - Omega*FFT1[i],
       Omega = exp(2*I*PI*i/Length) */
    tmpR = Omega[0].R*FFT1[i].R-Sign*Omega[0].I*FFT1[i].I;
    tmpI = Omega[0].R*FFT1[i].I+Sign*Omega[0].I*FFT1[i].R;
    FFT1[i].R = FFT0[i].R - tmpR;
    FFT1[i].I = FFT0[i].I - tmpI;
    FFT0[i].R = FFT0[i].R + tmpR;
    FFT0[i].I = FFT0[i].I + tmpI;
  }
}

/* Compute the complex Fourier Transform of Coef into FFT */
void FFT(Real * Coef, long Length, Complex * FFT, long NFFT)
{
  long i;
  /* Transform array of real coefficient into array of complex */
  for (i=0; i<Length; i++) {
    ComplexCoef[i].R = Coef[i];
    ComplexCoef[i].I = 0.;
  }
  for (; i<NFFT; i++)
    ComplexCoef[i].R = ComplexCoef[i].I = 0.;

  RecursiveFFT(ComplexCoef,FFT,NFFT,1,1);
}

/* Compute the inverse Fourier Transform of FFT into Coef */
void InverseFFT(Complex * FFT, long NFFT, Real * Coef, long Length)
{
  long i;
  Real invNFFT = 1./(Real) NFFT, tmp;

  RecursiveFFT(FFT, ComplexCoef, NFFT, 1, -1);
  for (i=0; i<Length; i++) {
    /* Closest integer to ComplexCoef[i].R/NFFT */
    tmp = invNFFT*ComplexCoef[i].R;
    Coef[i] = floor(0.5+tmp);
    if ((tmp-Coef[i])*(tmp-Coef[i])>FFTSquareWorstError)
      FFTSquareWorstError = (tmp-Coef[i])*(tmp-Coef[i]);
  }
}

void Convolution(Complex * A, Complex * B, long NFFT, Complex * C)
{
  long i;
  Real tmpR, tmpI;

  for (i=0; i<NFFT; i++) {
   tmpR = A[i].R*B[i].R-A[i].I*B[i].I;
    tmpI = A[i].R*B[i].I+A[i].I*B[i].R;
    C[i].R = tmpR;
    C[i].I = tmpI;
  }
}

void MulWithFFT(Real * ACoef, long ASize,
                Real * BCoef, long BSize,
                Real * CCoef)
{
  long NFFT = 2;

  while (NFFT<ASize+BSize)
    NFFT *= 2;

  if (NFFT>MAX_LENGTH) {
    printf("Error, FFT Size is too big in MulWithFFT\n");
    return;
  }
  FFT(ACoef, ASize, ArrayFFT0, NFFT);
  FFT(BCoef, BSize, ArrayFFT1, NFFT);
  Convolution(ArrayFFT0,ArrayFFT1,NFFT,ArrayFFT0);
  InverseFFT(ArrayFFT0,NFFT,CCoef, ASize+BSize-1);
}

#endif
