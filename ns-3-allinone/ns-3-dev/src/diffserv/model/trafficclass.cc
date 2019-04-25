/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>
#include <string>
#include <queue>
using namespace std;

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/queue.h"
#include "trafficclass.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "filter.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TrafficClass");

NS_OBJECT_ENSURE_REGISTERED (TrafficClass);

TypeId
TrafficClass::GetTypeId (void) 
{
static TypeId tid = TypeId ("ns3::TrafficClass")
    .SetGroupName ("TrafficClass")
    .AddConstructor<TrafficClass> ();
	return tid;
}

TrafficClass::TrafficClass ()
{
	NS_LOG_FUNCTION (this);
	bytes = 0;
	packets = 0;
	maxPackets = 0;
	maxBytes = 0;
	weight = 0;
	priority_level = 0;
	isDefault = false;
	filters = {};
}
	
TrafficClass::~TrafficClass ()
{
	NS_LOG_FUNCTION_NOARGS ();
}

void 
TrafficClass::SetMode (TrafficClass::QueueMode mode)
{
	m_mode = mode;
}

bool
TrafficClass::Enqueue (Ptr<Packet> p)
{
	switch (m_mode) {
		case QUEUE_MODE_BYTES:
			if (bytes + p->GetSize () <= maxBytes) {
				//enqueue
				m_queue.push(p);
				bytes += p->GetSize ();
				return true;
			} else {
				return false;
			}
			break;
		case QUEUE_MODE_PACKETS:
			if (packets + 1 <= maxPackets) {
				m_queue.push(p);
				packets++;
				return true;
			} else {
				return false;
			}
		break;
	}
	return false;
}

Ptr<Packet>
TrafficClass::Dequeue ()
{
	Ptr<Packet> p = m_queue.front();
	m_queue.pop();
	switch (m_mode) {
		case QUEUE_MODE_BYTES:
			bytes -= p->GetSize ();
			break;
		case QUEUE_MODE_PACKETS:
			packets--;
			break;
	}
	return p;
}

bool
TrafficClass::match (Ptr<Packet> p)
{
	for (unsigned int i = 0; i < filters.size(); i++)
	{
		if ((*filters.at(i)).match(p)) {
			return true;
		}
	}
	return false;
}

void TrafficClass::SetPriorityLevel(int l)
{
	priority_level = l;
}


void TrafficClass::SetWeight(int w)
{
	weight = w;
}


void
TrafficClass::SetFilters(vector<Filter*> f)
{
	filters = f;
}

bool
TrafficClass::isEmpty() 
{
	switch (m_mode){
		case QUEUE_MODE_BYTES:
			return bytes == 0;
			break;
		case QUEUE_MODE_PACKETS:
			return packets == 0;
			break;
	}
	return false;
}

}

