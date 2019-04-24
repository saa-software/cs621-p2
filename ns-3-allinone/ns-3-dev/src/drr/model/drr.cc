/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "drr.h"

namespace ns3 {

  const int LOW = 0;
  const int MED = 1;
  const int HIGH = 2;
  //TODO: create a getter/setter?
  const uint32_t QUANTUM = 100;

//template <typename Item>
DRR::DRR() {
  q_kind = 2;
  isNewRound = true;
  dc_arr = {0,0,0};
}

//template <typename Item>
DRR::~DRR() {

}

//template <typename Item>
TypeId
DRR::GetTypeId (void) {
  static TypeId tid = TypeId (("ns3::DRR"))
    .SetParent<DiffServ>()
    .SetGroupName("Network")
    .AddConstructor<DRR> ()
    .AddAttribute("Mode",
                  "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                  EnumValue(QUEUE_MODE_PACKETS),
                  MakeEnumAccessor(&DRR::SetMode,&DRR::GetMode),
                  MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
  ;
  return tid;
}
//
// template <typename Item>
Ptr<Packet>
DRR::Schedule (void) {
  switch (q_kind)
  {
    case 2:
            if(isNewRound) { //if scheduling for the first time, inc dc
              dc_arr[HIGH] += QUANTUM*(HIGH+1);
              isNewRound = false;
            }
            if(!q_class[HIGH]->isEmpty()) {
              Ptr<Packet> p = q_class[HIGH]->m_queue.front();
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
              Ptr<Packet> p = q_class[MED]->m_queue.front();
              if(p->GetSize() <= dc_arr[MED]) {
                Ptr<Packet> res = q_class[MED]->Dequeue();
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
              Ptr<Packet> p = q_class[LOW]->m_queue.front();
              if(p->GetSize() <= dc_arr[LOW]) {
                Ptr<Packet> res = q_class[LOW]->Dequeue();
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


uint32_t
DRR::Classify (Ptr<Packet> p) {
  if(q_class[HIGH]->match(p)) {
    return 2;
  } else if(q_class[MED]->match(p)) {
    return 1;
  } else {
    return 0;
  }
}


bool
DRR::DoEnqueue(Ptr<Packet> p) {
  uint32_t weight = Classify(p);
  if(weight == 2) {
    return q_class[HIGH]->Enqueue(p);
  } else if (weight == 1) {
    return q_class[MED]->Enqueue(p);
  } else {
    return q_class[LOW]->Enqueue(p);
  }
}
//
// template <typename Item>
// Ptr<Packet>
// DRR<Item>::DoDequeue (void) {
//
// }
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

void
DRR::SetMode (DRR::QueueMode mode) {
  m_mode = mode;
}

//template <typename Item>
typename DRR::QueueMode
DRR::GetMode (void) const {
  return m_mode;
}

}
