/*
 * ring_buffer.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "interface.h"
#include <vector>
#include "sequencer.h"

namespace disruptor {

template<typename T>
class RingBuffer : public Sequencer {

public:

	RingBuffer<T>(ClaimStrategy* claim_strategy, WaitStrategy* wait_strategy)
	: Sequencer(claim_strategy, wait_strategy) {
		index_mask_ = claim_strategy_->bufferSize() - 1;
		fill(claim_strategy_->bufferSize());
	}

	T* get(long sequence) {
		return entries[(int) sequence & index_mask_];
	}

private:

	void fill(int size) {
		for (int i = 0; i < size; i++) {
			T* entry = new T();
			entries.push_back(entry);
		}
	}

	int buffer_size_;

	int index_mask_;

	std::vector<T*> entries;
};

}


#endif /* RING_BUFFER_H_ */
