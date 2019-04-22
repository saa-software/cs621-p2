/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "drr.h"

namespace ns3 {

/* ... */
template <typename Item>
DRR<Item>::DRR() {

}

template <typename Item>
DRR<Item>::~DRR() {

}

template <typename Item>
TypeId
DRR<Item>::GetTypeId (void) {
  static TypeId tid = TypeId (("ns3::DRR<" + GetTypeParamName<DRR<Item>>() + ">").c_str())
    .SetParent<Queue<Item>>()
    .SetGroupName("Network")
    .template AddConstructor<DRR<Item>> ()
  ;
  return tid;
}
//
// template <typename Item>
// Ptr<Packet>
// DRR<Item>::Schedule (void) {
//
// }
//
// template <typename Item>
// uint32_t
// DRR<Item>::Classify (Ptr<Packet> p) {
//
// }
//
// template <typename Item>
// bool
// DRR<Item>::DoEnqueue(Ptr<Packet> p) {
//
// }
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

}
