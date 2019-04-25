/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "drr.h"

namespace ns3 {

  const int LOW = 0;
  const int MED = 1;
  const int HIGH = 2;
  //TODO: create a getter/setter?
  const uint32_t QUANTUM = 100;

template <typename Item>
DRR<Item>::DRR() {
  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());
  q_class[LOW]->SetWeight(LOW);
  q_class[MED]->SetWeight(MED);
  q_class[HIGH]->SetWeight(HIGH);

  q_kind = 2;
  isNewRound = true;
  dc_arr = {0,0,0};
}

template <typename Item>
DRR<Item>::~DRR() {

}

template <typename Item>
TypeId
DRR<Item>::GetTypeId (void) {
  static TypeId tid = TypeId (("ns3::DRR<" + GetTypeParamName<DRR<Item>>() + ">").c_str())
    .SetParent<DiffServ<Item>>()
    .SetGroupName("Network")
    .template AddConstructor<DRR<Item>> ()
    .AddAttribute("Mode",
                  "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                  EnumValue(QUEUE_MODE_PACKETS),
                  MakeEnumAccessor(&DRR<Item>::SetMode,&DRR<Item>::GetMode),
                  MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
  ;
  return tid;
}
//
template <typename Item>
Ptr<Item>
DRR<Item>::Schedule (void) {
  switch (this->q_kind)
  {
    case 2:
            if(isNewRound) { //if scheduling for the first time, inc dc
              dc_arr[HIGH] += QUANTUM*(HIGH+1);
              isNewRound = false;
            }
            if(!q_class[HIGH]->isEmpty()) {
              Ptr<Item> p = q_class[HIGH]->m_queue.front();
              if(p->GetSize() <= dc_arr[HIGH]) {
                Ptr<Packet> res = q_class[HIGH]->Dequeue();
                dc_arr[HIGH] -= res->GetSize();
                return res;
              } else {
                q_kind = 1;
                isNewRound = true;
              }
            }
            break;
    case 1:
            if(isNewRound) { //if scheduling for the first time, inc dc
              dc_arr[MED] += QUANTUM*(MED+1);
              isNewRound = false;
            }
            if(!q_class[MED]->isEmpty()) {
              Ptr<Item> p = q_class[MED]->m_queue.front();
              if(p->GetSize() <= dc_arr[MED]) {
                Ptr<Item> res = q_class[MED]->Dequeue();
                dc_arr[MED] -= res->GetSize();
                return res;
              } else {
                q_kind = 0;
                isNewRound = true;
              }
            }
            break;
    case 0:
            if(isNewRound) { //if scheduling for the first time, inc dc
              dc_arr[LOW] += QUANTUM*(LOW+1);
              isNewRound = false;
            }
            if(!q_class[LOW]->isEmpty()) {
              Ptr<Item> p = q_class[LOW]->m_queue.front();
              if(p->GetSize() <= dc_arr[LOW]) {
                Ptr<Item> res = q_class[LOW]->Dequeue();
                dc_arr[LOW] -= res->GetSize();
                return res;
              } else {
                q_kind = 2;
                isNewRound = true;
              }
            }
            break;
      default:
        return 0;
      }
      return 0;
}

template <typename Item>
uint32_t
DRR<Item>::Classify (Ptr<Item> p) {
  if(q_class[HIGH]->match(p)) {
    return 2;
  } else if(q_class[MED]->match(p)) {
    return 1;
  } else {
    return 0;
  }
}

template <typename Item>
bool
DRR<Item>::DoEnqueue(Ptr<Item> p) {
  uint32_t weight = Classify(p);
  if(weight == 2) {
    return q_class[HIGH]->Enqueue(p);
  } else if (weight == 1) {
    return q_class[MED]->Enqueue(p);
  } else {
    return q_class[LOW]->Enqueue(p);
  }
}

template <typename Item>
Ptr<Item>
DRR<Item>::DoDequeue (void) {
  Ptr<Item> p = Schedule();
  return p;
}
//
// template <typename Item>
// Ptr<const Packet>
// DRR<Item>::DoPeek (void) const {
//
// }
//
// template <typename Item>
// Ptr<Packet>
// DRR<Item>::DoRemove (void) {
//   //TODO: Change
//   Ptr<Packet> p = Create<Packet> (1024);
//   return p;
// }

template <typename Item>
void
DRR<Item>::SetMode (DRR<Item>::QueueMode mode) {
  m_mode = mode;
}

template <typename Item>
typename DRR<Item>::QueueMode
DRR<Item>::GetMode (void) const {
  return m_mode;
}

}
