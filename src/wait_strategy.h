/*
 * wait_strategy.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef WAIT_STRATEGY_H_
#define WAIT_STRATEGY_H_


#include "interface.h"

namespace disruptor {


class BusySpinWaitStrategy : public WaitStrategy {

public:

	long waitFor(long sequence, Sequence& cursor, const std::vector<Sequence*>& dependents,
			SequenceBarrier* barrier) {
		long avbl_sequence;

		if (0 == dependents.size()) {
			while ((avbl_sequence = cursor.get()) < sequence) {
				barrier->checkAlert();
			}
		} else {
			while ((avbl_sequence = getMinimumSequence(dependents)) < sequence) {
				barrier->checkAlert();
			}
		}
		return avbl_sequence;
	}

	void signalAllWhenBlocking() {

	}

private:

	long getMinimumSequence(const std::vector<Sequence*>& sequences) {
		long minimum = LONG_MAX;
		for (int i = 0; i < sequences.size(); i++) {
			long sequence = sequences[i]->get();
			minimum = minimum < sequence ? minimum : sequence;
		}
		return minimum;
	}

};

}

#endif /* WAIT_STRATEGY_H_ */
