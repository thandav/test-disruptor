/*
 * event_factory.h
 *
 *  Created on: Jul 22, 2012
 *      Author: thandav
 */

#ifndef EVENT_FACTORY_H_
#define EVENT_FACTORY_H_

namespace disruptor {

template<typename T>
class EventFactory {
public:
	virtual ~EventFactory();
	virtual T* newInstance(const int& size) = 0;
};

#endif /* EVENT_FACTORY_H_ */
