/*
 *
 * Copyright (c) 1998-9
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */
 
 /*
  *	FILE     hptimer.h
  *	VERSION  2.00
  *
  * generic high performance timer class, uses Windows API
  * when available, otherwise clock(), whether or not high
  * performance is actually achieved depends upon the underlying
  * operating system.
  *
  */

#ifndef __wtimer_h
#define __wtimer_h

#include <jm/jm_cfg.h>

#if (defined(__WIN32__) || defined(_WIN32) || defined(WIN32)) && !defined(JM_NO_INT64)

#include <windows.h>


typedef LARGE_INTEGER hpt_li;
typedef JM_INT64t htp_i;
#define HPT_MAXI JM_IMM64(0x7000000000000000)

#else

#include <time.h>

typedef clock_t hpt_li;
typedef clock_t htp_i;
#define HPT_MAXI 0x70000000L

inline void QueryPerformanceFrequency(hpt_li* p)
{
   *p = CLK_TCK;
}

inline void QueryPerformanceCounter(hpt_li* p)
{
   *p = clock();
}

#endif


class hptimer
{
   unsigned long count;
   htp_i PerformanceFreq;
   htp_i time1;
   htp_i time2;
   unsigned long num;
   htp_i total;
   htp_i _max, _min;
public:
   hptimer()
   {
      QueryPerformanceFrequency((hpt_li*)&PerformanceFreq);
      reset();
   }

   void start()
   {
      QueryPerformanceCounter((hpt_li*)&time1);
   }

   void stop()
   {
      QueryPerformanceCounter((hpt_li*)&time2);
      ++num;
      htp_i interval = time2 - time1;
      total += interval;
      if(interval < _min) _min = interval;
      if(interval > _max) _max = interval;
   }

   htp_i time()
   {
      double value = total;
      value /= (double)num;
      value *= (double)1000000L;
      value /= (double)(PerformanceFreq);
      return value;
   }

   htp_i minval()
   {
      double value = _min;
      value /= (double)(PerformanceFreq);
      value *= (double)1000000L;
      return value;
   }

   htp_i maxval()
   {
      double value = _max;
      value /= (double)(PerformanceFreq);
      value *= (double)1000000L;
      return value;
   }

   void reset()
   {
      num = 0;
      total = 0;
      _max = 0;
      _min = HPT_MAXI;
   }
};

#endif








