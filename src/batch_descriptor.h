/*
 * batch_descriptor.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef BATCH_DESCRIPTOR_H_
#define BATCH_DESCRIPTOR_H_

#include "sequencer.h"

namespace disruptor {

class BatchDescriptor {

public:

	BatchDescriptor(int size) : size_(size), end_(Sequencer::INITIAL_CURSOR_VALUE) {

	}

	long end() const {
		return end_;
	}

	void setEnd(long end) {
		end_ = end;
	}

	int size() const {
		return size_;
	}

	long start() {
		return end_ - (size_ -1L);
	}

private:

	int size_;

	long end_;

};

}

#endif /* BATCH_DESCRIPTOR_H_ */
