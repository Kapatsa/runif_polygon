//
//  randoms.cpp
//  FibPositive
//
//  Created by David Kapatsa on 3/25/19.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//
#include "randoms.hpp"
#include <math.h>
#include "stdint.h"
static uint32_t iu, iuhold, mult=663608941l;

double rnunif (void)
{
    const double flt = 0.232830643654e-9;
    iu *= mult;
    return (iu*flt);
}

void rninit ( uint32_t  iufir)
{
    iu = ( ( iufir%2 ) ? iufir : iufir + 1 );
    iuhold = iu;
}

void rnrest (void)
{ rninit (iuhold); }

void rnconst ( uint32_t multnew)
{
    int rest = multnew % 8;
    if ( (rest != 5) && (rest != 3) )
        mult = multnew - rest + 5;
    else
        mult = multnew;
}

uint32_t rnlast (void)
{ return ( iu ); }

uint32_t rnfirst (void)
{ return ( iuhold ); }
