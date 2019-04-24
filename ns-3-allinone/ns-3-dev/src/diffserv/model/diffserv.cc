/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "diffserv.h"

namespace ns3 {

  template <typename Item>

  TypeId
  DiffServ<Item>::GetTypeId (void) {
    static TypeId tid = TypeId (("ns3::DiffServ<" + GetTypeParamName<DiffServ<Item> > () + ">").c_str ())
      .SetParent<Queue<Item>>()
      .SetGroupName("Network")
    ;
    return tid;
  }

  template <typename Item>
  DiffServ<Item>::DiffServ () {

  }

  template <typename Item>
  DiffServ<Item>::~DiffServ () {

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
  Ptr<Item>
  DiffServ<Item>::Dequeue (void){
    Ptr<Item> p = DoDequeue(Head());
    return p;
  }

  template <typename Item>
  bool
  DiffServ<Item>::Enqueue(Ptr<Item> p) {
    return DoEnqueue(Tail(), p);
  }

  template <typename Item>
  Ptr<const Item>
  DiffServ<Item>::Peek (void) const {
    return DoPeek(Head());
  }

  template <typename Item>
  Ptr<Item>
  DiffServ<Item>::Remove (void){
    Ptr<Item> p = DoRemove(Head());
    return p;
  }

  NS_OBJECT_TEMPLATE_CLASS_DEFINE (DiffServ,Packet);
  NS_OBJECT_TEMPLATE_CLASS_DEFINE (DiffServ,QueueDiscItem);

}
