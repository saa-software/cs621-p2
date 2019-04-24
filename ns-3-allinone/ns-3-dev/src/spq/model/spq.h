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

class SPQ : public DiffServ<Item> {
public:
  static TypeId GetTypeId (void);

  SPQ ();
  ~SPQ();

  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };
  Ptr<Item> Schedule (void);
  uint32_t Classify (Ptr<Item> p);
  void SetMode (SPQ<Item>::QueueMode mode);
  SPQ<Item>::QueueMode GetMode (void) const;

private:
   bool DoEnqueue(Ptr<Item> p);
   Ptr<Item> DoDequeue (void);
   Ptr<const Item> DoPeek (void) const;
   Ptr<Item> DoRemove (void);
   std::vector<TrafficClass*> q_class;

  QueueMode m_mode;
};

extern template class SPQ<Packet>;
// extern template class SPQ<QueueDiscItem>;
}

#endif /* SPQ_H */
