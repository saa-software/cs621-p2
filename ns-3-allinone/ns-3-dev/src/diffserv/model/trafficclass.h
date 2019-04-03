/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef TRAFFICCLASS_H
#define TRAFFICCLASS_H

#include <vector>
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/queue.h"
#include "ns3/object.h"
// #include "ns3/filter.h"


namespace ns3 {

class TrafficClass  : public Object {
/**
* \ingroup diffserv
*/
public:
	/**
	* A collection of filters for the class, any of which should match
	*/
	// TODO: put this back vector<Filter*> filters;
	
	/**
	* \brief Get the TypeId
	*
	* \return The TypeId for this class
	*/
	static TypeId GetTypeId (void);

	/**
	* Construct a TrafficClass
	*
	* Default constructor
	*/
	TrafficClass ();

	/**
	* Destroy a TrafficClass
	*
	* Destructor for a TrafficClass
	*/
	virtual ~TrafficClass ();

	/**
	* Enqueues a packet to the queue.
	*
	* \param p Packet to enqueue
	* \returns a boolean indicating whether or not to operation was successful
	*/

	bool Enqueue(Ptr<Packet> p);

	/**
	* Dequeues a packet from the queue.
	*
	* \returns the packet dequeued
	*/
	Ptr<Packet> Dequeue();

	/**
	* Indicates whether or not a packet matches the Filter(s) of the TrafficClass
	*
	* \param p Packet to check for a match
	* \returns a boolean indicating whether or not packet was a match
	*/
	bool match(Ptr<Packet> p);


private:
	/**
	* Count of bytes in Queue
	*/
	uint32_t bytes;

	/**
	* Count of packets in queue
	*/
	uint32_t packets;

	/**
	* Maximum number of packets allowed in queue
	*/
	uint32_t maxPackets;

	/**
	* Maximum number of bytes allowed in queue
	*/
	uint32_t maxBytes;

	/**
	* Weight of TrafficClass (only applicable if QoS mechanism uses weight)
	*/
	double_t weight;

	/**
	* Priority level of TrafficClass (only applicable if QoS mechanism uses priority level)
	*/
	uint32_t priority_level;

	/**
	* If it is default or not
	*/
	bool isDefault;

	/**
	* The queue which the TrafficClass has management of.
	* \see class Queue
	*/
	 Ptr<Queue<Packet> > m_queue;


};


}

#endif /* TRAFFICCLASS_H */

