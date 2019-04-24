/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "spq.h"

namespace ns3 {

/* ... */
  const int LOW = 0;
  const int HIGH = 1;

  template <typename Item>
  TypeId
  SPQ<Item>::GetTypeId (void) {
    static TypeId tid = TypeId (("ns3::SPQ<" + GetTypeParamName<SPQ<Item> > () + ">").c_str ())
      .SetParent<DiffServ<Item> >()
      .SetGroupName("Network")
      .template AddConstructor<SPQ<Item>> ()
      .AddAttribute("Mode",
                    "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                    EnumValue(QUEUE_MODE_PACKETS),
                    MakeEnumAccessor(&SPQ<Item>::SetMode,&SPQ<Item>::GetMode),
                    MakeEnumChecker(QUEUE_MODE_BYTES,"QUEUE_MODE_BYTES", QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))

    ;
    return tid;
  }

  template <typename Item>
  SPQ<Item>::SPQ () {

  }

  template <typename Item>
  SPQ<Item>::~SPQ () {

  }

  template <typename Item>
  Ptr<Item>
  SPQ<Item>::Schedule (void) {
    //check for the high priority queue TC
    Ptr<Item> p;
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

  template <typename Item>
  uint32_t
  SPQ<Item>::Classify (Ptr<Item> p) {
    if(q_class[HIGH]->match(p)) {
      return 1;
    } else {
      return 0;
    }
  }

  template <typename Item>
  bool
  SPQ<Item>::DoEnqueue(Ptr<Item> p) {
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

  template <typename Item>
  Ptr<Item>
  SPQ<Item>::DoDequeue (void) {
    //call Schedule
    Ptr<Item> p = Schedule();
    return p;
  }

/**
* Peek the front item in the queue without removing it.
  Retun 0 if queue(s) is(are) empty.
*/
  template <typename Item>
  Ptr<const Item>
  SPQ<Item>::DoPeek (void) const {
    Ptr<Item> p;

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
  template <typename Item>
  Ptr<Item>
  SPQ<Item>::DoRemove (void) {
    Ptr<Item> p;
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


  template <typename Item>
  void
  SPQ<Item>::SetMode (SPQ<Item>::QueueMode mode) {
    m_mode = mode;
  }

  template <typename Item>
  typename SPQ<Item>::QueueMode
  SPQ<Item>::GetMode (void) const {
    return m_mode;
  }

  NS_OBJECT_TEMPLATE_CLASS_DEFINE (SPQ,Packet);
  // NS_OBJECT_TEMPLATE_CLASS_DEFINE (SPQ,QueueDiscItem);
}
