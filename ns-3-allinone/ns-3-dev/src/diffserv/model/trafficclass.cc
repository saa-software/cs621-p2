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
	maxPackets = 1000;
	maxBytes = 50000;
	weight = 0;
	priority_level = 0;
	isDefault = false;
}
	
TrafficClass::~TrafficClass ()
{
	NS_LOG_FUNCTION_NOARGS ();
}

void 
TrafficClass::SetMode (int mode)
{
	NS_LOG_FUNCTION (this);
	if (mode == 0)
	{
		m_mode = QUEUE_MODE_PACKETS;
	} else {
		m_mode = QUEUE_MODE_BYTES;
	}
}

void TrafficClass::SetMaxPackets(uint32_t p) {
	NS_LOG_FUNCTION (this);
	maxPackets = p;
}

void TrafficClass::SetMaxBytes(uint32_t b) {
	NS_LOG_FUNCTION (this);
	maxBytes = b;
}

bool
TrafficClass::Enqueue (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);
	switch (m_mode) {
		case QUEUE_MODE_BYTES:
			if (bytes + p->GetSize () <= maxBytes) {
				//enqueue
				m_queue.push(p);
				bytes += p->GetSize ();

				NS_LOG_FUNCTION (bytes);

				return true;
			} else {
				return false;
			}
			break;
		case QUEUE_MODE_PACKETS:
			if (packets + 1 <= maxPackets) {
				m_queue.push(p);
				packets++;

				NS_LOG_FUNCTION (packets);
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
	NS_LOG_FUNCTION (this);
	Ptr<Packet> p = m_queue.front();
	m_queue.pop();
	switch (m_mode) {
		case QUEUE_MODE_BYTES:
			bytes -= p->GetSize ();
			NS_LOG_FUNCTION (bytes);
			break;
		case QUEUE_MODE_PACKETS:
			packets--;
			NS_LOG_FUNCTION (packets);
			break;
	}
	return p;
}

bool
TrafficClass::match (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);
	for (unsigned int i = 0; i < filters.size(); i++)
	{
		NS_LOG_FUNCTION (i);
		Filter* f = filters.at(i);
		NS_LOG_FUNCTION (f);
		if (f->match(p)) {
			return true;
		}
	}
	return false;
}

void TrafficClass::SetPriorityLevel(int l)
{
	NS_LOG_FUNCTION (this);
	priority_level = l;
}


void TrafficClass::SetWeight(int w)
{
	NS_LOG_FUNCTION (this);
	weight = w;
}


void
TrafficClass::SetFilters(vector<Filter*> f)
{
	NS_LOG_FUNCTION (this);
	filters = f;
}

bool
TrafficClass::isEmpty() 
{
	NS_LOG_FUNCTION (this);
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

