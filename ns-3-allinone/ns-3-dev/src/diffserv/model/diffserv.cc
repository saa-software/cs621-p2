/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "diffserv.h"

namespace ns3 {

  template <typename Item>
  DiffServ<Item>::DiffServ() {

  }
  template <typename Item>
  DiffServ<Item>::~DiffServ() {

  }

  template <typename Item>
  void
  DiffServ<Item>::SetMode (DiffServ<Item>::QueueMode mode) {
    m_mode = mode;
  }

  template <typename Item>
  typename DiffServ<Item>::QueueMode
  DiffServ<Item>::GetMode (void) const {
    return m_mode;
  }


  template <typename Item>
  Ptr<Packet>
  DiffServ<Item>::Schedule (void) {
    //TODO: Change
    Ptr<Packet> p = Create<Packet> (1024);
    return p;
  }

  template <typename Item>
  uint32_t
  DiffServ<Item>::Classify (Ptr<Packet> p) {
    //TODO: CHANGE THIS
    return 0;
  }

}
