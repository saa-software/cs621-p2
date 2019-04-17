/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "spq.h"

namespace ns3 {

/* ... */
  template <typename Item>
  SPQ<Item>::SPQ() {

  }

  template <typename Item>
  SPQ<Item>::~SPQ() {

  }
  
  Ptr<Packet> Schedule (void) {
    //TODO: Change
    Ptr<Packet> p = Create<Packet> (1024);
    return p;
  }

}
