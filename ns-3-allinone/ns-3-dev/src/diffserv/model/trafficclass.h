/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef TRAFFICCLASS_H
#define TRAFFICCLASS_H

#include <vector>
#include <string>
#include <queue>
using namespace std;

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/queue.h"
#include "ns3/object.h"
#include "filter.h"


namespace ns3 {

class TrafficClass : public Object {
/**
* \ingroup diffserv
*/
public:
	enum QueueMode
	  {
	    QUEUE_MODE_PACKETS,
	    QUEUE_MODE_BYTES,
	  };

	void SetMode (TrafficClass::QueueMode mode);
	/**
	* A collection of filters for the class, any of which should match
	*/
	vector<Filter*> filters;
	
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

	bool isEmpty();

	bool Enqueue(Ptr<Packet> p);

	/**
	* Dequeues a packet from the queue.
	*
	* \returns the dequeued packet
	*/
	Ptr<Packet> Dequeue();

	/**
	* Indicates whether or not a packet matches the Filter(s) of the TrafficClass
	*
	* \param p Packet to check for a match
	* \returns a boolean indicating whether or not packet was a match
	*/
	bool match(Ptr<Packet> p);
	/**
	* Set filters for the queue
	*
	* \param f vector of filters
	*/
	void SetFilters(vector<Filter*> f);


	/**
	* The queue which the TrafficClass has management of.
	* \see class Queue
	*/
	 queue<Ptr<Packet>> m_queue;

	 void SetPriorityLevel(uint32_t w);

	 void SetWeight(uint32_t w);

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
	uint32_t weight;

	/**
	* Priority level of TrafficClass (only applicable if QoS mechanism uses priority level)
	*/
	uint32_t priority_level;

	/**
	* If it is default or not
	*/
	bool isDefault;

	/**
	* Mode of the queue
	*/
	TrafficClass::QueueMode m_mode;


};


}

#endif /* TRAFFICCLASS_H */

