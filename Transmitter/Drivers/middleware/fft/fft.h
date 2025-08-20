#ifndef FFT_h
#define FFT_h

 #include "config.h"
 #if USE_FFT
 
typedef double Real;

extern double FFTSquareWorstError;
extern long AllocatedMemory;

void InitializeFFT(void);

void MulWithFFT(Real * ACoef, long ASize,
                Real * BCoef, long BSize,
                Real * CCoef);

typedef struct ComplexTag {
  Real R,I;
} Complex;


void FFT(Real * Coef, long Length, Complex * FFT, long NFFT);

#endif

#endif
