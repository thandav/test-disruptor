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
#include <pthread.h>

struct processor_attr {
	disruptor::RingBuffer<disruptor::TestEvent>* ring_buffer;
	disruptor::SequenceBarrier* sequence_barrier;
	disruptor::Sequence* sequence;
};

void* processor(void* ptr) {
	struct processor_attr* attr = (processor_attr*) ptr;
	long next_sequence = attr->sequence->get() + 1L;
	while (true) {
		long avbl_sequence = attr->sequence_barrier->waitFor(next_sequence);
		disruptor::TestEvent* event = NULL;
		while (next_sequence <= avbl_sequence) {
			event = attr->ring_buffer->get(next_sequence);
			std::cout << "found: " << event->sequence() << " " << event->value() << std::endl;
			next_sequence++;
			sleep(1);
		}
		attr->sequence->set(event->sequence());
	}
	return ptr;
}

int main(int argc, const char** argv) {
	std::cout << "program start --- " << std::endl;

	disruptor::ClaimStrategy* claim_strategy = new disruptor::SingleThreadedClaimStrategy(16);
	disruptor::WaitStrategy* wait_strategy = new disruptor::BusySpinWaitStrategy();

	disruptor::RingBuffer<disruptor::TestEvent>* ring_buffer =
			new disruptor::RingBuffer<disruptor::TestEvent>(claim_strategy, wait_strategy);

	disruptor::Sequence* seqnum = new disruptor::Sequence(-1);

	std::vector<disruptor::Sequence*> sequences_to_track;

	disruptor::SequenceBarrier* sequence_barrier = ring_buffer->newBarrier(sequences_to_track);

	sequences_to_track.push_back(seqnum);

	ring_buffer->setGatingSequences(sequences_to_track);

	struct processor_attr* attr = new processor_attr();
	attr->ring_buffer = ring_buffer;
	attr->sequence_barrier = sequence_barrier;
	attr->sequence = seqnum;

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, processor, (void*) attr);
	if (rc) {
		std::cout << "ERROR: return code from pthread_create() is: " << rc << std::endl;
		exit(1);
	}
	for (int i = 0; i < 100; i++) {
		long sequence = ring_buffer->next();
		disruptor::TestEvent* event = ring_buffer->get(sequence);
		std::stringstream ss;
		ss << "this is seqnum: " << i;

		event->setSequence(i);
		event->setValue(ss.str());
		ring_buffer->publish(event->sequence());
		std::cout << "added event seqnum: " << event->sequence() << " event: " << event->value() << std::endl;
		sleep(2);
	}

	std::cout << "program end --- " << std::endl;

}
