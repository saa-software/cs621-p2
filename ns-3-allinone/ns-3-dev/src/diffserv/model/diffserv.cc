/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "diffserv.h"

namespace ns3 {

  template <typename Item>
  DiffServ<Item>::DiffServ() {

  }
  template <typename Item>
  DiffServ<Item>::~DiffServ() {

  }

  // void SetMode (DiffServ::QueueMode mode) {
  //   m_mode = mode;
  // }
  //
  // DiffServ::QueueMode GetMode (void) const {
  //   return m_mode;
  // }


  //template <typename Item>
  Ptr<Packet> Schedule (void) {
    //TODO: Change
    Ptr<Packet> p = Create<Packet> (1024);
    return p;
  }

  template <typename Item>
  uint32_t Classify (Ptr<Packet> p) {
    //TODO: CHANGE THIS
    return 0;
  }

}
