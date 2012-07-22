/*
 * sequence_barrier.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef SEQUENCE_BARRIER_H_
#define SEQUENCE_BARRIER_H_

#include <vector>
#include "sequence.h"
#include "wait_strategy.h"

namespace disruptor {

class SequenceBarrier {

public:

	virtual ~SequenceBarrier();

	virtual long waitFor(long sequence) = 0;

	virtual long cursor() = 0;

	virtual bool isAlerted() = 0;

	virtual void alert() = 0;

	virtual void clearAlert() = 0;

	virtual void checkAlert() = 0;

};

class ProcessingSequenceBarrier : public SequenceBarrier {

public:

	ProcessingSequenceBarrier(const WaitStrategy& wait_strategy,
			const Sequence& cursor_sequence,
			const std::vector<Sequence*>& dependent_sequences)
	: wait_strategy_(&wait_strategy), cursor_sequence_(cursor_sequence),
	  dependent_sequences_(dependent_sequences), alerted_(false) {

	}

	long waitFor(long sequence) {
		checkAlert();
		return wait_strategy_->waitFor(sequence, *cursor_sequence_, dependent_sequences_, this);
	}

	long cursor() {
		return cursor_sequence_->get();
	}

	bool isAlerted() {
		return alerted_;
	}

	void alert() {
		alerted_ = true;
		wait_strategy_->signalAllWhenBlocking();
	}

	void clearAlert() {
		alerted_ = false;
	}

	void checkAlert() {
		if (alerted_) {
			throw new std::exception();
		}
	}

private:

	WaitStrategy* wait_strategy_;

	Sequence* cursor_sequence_;

	std::vector<Sequence*> dependent_sequences_;

	bool alerted_;

};

}


#endif /* SEQUENCE_BARRIER_H_ */
