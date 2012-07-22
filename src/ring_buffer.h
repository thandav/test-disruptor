/*
 * ring_buffer.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

namespace disruptor {

template<typename T>
class RingBuffer<T> {

public:

	RingBuffer<T>(EventFactory<T> event_factory, ClaimStrategy* claim_strategy, WaitStrategy* wait_strategy)
	: claim_strategy_(claim_strategy), wait_strategy_(wait_strategy) {
		index_mask_ = claim_strategy_->bufferSize() - 1;
		entries = new T[claim_strategy_->bufferSize()];
		fill(event_factory, claim_strategy_->bufferSize());
	}

	T& get(long sequence) {
		return (T) entries[(int) sequence & index_mask_];
	}

private:

	void fill(EventFactory<T>& event_factory, int size) {
		for (int i = 0; i < size; i++) {
			T* entry = event_factory.newInstance();
			entries.push_back(entry);
		}
	}

	int index_mask_;

	ClaimStrategy* claim_strategy_;

	WaitStrategy* wait_strategy_;

	std::vector<T*> entries;
};

}


#endif /* RING_BUFFER_H_ */
