/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DIFFSERV_H
#define DIFFSERV_H

 #include "ns3/queue.h"
 #include <queue>
 #include "ns3/packet.h"
 #include <vector>
 #include "trafficclass.h"


namespace ns3 {

/* ... */
template <typename Item>
class DiffServ : public Queue<Item> {
public:
  static TypeId GetTypeId (void);
  DiffServ();
  virtual ~DiffServ();
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };
  void SetMode (DiffServ::QueueMode mode);
 //  //do i need constant for this?
  DiffServ::QueueMode GetMode (void) const;
  virtual Ptr<Item> Schedule (void)=0;
  virtual uint32_t Classify (Ptr<Item> p)=0;
  virtual Ptr<Item> Dequeue (void);
  virtual bool Enqueue (Ptr<Item> p);
  virtual Ptr<const Item> Peek (void) const;
  virtual Ptr<Item> Remove (void);

private:
  // virtual bool DoEnqueue(Ptr<Packet> p);
  // virtual Ptr<Packet> DoDequeue (void);
  // virtual Ptr<const Packet> DoPeek (void) const;
  // virtual Ptr<Packet> DoRemove (void);
  using Queue<Item>::Head;
  using Queue<Item>::Tail;
  using Queue<Item>::DoEnqueue;
  using Queue<Item>::DoDequeue;
  using Queue<Item>::DoRemove;
  using Queue<Item>::DoPeek;

protected:
  std::vector<TrafficClass*> q_class;
  QueueMode m_mode;

};

extern template class DiffServ<Packet>;
extern template class DiffServ<QueueDiscItem>;

}

#endif /* DIFFSERV_H */
