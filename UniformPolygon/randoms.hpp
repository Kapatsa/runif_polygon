//
//  randoms.hpp
//  FibPositive
//
//  Created by David Kapatsa on 3/25/19.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#ifndef randoms_hpp
#define randoms_hpp

#include <stdio.h>
#include <math.h>
#include "stdint.h"

void  rninit   ( uint32_t iufir );
void  rnrest   ( void );
void  rnconst  ( uint32_t iufir );
void  rnconfix ( uint32_t nmb );

uint32_t rnfirst (void);
uint32_t rnlast  (void);
uint32_t rnconrd (void);

double  rnunif ( void );
uint32_t  rnexp  ( void );
uint32_t  rnnorm ( void );


#endif /* randoms_hpp */
