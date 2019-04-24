/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "spq.h"

namespace ns3 {

/* ... */
  const int LOW = 0;
  const int HIGH = 1;

//  template <typename Item>
  SPQ::SPQ() {

  }

  // template <typename Item>
  SPQ::~SPQ() {

  }

  // template <typename Item>
  TypeId
  SPQ::GetTypeId (void) {
    static TypeId tid = TypeId (("ns3::SPQ"))
      .SetParent<DiffServ>()
      .SetGroupName("Network")
      .AddConstructor<SPQ> ()
      .AddAttribute("Mode",
                    "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                    EnumValue(QUEUE_MODE_PACKETS),
                    MakeEnumAccessor(&SPQ::SetMode,&SPQ::GetMode),
                    MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
    ;
    return tid;
  }

  //template <typename Item>
  Ptr<Packet>
  SPQ::Schedule (void) {
    //check for the high priority queue TC
    Ptr<Packet> p;
    //Dequeue high if not empty
    if(!q_class[HIGH]->isEmpty()) {
      p = q_class[HIGH]->Dequeue();
      return p;
    } else { //if high is empty
      if(!q_class[LOW]->isEmpty()) {
        //dequeue low
        p = q_class[LOW]->Dequeue();
        return p;
      } else {
        return 0;
      }
    }

  }

  //template <typename Item>
  uint32_t
  SPQ::Classify (Ptr<Packet> p) {
    if(q_class[HIGH]->match(p)) {
      return 1;
    } else {
      return 0;
    }
  }

  //template <typename Item>
  bool
  SPQ::DoEnqueue(Ptr<Packet> p) {
    //call Classify
    bool res;
    uint32_t priorityLevel = Classify(p);
    if(priorityLevel == 1) {
      res = q_class[HIGH]->Enqueue(p);
    } else {
      res = q_class[LOW]->Enqueue(p);
    }

    return res;
  }

//  template <typename Item>
  Ptr<Packet>
  SPQ::DoDequeue (void) {
    //call Schedule
    Ptr<Packet> p = Schedule();
    return p;
  }

/**
* Peek the front item in the queue without removing it.
  Retun 0 if queue(s) is(are) empty.
*/
//  template <typename Item>
  Ptr<const Packet>
  SPQ::DoPeek (void) const {
    Ptr<Packet> p;

    if(!q_class[HIGH]->isEmpty()) {
     p = q_class[HIGH]->m_queue.front();
     return p;
    } else { //if high is empty
      if(!q_class[LOW]->isEmpty()) {
        //dequeue low
        p = q_class[LOW]->m_queue.front();
        return p;
      } else { //if both are empty
        return 0;
      }
    }
  }


  /**
   Pull the item to drop from the queue
  */
  //template <typename Item>
  Ptr<Packet>
  SPQ::DoRemove (void) {
    Ptr<Packet> p;
    //Dequeue high if not empty
    if(!q_class[HIGH]->isEmpty()) {
      p = q_class[HIGH]->Dequeue();
      return p;
    } else { //if high is empty
      if(!q_class[LOW]->isEmpty()) {
        //dequeue low
        p = q_class[LOW]->Dequeue();
        return p;
      } else {
        return 0;
      }
    }
    return p;
  }


//  template <typename Item>
  void
  SPQ::SetMode (SPQ::QueueMode mode) {
    m_mode = mode;
  }

  //template <typename Item>
  typename SPQ::QueueMode
  SPQ::GetMode (void) const {
    return m_mode;
  }

// NS_OBJECT_TEMPLATE_CLASS_DEFINE (SPQ,Packet);
// NS_OBJECT_TEMPLATE_CLASS_DEFINE (SPQ,QueueDiscItem);
}
