/*
 * batch_descriptor.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef BATCH_DESCRIPTOR_H_
#define BATCH_DESCRIPTOR_H_

namespace disruptor {

class BatchDescriptor {

public:

	BatchDescriptor(int size) : size_(size), end_(-1) {

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
