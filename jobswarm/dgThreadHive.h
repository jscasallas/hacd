/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/


#ifndef __DG_TREAD_HIVE_H__
#define __DG_TREAD_HIVE_H__


#include "dgWorkerThread.h"

class dgThreadHive  
{
public:
	dgThreadHive(void);
	~dgThreadHive();

	hacd::HaI32 GetThreadCount() const;
	hacd::HaI32 GetMaxThreadCount() const;
	void SetThreadsCount (hacd::HaI32 count);

	bool ProcessWorkerThreads(); // returns true if there is still work pending to be completed..
	void QueueJob (dgWorkerThreadTaskCallback callback, void* const context);

private:
	void DestroydgThreads();
	hacd::HaI32 m_numOfThreads;			// number of threads allocated for the hive.
	hacd::HaI32 m_taskScheduler;		// array index which places spreads jobs accross the multiple available threads. 
	dgWorkerThread* m_workerBee;		// the array of worker threads.
};



#endif