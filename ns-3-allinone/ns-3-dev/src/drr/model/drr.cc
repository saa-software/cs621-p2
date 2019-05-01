/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "drr.h"
#include "ns3/filter.h"
#include "ns3/trafficclass.h"
#include "ns3/log.h"
#include "ns3/string.h"

namespace ns3 {

  NS_OBJECT_ENSURE_REGISTERED (DRR);

  const int LOW = 0;
  const int MED = 1;
  const int HIGH = 2;
  //TODO: create a getter/setter?
  const uint32_t QUANTUM = 110;

TypeId
DRR::GetTypeId (void) {
  static TypeId tid = TypeId ("ns3::DRR<Packet>")
    .SetParent<DiffServ<Packet>> ()
    .SetGroupName("Network")
    .template AddConstructor<DRR> ()
    .AddAttribute("Mode",
                  "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                  EnumValue(QUEUE_MODE_PACKETS),
                  MakeEnumAccessor(&DRR::SetMode,&DRR::GetMode),
                  MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
    .AddAttribute("NumberOfQueues", "The number of queues to use in DRR", IntegerValue (0),
                  MakeIntegerAccessor (&DRR::SetNumberOfQueues),
                  MakeIntegerChecker<int> (INT64_MIN, INT64_MAX))
    .AddAttribute("QueueLevels",
                  "A string that represents the level for each queue. Number of chars must "
                  "match queue levels.",
                  StringValue (""), MakeStringAccessor (&DRR::SetQueueQuantums),
                  MakeStringChecker ())
    .AddAttribute("Setup", "Initiates setup for SPQ queue.", IntegerValue (1),
                  MakeIntegerAccessor (&DRR::Setup),
                  MakeIntegerChecker<int> (INT64_MIN, INT64_MAX));
  ;
  return tid;
}


// template <typename Item>
DRR::DRR() :DiffServ(),NS_LOG_TEMPLATE_DEFINE ("DRR") {
  NS_LOG_FUNCTION (this);
}

// template <typename Item>
DRR::~DRR() {}

void
DRR::Setup(int s) {

    NS_LOG_FUNCTION (this);

    int i;
    int port = 9;
    for (i = 0; i < m_numberOfQueues; i++)
      {
        // New traffic class
        q_class.push_back (new TrafficClass ());
        q_class[i]->SetPriorityLevel (m_queueQuantums[i]);
        q_class[i]->SetMode (0);
        q_class[i]->SetMaxPackets (20000);
        // New filter elements vec
        vector<FilterElements *> fe;
        fe.push_back (new Destination_Port_Number (port));
        port = port + 1;
        // Add to filters
        vector<Filter *> f;
        f.push_back (new Filter ());
        f[0]->set (fe);
        q_class[i]->SetFilters (f);
      }

      q_kind = HIGH; //Start with high
      isNewRound = true;
      dc_arr = {0,0,0};
  // q_class.push_back(new TrafficClass());
  // q_class.push_back(new TrafficClass());
  // q_class.push_back(new TrafficClass());

  // q_class[LOW]->SetWeight(LOW);
  // q_class[MED]->SetWeight(MED);
  // q_class[HIGH]->SetWeight(HIGH);

  // q_class[LOW]->SetMode(0);
  // q_class[MED]->SetMode(0);
  // q_class[HIGH]->SetMode(0);

  // q_class[LOW]->SetMaxPackets(500);
  // q_class[MED]->SetMaxPackets(500);
  // q_class[HIGH]->SetMaxPackets(500);

  // vector<FilterElements*> feLow;
  // feLow.push_back(new Destination_Port_Number(9));

  // vector<Filter*> fLow;
  // fLow.push_back(new Filter());
  // fLow[0]->set(feLow);
  // q_class[LOW]->SetFilters(fLow);

  // vector<FilterElements*> feMed;
  // feMed.push_back(new Destination_Port_Number(10));

  // vector<Filter*> fMed;
  // fMed.push_back(new Filter());
  // fMed[0]->set(feMed);
  // q_class[MED]->SetFilters(fMed);

  // vector<FilterElements*> feHigh;
  // feHigh.push_back(new Destination_Port_Number(11));

  // vector<Filter*> fHigh;
  // fHigh.push_back(new Filter());
  // fHigh[0]->set(feHigh);
  // q_class[HIGH]->SetFilters(fHigh);

  // q_kind = HIGH; //Start with high
  // isNewRound = true;
  // dc_arr = {0,0,0};
}

// template <typename Item>
// TypeId
// DRR<Item>::GetTypeId (void) {
//   static TypeId tid = TypeId (("ns3::DRR<" + GetTypeParamName<DRR<Item>>() + ">").c_str())
//     .SetParent<DiffServ<Item>>()
//     .SetGroupName("Network")
//     .template AddConstructor<DRR<Item>> ()
//     .AddAttribute("Mode",
//                   "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
//                   EnumValue(QUEUE_MODE_PACKETS),
//                   MakeEnumAccessor(&DRR<Item>::SetMode,&DRR<Item>::GetMode),
//                   MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
//   ;
//   return tid;
// }
//


  void
  DRR::SetNumberOfQueues (int numberOfQueues)
  {
    NS_LOG_FUNCTION (this << numberOfQueues);
    m_numberOfQueues = numberOfQueues;
  }

  void
  DRR::SetQueueQuantums (std::string queueQuantums)
  {
    NS_LOG_FUNCTION (this << queueQuantums);
    m_queueQuantums = queueQuantums;
  }

// template <typename Item>
Ptr<Packet>
DRR::Schedule (void) {
  NS_LOG_FUNCTION (this);

  NS_LOG_INFO("q_kind: " << q_kind);

  //if all queues are empty, return 0
  if (q_class[HIGH]->isEmpty() && q_class[MED]->isEmpty() && q_class[LOW]->isEmpty()) {
    return 0;
  }

    switch (this->q_kind)
    {
      case HIGH: //HIGH
      NS_LOG_INFO("HIGH: isNewRound : " << isNewRound);
              if(!q_class[HIGH]->isEmpty()) {
                if(isNewRound) { //if scheduling for the first time, inc dc
                  dc_arr[HIGH] += QUANTUM*(HIGH+1);
                  isNewRound = false;
                }
                NS_LOG_INFO("High Deficit Counter: " << dc_arr[HIGH]);
                Ptr<Packet> p = q_class[HIGH]->m_queue.front();
                if(p->GetSize() <= dc_arr[HIGH]) {
                  Ptr<Packet> res = q_class[HIGH]->Dequeue();
                  dc_arr[HIGH] -= res->GetSize();
                  NS_LOG_INFO("Dequeue High");
                  return res;
                } else { //not enough dc, move to MED
                  q_kind = MED;
                  isNewRound = true;
                }
              } else { //if queue is empty
                q_kind = MED;
                isNewRound = true;
              }
      case MED:
        NS_LOG_INFO("MED: isNewRound : " << isNewRound);
              if(!q_class[MED]->isEmpty()) {
                if(isNewRound) { //if scheduling for the first time, inc dc
                  dc_arr[MED] += QUANTUM*(MED+1);
                  isNewRound = false;
                }
                NS_LOG_INFO("Med Deficit Counter: " << dc_arr[MED]);
                Ptr<Packet> p = q_class[MED]->m_queue.front();
                if(p->GetSize() <= dc_arr[MED]) {
                  Ptr<Packet> res = q_class[MED]->Dequeue();
                  dc_arr[MED] -= res->GetSize();
                  NS_LOG_INFO("Dequeue Med");
                  return res;
                } else {
                  q_kind = LOW;
                  isNewRound = true;
                }
              } else { //if queue is empty
                q_kind = LOW;
                isNewRound = true;
              }
      case LOW:
          NS_LOG_INFO("LOW: isNewRound : "<< isNewRound);
              if(!q_class[LOW]->isEmpty()) {
                if(isNewRound) { //if scheduling for the first time, inc dc
                  dc_arr[LOW] += QUANTUM*(LOW+1);
                  isNewRound = false;
                }
                NS_LOG_INFO("Low Deficit Counter: " << dc_arr[LOW]);
                Ptr<Packet> p = q_class[LOW]->m_queue.front();
                if(p->GetSize() <= dc_arr[LOW]) {
                  Ptr<Packet> res = q_class[LOW]->Dequeue();
                  dc_arr[LOW] -= res->GetSize();
                  NS_LOG_INFO("Dequeue LOW");
                  return res;
                } else {
                  q_kind = HIGH;
                  isNewRound = true;
                }
              } else { //if queue is empty
                q_kind = HIGH;
                isNewRound = true;
              }
        default:
          return 0;
        }

      return 0;
}

//template <typename Item>
uint32_t
DRR::Classify (Ptr<Packet> p) {
  NS_LOG_FUNCTION (this);
  if(q_class[HIGH]->match(p)) {
    NS_LOG_FUNCTION ("classify 2");
    return 2;
  } else if(q_class[MED]->match(p)) {
    NS_LOG_FUNCTION ("classify 1");
    return 1;
  } else {
    NS_LOG_FUNCTION ("classify 0");
    return 0;
  }
}

// template <typename Item>
bool
DRR::Enqueue(Ptr<Packet> p) {
  NS_LOG_FUNCTION (this << p);
  uint32_t weight = Classify(p);
  NS_LOG_INFO("weight: " << weight);
  if(weight == 2) {
    NS_LOG_INFO("Enqueue HIGH");
    return q_class[HIGH]->Enqueue(p);
  } else if (weight == 1) {
    NS_LOG_INFO("Enqueue MED");
    return q_class[MED]->Enqueue(p);
  } else {
    NS_LOG_INFO("Enqueue LOW");
    return q_class[LOW]->Enqueue(p);
  }
}


Ptr<Packet>
DRR::Dequeue (void) {
  NS_LOG_FUNCTION (this);
  Ptr<Packet> p;
  while (!q_class[HIGH]->isEmpty() || !q_class[MED]->isEmpty() || !q_class[LOW]->isEmpty()) {
    p = Schedule();
    if(p != 0) {
      return p;
    }
  }

  return p;
}

// template <typename Item>
Ptr<const Packet>
DRR::Peek (void) const {
  NS_LOG_FUNCTION (this);

  //if all queues are empty, return 0
  if (q_class[HIGH]->isEmpty() && q_class[MED]->isEmpty() && q_class[LOW]->isEmpty()) {
    return 0;
  }

  int peek_q_kind = this->q_kind;
  bool peek_is_new = this->isNewRound;

  Ptr<Packet> p;
  while (true) {
    switch (peek_q_kind)
    {
      case HIGH:
            if(!q_class[HIGH]->isEmpty()) {
              p = q_class[HIGH]->m_queue.front();
              if(peek_is_new) {
                if(dc_arr[HIGH]+QUANTUM*(HIGH+1) >= p->GetSize()){
                  return p;
                } else {
                  peek_q_kind = MED;
                  //peek_is_new is still true
                }
              } else { // !peek_is_new
                if(dc_arr[HIGH] >=  p->GetSize()) {
                  return p;
                } else {
                  peek_q_kind = MED;
                  peek_is_new = true;
                }
              }
            } else { //if queue is empty
              peek_q_kind = MED;
            }
              break;
      case MED:
            if(!q_class[MED]->isEmpty()) {
              p = q_class[MED]->m_queue.front();
              if(peek_is_new) {
                if(dc_arr[MED]+QUANTUM*(MED+1) >= p->GetSize()){
                  return p;
                } else {
                  peek_q_kind = LOW;
                  //peek_is_new is still true
                }
              } else { // !peek_is_new
                if(dc_arr[MED] >= p->GetSize()) {
                  return p;
                } else {
                  peek_q_kind = LOW;
                  peek_is_new = true;
                }
              }
            } else { //if queue is empty
              peek_q_kind = LOW;
            }
              break;
      case LOW:
            if(!q_class[LOW]->isEmpty()) {
              p = q_class[LOW]->m_queue.front();
              if(peek_is_new) {
                if(dc_arr[LOW]+QUANTUM*(LOW+1) >= p->GetSize()){
                  return p;
                } else {
                  peek_q_kind = HIGH;
                  //peek_is_new is still true
                }
              } else { // !peek_is_new
                if(dc_arr[LOW] >= p->GetSize()) {
                  return p;
                } else {
                  peek_q_kind = HIGH;
                  peek_is_new = true;
                }
              }
            } else { //if queue is empty
              peek_q_kind = HIGH;
            }
              break;
        default:
          return 0;
    }
  }
  return 0;//never reached

}

// template <typename Item>
Ptr<Packet>
DRR::Remove (void) {
  Ptr<Packet> p = Dequeue();
  DropAfterDequeue(p);
  return p;
}

// template <typename Item>
void
DRR::SetMode (DRR::QueueMode mode) {
  m_mode = mode;
}

// template <typename Item>
typename DRR::QueueMode
DRR::GetMode (void) const {
  return m_mode;
}

}
