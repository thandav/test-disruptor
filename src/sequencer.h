/*
 * sequencer.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include <vector>
#include <exception>
#include "sequence.h"
#include "sequence_barrier.h"
#include "claim_strategy.h"
#include "wait_strategy.h"

namespace disruptor {

class Sequencer {

public:

	Sequencer(const ClaimStrategy& claim_strategy, const WaitStrategy& wait_strategy)
	: claim_strategy_(&claim_strategy), wait_strategy_(&wait_strategy) {
		cursor_ = new Sequence(INITIAL_CURSOR_VALUE);
	}

	void setGatingSequences(const std::vector<Sequence*>& sequences) {
		this->gating_sequences_ = sequences;
	}

	SequenceBarrier* newBarrier(const std::vector<Sequence*>& sequences_to_track) {
		return new ProcessingSequenceBarrier(*wait_strategy_, *cursor_, sequences_to_track);
	}

	BatchDescriptor* newBatchDescriptor(int size) {
		return new BatchDescriptor(size < claim_strategy_->bufferSize() ? size : claim_strategy_->bufferSize());
	}

	int bufferSize() {
		return claim_strategy_->bufferSize();
	}

	long cursor() {
		return cursor_->get();
	}

	bool hasAvailableCapacity(int avbl_capacity) {
		return claim_strategy_->hasAvailableCapacity(avbl_capacity, gating_sequences_);
	}

	long next() {
		if (gating_sequences_.size() == 0) {
			throw new std::exception();
		}

		return claim_strategy_->incrementAndGet(gating_sequences_);
	}

	long tryNext(int required_capacity) {
		if (gating_sequences_.size() == 0) {
			throw new std::exception();
		}

		if (required_capacity < 1) {
			throw new std::exception();
		}

		return claim_strategy_->checkAndIncrement(required_capacity, 1, gating_sequences_);
	}

	BatchDescriptor& next(BatchDescriptor& batch_descriptor) {
		if (gating_sequences_.size() == 0) {
			throw new std::exception();
		}
		long sequence = claim_strategy_->incrementAndGet(batch_descriptor.size(), gating_sequences_);
		batch_descriptor.setEnd(sequence);
		return batch_descriptor;
	}

	long claim(long sequence) {
		if (gating_sequences_.size() == 0) {
			throw new std::exception();
		}

		claim_strategy_->setSequence(sequence, gating_sequences_);
		return sequence;
	}

	void publish(long sequence) {
		publish(sequence, 1);
	}

	void publish(const BatchDescriptor& batch_descriptor) {
		publish(batch_descriptor.end(), batch_descriptor.size());
	}

	void forcePublish(long sequence) {
		cursor_->set(sequence);
		wait_strategy_->signalAllWhenBlocking();
	}

	long remainingCapacity() {
		//TODO
		return 0;
	}

	static const long INITIAL_CURSOR_VALUE = -1L;

private:

	void publish(long sequence, int batch_size) {
		cursor_->set(sequence);
		wait_strategy_->signalAllWhenBlocking();
	}

	Sequence* cursor_;

	std::vector<Sequence*> gating_sequences_;

	ClaimStrategy* claim_strategy_;

	WaitStrategy* wait_strategy_;

};

}


#endif /* SEQUENCER_H_ */
