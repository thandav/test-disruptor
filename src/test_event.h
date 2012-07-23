/*
 * test_event.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef TEST_EVENT_H_
#define TEST_EVENT_H_

namespace disruptor {

class TestEvent {

public:

	long sequence() {
		return sequence_;
	}

	void setSequence(long sequence) {
		sequence_ = sequence;
	}

	void setValue(const std::string& value) {
		value_ = value;
	}

	const std::string& value() {
		return value_;
	}

private:

	long sequence_;

	std::string value_;
};

}


#endif /* TEST_EVENT_H_ */
