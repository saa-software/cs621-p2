/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DRR_H
#define DRR_H

#include "ns3/queue.h"
#include <queue>
#include "ns3/packet.h"
#include <vector>
#include "ns3/diffserv.h"

namespace ns3 {


template <typename Item>
class DRR: public DiffServ<Item> {
public:
  static TypeId GetTypeId (void);
  DRR();
  ~DRR();
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };

  Ptr<Item> Schedule (void);
  uint32_t Classify (Ptr<Item> p);
  void SetMode (DRR<Item>::QueueMode mode);
  DRR<Item>::QueueMode GetMode (void) const;

private:
  bool DoEnqueue(Ptr<Item> p);
  Ptr<Item> DoDequeue (void);
  // Ptr<const Item> DoPeek (void) const;
  // Ptr<Item> DoRemove (void);

  std::vector<TrafficClass*> q_class;
  QueueMode m_mode;
  int q_kind;
  bool isNewRound;
  std::vector<uint32_t> dc_arr;

};

}

#endif /* DRR_H */
