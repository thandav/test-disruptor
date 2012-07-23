/*
 * claim_strategy.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef CLAIM_STRATEGY_H_
#define CLAIM_STRATEGY_H_

#include "interface.h"

namespace disruptor {


class SingleThreadedClaimStrategy : public ClaimStrategy {

public:

	SingleThreadedClaimStrategy(int buffer_size)
	: buffer_size_(buffer_size), min_gating_sequence_(-1), claim_sequence_(-1) {

	}

	int bufferSize() {
		return buffer_size_;
	}

	long sequence() {
		return claim_sequence_;
	}

	bool hasAvailableCapacity(int avbl_capacity, const std::vector<Sequence*>& dependent_sequences) {
		long wrap_point = (claim_sequence_ + avbl_capacity) - buffer_size_;
		if (wrap_point > min_gating_sequence_) {
			min_gating_sequence_ = getMinimumSequence(dependent_sequences);
			if (wrap_point > min_gating_sequence_) {
				return false;
			}
		}
		return true;
	}

	long incrementAndGet(const std::vector<Sequence*>& dependent_sequences) {
		long next_sequence = claim_sequence_ + 1L;
		claim_sequence_ = next_sequence;
		waitForFreeSlotAt(next_sequence, dependent_sequences);
		return next_sequence;
	}

	long incrementAndGet(int delta, const std::vector<Sequence*>& dependent_sequences) {
		long next_sequence = claim_sequence_ + delta;
		claim_sequence_ = next_sequence;
		waitForFreeSlotAt(next_sequence, dependent_sequences);
		return next_sequence;
	}

	void setSequence(long sequence, const std::vector<Sequence*>& dependent_sequences) {
		claim_sequence_ = sequence;
		waitForFreeSlotAt(sequence, dependent_sequences);
	}

	void serializePublishing(long sequence, Sequence& cursor, int batch_size) {
		cursor.set(sequence);
	}

	long checkAndIncrement(int avbl_capacity, int delta, const std::vector<Sequence*>& dependent_sequences) {
		if (!hasAvailableCapacity(avbl_capacity, dependent_sequences)) {
			throw new std::exception();
		}

		return incrementAndGet(delta, dependent_sequences);
	}

private:

	void waitForFreeSlotAt(long sequence, const std::vector<Sequence*>& dependent_sequences) {
		long wrap_point = sequence - buffer_size_;
		if (wrap_point > min_gating_sequence_) {
			long min_sequence;
			while (wrap_point > (min_sequence = getMinimumSequence(dependent_sequences))) {
				//i think you should yield here???
			}
			min_gating_sequence_ = min_sequence;
		}
	}

	long getMinimumSequence(const std::vector<Sequence*>& sequences) {
		long minimum = LONG_MAX;
		for (int i = 0; i < sequences.size(); i++) {
			long sequence = sequences[i]->get();
			minimum = minimum < sequence ? minimum : sequence;
		}
		return minimum;
	}

	int buffer_size_;

	long min_gating_sequence_;

	long claim_sequence_;

};

}

#endif /* CLAIM_STRATEGY_H_ */
