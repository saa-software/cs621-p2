/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/queue.h"
#include "trafficclass.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TrafficClass");

NS_OBJECT_ENSURE_REGISTERED (TrafficClass);

TypeId
TrafficClass::GetTypeId (void) 
{
static TypeId tid = TypeId ("ns3::TrafficClass")
    .SetGroupName ("TrafficClass")
    .AddConstructor<TrafficClass> ()
	.AddAttribute ("TxQueue",
                   "A queue to use as the transmit queue in the device.",
                   PointerValue (),
                   MakePointerAccessor (&TrafficClass::m_queue),
                   MakePointerChecker<Queue<Packet> > ());
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
}

TrafficClass::~TrafficClass ()
{
	NS_LOG_FUNCTION_NOARGS ();
	m_queue = 0;
}

bool
TrafficClass::Enqueue (Ptr<Packet> p) {
	//TODO fix this
	return false;
}

Ptr<Packet>
TrafficClass::Dequeue () {
	Ptr<Packet> p;
	return p;
}

bool
TrafficClass::match (Ptr<Packet> p) {
	//TODO fix this
	return false;
}

}

