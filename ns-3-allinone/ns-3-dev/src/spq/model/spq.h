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
// template <typename Item>

class SPQ : public DiffServ<ns3::Packet> {
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
  void SetMode (SPQ::QueueMode mode);
  SPQ::QueueMode GetMode (void) const;
  void SetNumberOfQueues (int numberOfQueues);
  void SetQueueLevels (std::string queueLevels);
  void Setup (int s);

private:
  virtual bool Enqueue(Ptr<Packet> p);
  virtual Ptr<Packet> Dequeue (void);
  virtual Ptr<const Packet> Peek (void) const;
  virtual Ptr<Packet> Remove (void);
  std::vector<TrafficClass *> q_class;
  QueueMode m_mode;
  int m_numberOfQueues;
  std::string m_queueLevels;

  NS_LOG_TEMPLATE_DECLARE;
};

// extern template class SPQ<Packet>;
// extern template class SPQ<QueueDiscItem>;
}

#endif /* SPQ_H */
