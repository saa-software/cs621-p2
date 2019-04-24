/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DRR_H
#define DRR_H

#include "ns3/queue.h"
#include <queue>
#include "ns3/packet.h"
#include <vector>
#include "ns3/diffserv.h"

namespace ns3 {

/*
//template <typename Item>
class DRR: public DiffServ {
public:
  static TypeId GetTypeId (void);
  DRR();
  ~DRR();
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };
  Ptr<Packet> Schedule (void);
  uint32_t Classify (Ptr<Packet> p);
  void SetMode (DRR::QueueMode mode);
  DRR::QueueMode GetMode (void) const;

private:
  bool DoEnqueue(Ptr<Packet> p);
  Ptr<Packet> DoDequeue (void);
  Ptr<const Packet> DoPeek (void) const;
  Ptr<Packet> DoRemove (void);

};
*/
}

#endif /* DRR_H */
