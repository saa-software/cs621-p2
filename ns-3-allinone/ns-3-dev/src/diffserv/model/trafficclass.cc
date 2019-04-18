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
	weight = 0.0;
	priority_level = 0;
	isDefault = false;
	filters = {};
}
	
TrafficClass::~TrafficClass ()
{
	NS_LOG_FUNCTION_NOARGS ();
}

bool
TrafficClass::Enqueue (Ptr<Packet> p)
{
	if (m_queueMode == "bytes") //placeholder
	{
		if (bytes + p->GetSize () <= maxBytes) {
			//enqueue
			m_queue.push(p);
			bytes += p->GetSize ();
			return true;
		} else {
			return false;
		}
	} else {
		if (packets + 1 <= maxPackets) {
			m_queue.push(p);
			packets++;
			return true;
		} else {
			return false;
		}
	}
}

Ptr<Packet>
TrafficClass::Dequeue ()
{
	Ptr<Packet> p = m_queue.front();
	m_queue.pop();
	if (m_queueMode == "bytes")
	{
		bytes -= p->GetSize ();
	} else {
		packets--;
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

void
TrafficClass::SetFilters(vector<Filter*> f)
{
	filters = f;
}

bool
TrafficClass::isEmpty() 
{
	if (m_queueMode == "bytes")
	{
		return bytes == 0;
	} else {
		return packets == 0;
	}
}

}

