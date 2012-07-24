/*
 * sequence_barrier.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef SEQUENCE_BARRIER_H_
#define SEQUENCE_BARRIER_H_

#include "interface.h"

namespace disruptor {


class ProcessingSequenceBarrier : public SequenceBarrier {

public:

	ProcessingSequenceBarrier(WaitStrategy* wait_strategy, Sequence* cursor_sequence, const std::vector<Sequence*>& dependent_sequences)
	: wait_strategy_(wait_strategy), cursor_sequence_(cursor_sequence), dependent_sequences_(dependent_sequences), alerted_(false) {

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
