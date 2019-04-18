/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "spq.h"

namespace ns3 {

/* ... */
  const int LOW = 0;
  const int HIGH = 1;

  template <typename Item>
  SPQ<Item>::SPQ() {

  }

  template <typename Item>
  SPQ<Item>::~SPQ() {

  }

  template <typename Item>
  Ptr<Packet>
  SPQ<Item>::Schedule (void) {
    //check for the high priority queue TC
    Ptr<Packet> p;
    //Dequeue high if not empty
    if(!DiffServ<Item>::q_class[HIGH]->isEmpty()) {
      p = DiffServ<Item>::q_class[HIGH]->Dequeue();
    } else { //if high is empty
      if(!DiffServ<Item>::q_class[LOW]->isEmpty()) {
        //dequeue low
        p = DiffServ<Item>::q_class[LOW]->Dequeue();
      }
    }
    return p;
  }

  template <typename Item>
  uint32_t
  SPQ<Item>::Classify (Ptr<Packet> p) {
    if(DiffServ<Item>::q_class[LOW]->match(p)) {
      return 0;
    } else {
      return 1;
    }
  }

  template <typename Item>
  bool
  SPQ<Item>::DoEnqueue(Ptr<Packet> p) {
    //call Classify
    bool res;
    uint32_t priorityLevel = Classify(p);
    if(priorityLevel == 1) {
      res = DiffServ<Item>::q_class[HIGH]->Enqueue(p);
    } else {
      res = DiffServ<Item>::q_class[LOW]->Enqueue(p);
    }
    //TODO: chekc for the max packets/bytes size
    //WHEN TO drop packets??!?
    return res;
  }

  template <typename Item>
  Ptr<Packet>
  SPQ<Item>::DoDequeue (void) {
    //call Schedule
    Ptr<Packet> p = Schedule();
    return p;
  }

  template <typename Item>
  Ptr<const Packet>
  SPQ<Item>::DoPeek (void) const {
    //TODO: Change
    Ptr<Packet> p = Create<Packet> (1024);
    return p;
  }

  template <typename Item>
  Ptr<Packet>
  SPQ<Item>::DoRemove (void) {
    //TODO: Change
    Ptr<Packet> p = Create<Packet> (1024);
    return p;
  }


  // template <typename Item>
  // void
  // SPQ<Item>::SetMode (SPQ<Item>::QueueMode mode) {
  //   m_mode = mode;
  // }
  //
  // template <typename Item>
  // typename SPQ<Item>::QueueMode
  // SPQ<Item>::GetMode (void) const {
  //   return m_mode;
  // }
}
