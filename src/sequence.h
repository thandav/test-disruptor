/*
 * sequence.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

namespace disruptor {

class Sequence {

public:

	Sequence(long initial_value) : value_(initial_value) {

	}

	long get() {
		return value_;
	}

	long get() const {
		return value_;
	}

	void set(long value) {
		this->value_ = value;
	}

private:

	long value_;

};

}

#endif /* SEQUENCE_H_ */
