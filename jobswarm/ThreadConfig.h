#ifndef THREAD_CONFIG_H

#define THREAD_CONFIG_H

/*!
**
** Copyright (c) 20011 by John W. Ratcliff mailto:jratcliffscarab@gmail.com
**
**
** The MIT license:
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is furnished
** to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.

** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
** WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifdef _MSC_VER
typedef __int64 int64_t;
#else
#include <stdint.h>
#endif

namespace THREAD_CONFIG
{

unsigned int tc_timeGetTime(void);
void     tc_sleep(unsigned int ms);

int		tc_atomicAdd(int *addend,int amount);

class ThreadMutex
{
public:
  virtual void lock(void) = 0;
  virtual void unlock(void) = 0;
  virtual bool tryLock(void) = 0;
};


ThreadMutex * tc_createThreadMutex(void);
void          tc_releaseThreadMutex(ThreadMutex *tm);

class ThreadInterface
{
public:
  virtual void threadMain(void) = 0;
};

class Thread
{
public:
	virtual void Suspend(void) = 0;
	virtual void Resume(void) = 0;
};

Thread      * tc_createThread(ThreadInterface *tinterface);
void          tc_releaseThread(Thread *t);

class ThreadEvent
{
public:
  virtual void setEvent(void) = 0; // signal the event
  virtual void resetEvent(void) = 0;
  virtual void waitForSingleObject(unsigned int ms) = 0;
};

ThreadEvent * tc_createThreadEvent(void);
void          tc_releaseThreadEvent(ThreadEvent *t);

}; // end of namespace


#endif
