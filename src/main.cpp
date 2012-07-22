/*
 * main.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#include <iostream>

#include "claim_strategy.h"
#include "wait_strategy.h"

int main(int argc, const char** argv) {
	std::cout << "program start --- " << std::endl;

	disruptor::ClaimStrategy* claim_strategy = new disruptor::SingleThreadedClaimStrategy(1024);
	disruptor::WaitStrategy* wait_strategy = new disruptor::BusySpinWaitStrategy();

	std::cout << "program end --- " << std::endl;

}
