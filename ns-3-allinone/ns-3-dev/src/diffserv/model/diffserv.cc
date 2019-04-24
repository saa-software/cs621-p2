/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "diffserv.h"

namespace ns3 {


  DiffServ::DiffServ() {

  }

  DiffServ::~DiffServ() {

  }

  TypeId
  DiffServ::GetTypeId (void) {
    static TypeId tid = TypeId (("ns3::DiffServ"))
      .SetParent<Queue<Packet>>()
      .SetGroupName("Network")
    ;
    return tid;
  }

  void
  DiffServ::SetMode (DiffServ::QueueMode mode) {
    m_mode = mode;
  }


  typename DiffServ::QueueMode
  DiffServ::GetMode (void) const {
    return m_mode;
  }

  Ptr<Packet>
  DiffServ::Dequeue (void){
    Ptr<Packet> p = DoDequeue(Head());
    return p;
  }

  bool
  DiffServ::Enqueue(Ptr<Packet> p) {
    return DoEnqueue(Tail(), p);
  }

  Ptr<const Packet>
  DiffServ::Peek (void) const {
    return DoPeek(Head());
  }

  Ptr<Packet>
  DiffServ::Remove (void){
    Ptr<Packet> p = DoRemove(Head());
    return p;
  }

}
