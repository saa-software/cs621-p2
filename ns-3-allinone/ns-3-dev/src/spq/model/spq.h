/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef SPQ_H
#define SPQ_H

#include "ns3/queue.h"
#include <queue>
#include "ns3/packet.h"
#include <vector>
#include "ns3/diffserv.h"

namespace ns3 {

/* ... */
template <typename Item>
class SPQ: public DiffServ<Item> {
public:
  static TypeId GetTypeId (void);
  SPQ ();
  ~SPQ();
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };
  Ptr<Packet> Schedule (void);
  uint32_t Classify (Ptr<Packet> p);
  void SetMode (SPQ<Item>::QueueMode mode);
  SPQ<Item>::QueueMode GetMode (void) const;

private:
   bool DoEnqueue(Ptr<Packet> p);
   Ptr<Packet> DoDequeue (void);
   Ptr<const Packet> DoPeek (void) const;
   Ptr<Packet> DoRemove (void);
  //std::vector<TrafficClass*> q_class;

  SPQ<Item>::QueueMode m_mode;
};

}

#endif /* SPQ_H */
