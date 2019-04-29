/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DRR_H
#define DRR_H

#include "ns3/queue.h"
#include <queue>
#include "ns3/packet.h"
#include <vector>
#include "ns3/diffserv.h"

namespace ns3 {


//template <typename Item>
class DRR: public DiffServ<Packet> {
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
  void SetNumberOfQueues (int numberOfQueues);
  void SetQueueQuantums (std::string queueLevels);
  void Setup (int s);

private:
  virtual bool Enqueue(Ptr<Packet> p);
  virtual Ptr<Packet> Dequeue (void);
  virtual Ptr<const Packet> Peek (void) const;
  virtual Ptr<Packet> Remove (void);

  std::vector<TrafficClass*> q_class;
  QueueMode m_mode;
  int q_kind;
  bool isNewRound;
  std::vector<uint32_t> dc_arr;
  int m_numberOfQueues;
  std::string m_queueQuantums;

  NS_LOG_TEMPLATE_DECLARE;
};

}

#endif /* DRR_H */
