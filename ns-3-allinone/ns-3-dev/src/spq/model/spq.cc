/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>
#include <iostream>
using namespace std;

#include "spq.h"
#include "ns3/filter.h"
#include "ns3/trafficclass.h"
#include "ns3/integer.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/string.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SPQ);

// NS_LOG_COMPONENT_DEFINE ("SPQ");
/* ... */
const int LOW = 0;
const int HIGH = 1;

TypeId
SPQ::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::SPQ<Packet>")
          .SetParent<DiffServ<Packet>> ()
          .SetGroupName ("Network")
          .template AddConstructor<SPQ> ()
          .AddAttribute ("Mode",
                         "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the "
                         "maximum queue size metric.",
                         EnumValue (QUEUE_MODE_PACKETS),
                         MakeEnumAccessor (&SPQ::SetMode, &SPQ::GetMode),
                         MakeEnumChecker (QUEUE_MODE_BYTES, "QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS,
                                          "QUEUE_MODE_PACKETS"))
          .AddAttribute ("NumberOfQueues", "The number of queues to use in SPQ", IntegerValue (0),
                         MakeIntegerAccessor (&SPQ::SetNumberOfQueues),
                         MakeIntegerChecker<int> (INT64_MIN, INT64_MAX))
          .AddAttribute ("QueueLevels",
                         "A string that represents the level for each queue. Number of chars must "
                         "match queue levels.",
                         StringValue (""), MakeStringAccessor (&SPQ::SetQueueLevels),
                         MakeStringChecker ())
          .AddAttribute ("Setup", "Initiates setup for SPQ queue.", IntegerValue (1),
                         MakeIntegerAccessor (&SPQ::Setup),
                         MakeIntegerChecker<int> (INT64_MIN, INT64_MAX));
  return tid;
}

void
SPQ::SetNumberOfQueues (int numberOfQueues)
{
  NS_LOG_FUNCTION (this << numberOfQueues);
  m_numberOfQueues = numberOfQueues;
}

void
SPQ::SetQueueLevels (std::string queueLevels)
{
  NS_LOG_FUNCTION (this << queueLevels);
  m_queueLevels = queueLevels;
}

// template <typename Item>
SPQ::SPQ () : DiffServ (), NS_LOG_TEMPLATE_DEFINE ("SPQ")
{

  NS_LOG_FUNCTION (this);
}

void
SPQ::Setup (int s)
{
  NS_LOG_FUNCTION (this);

  int i;
  for (i = 0; i < m_numberOfQueues; i++)
    {
      TrafficClass tc = TrafficClass ();
      q_class.push_back (&tc);
      q_class[i]->SetPriorityLevel (m_queueLevels[i]);
      q_class[i]->SetMode (0);
      q_class[i]->SetMaxPackets (500);
    }

  vector<FilterElements *> feLow;
  feLow.push_back (new Destination_Port_Number (9));

  vector<Filter *> fLow;
  fLow.push_back (new Filter ());
  fLow[0]->set (feLow);
  q_class[LOW]->SetFilters (fLow);

  vector<FilterElements *> feHigh;
  feHigh.push_back (new Destination_Port_Number (10));

  vector<Filter *> fHigh;
  fHigh.push_back (new Filter ());
  fHigh[0]->set (feHigh);
  q_class[HIGH]->SetFilters (fHigh);
}

// template <typename Item>
SPQ::~SPQ ()
{
}

// template <typename Item>
Ptr<Packet>
SPQ::Schedule (void)
{
  NS_LOG_FUNCTION (this);
  //check for the high priority queue TC
  Ptr<Packet> p;
  //Dequeue high if not empty
  if (!q_class[HIGH]->isEmpty ())
    {
      p = q_class[HIGH]->Dequeue ();
      return p;
    }
  else
    { //if high is empty
      if (!q_class[LOW]->isEmpty ())
        {
          //dequeue low
          p = q_class[LOW]->Dequeue ();
          return p;
        }
      else
        {
          return 0;
        }
    }
}

// template <typename Item>
uint32_t
SPQ::Classify (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  if (q_class[HIGH]->match (p))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

// template <typename Item>
bool
SPQ::Enqueue (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  //call Classify
  bool res = false;
  uint32_t priorityLevel = Classify (p);
  if (priorityLevel == 1)
    {
      res = q_class[HIGH]->Enqueue (p);
    }
  else
    {
      res = q_class[LOW]->Enqueue (p);
    }
  return res;
}

// template <typename Item>
Ptr<Packet>
SPQ::Dequeue (void)
{
  NS_LOG_FUNCTION (this);
  //call Schedule
  Ptr<Packet> p = Schedule ();
  return p;
}

/**
* Peek the front item in the queue without removing it.
  Retun 0 if queue(s) is(are) empty.
*/
// template <typename Item>
Ptr<const Packet>
SPQ::Peek (void) const
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> p;

  if (!q_class[HIGH]->isEmpty ())
    {
      p = q_class[HIGH]->m_queue.front ();
      return p;
    }
  else
    { //if high is empty
      if (!q_class[LOW]->isEmpty ())
        {
          //dequeue low
          p = q_class[LOW]->m_queue.front ();
          return p;
        }
      else
        { //if both are empty
          return 0;
        }
    }
}

/**
   Pull the item to drop from the queue
  */
// template <typename Item>
Ptr<Packet>
SPQ::Remove (void)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> p;
  //Dequeue high if not empty
  if (!q_class[HIGH]->isEmpty ())
    {
      p = q_class[HIGH]->Dequeue ();
      return p;
    }
  else
    { //if high is empty
      if (!q_class[LOW]->isEmpty ())
        {
          //dequeue low
          p = q_class[LOW]->Dequeue ();
          return p;
        }
      else
        {
          return 0;
        }
    }
  DropAfterDequeue (p);
  return p;
}

// template <typename Item>
void
SPQ::SetMode (SPQ::QueueMode mode)
{
  m_mode = mode;
}

//template <typename Item>
typename SPQ::QueueMode
SPQ::GetMode (void) const
{
  return m_mode;
}

// NS_OBJECT_TEMPLATE_CLASS_DEFINE (SPQ,Packet);
// NS_OBJECT_TEMPLATE_CLASS_DEFINE (SPQ,QueueDiscItem);
} // namespace ns3
