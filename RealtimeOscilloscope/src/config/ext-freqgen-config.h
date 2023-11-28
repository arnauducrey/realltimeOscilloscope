/*
 * ext-freqgen-config.h
 *
 *  Created on: 24 Nov 2020
 *      Author: rim
 */

#ifndef EXT_FREQGEN_CONFIG_H_
#define EXT_FREQGEN_CONFIG_H_

#define EXTFREQGEN_NOTIFICATION_TRACE	1 			//  (0,1) default=1 trace out of waveform and frequency
#define EXTFREQGEN_START_FREQUENCY      HZ_1000		//  one of these:
													//	HZ_50 = 50,
													//	HZ_110 =110,
													//	HZ_240 = 240,
													//	HZ_370 = 370,
													//	HZ_500 = 500,
													//	HZ_700 = 700,
													//	HZ_1000 = 1000,
													//	HZ_5000 = 5000,
													//	HZ_10000 = 10000
#define EXTFREQGEN_START_WAVEFORM       TRIANGLE	//  one of these:
													//  SINUS
													//  TRIANGLE
													//  SQUARE

#endif /* EXT_FREQGEN_CONFIG_H_ */
