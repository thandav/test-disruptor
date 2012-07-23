/*
 * main.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#include <iostream>
#include <sstream>
#include "ring_buffer.h"
#include "wait_strategy.h"
#include "claim_strategy.h"
#include "test_event.h"

int main(int argc, const char** argv) {
	std::cout << "program start --- " << std::endl;

	disruptor::ClaimStrategy* claim_strategy = new disruptor::SingleThreadedClaimStrategy(16);
	disruptor::WaitStrategy* wait_strategy = new disruptor::BusySpinWaitStrategy();

	disruptor::RingBuffer<disruptor::TestEvent>* ring_buffer =
			new disruptor::RingBuffer<disruptor::TestEvent>(claim_strategy, wait_strategy);

	for (int i = 0; i < 100; i++) {
		disruptor::TestEvent* event = ring_buffer->get(i);
		std::stringstream ss;
		ss << "this is seqnum: " << i;

		event->setSequence(i);
		event->setValue(ss.str());
	}

	for (int i = 0; i < 16; i++) {
		disruptor::TestEvent* event = ring_buffer->get(i);
		std::cout << "found: " << event->sequence() << " " << event->value() << std::endl;
	}

	std::cout << "program end --- " << std::endl;

}
