/*
 * interface.h
 *
 *  Created on: Jul 23, 2012
 *      Author: tb76300
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "sequence.h"
#include <vector>
#include <climits>
#include <exception>
#include <unistd.h>

namespace disruptor {

class SequenceBarrier {

public:

	virtual ~SequenceBarrier() {}

	virtual long waitFor(long sequence) = 0;

	virtual long cursor() = 0;

	virtual bool isAlerted() = 0;

	virtual void alert() = 0;

	virtual void clearAlert() = 0;

	virtual void checkAlert() = 0;

};


class ClaimStrategy {

public:

	virtual ~ClaimStrategy() {}

	virtual int bufferSize() = 0;

	virtual long sequence() = 0;

	virtual bool hasAvailableCapacity(int avbl_capacity, const std::vector<Sequence*>& dependent_sequences) = 0;

	virtual long incrementAndGet(const std::vector<Sequence*>& dependent_sequences) = 0;

	virtual long incrementAndGet(int delta, const std::vector<Sequence*>& dependent_sequences) = 0;

	virtual void setSequence(long, const std::vector<Sequence*>&) = 0;

	virtual void serializePublishing(long, Sequence&, int) = 0;

	virtual long checkAndIncrement(int, int, const std::vector<Sequence*>&) = 0;

};


class WaitStrategy {

public:

	virtual ~WaitStrategy() {}

	virtual long waitFor(long sequence, const Sequence& cursor, const std::vector<Sequence*>& dependents,
			SequenceBarrier* barrier) = 0;

	//TODO add waitFor with timeout

	virtual void signalAllWhenBlocking() = 0;
};


}


#endif /* INTERFACE_H_ */
