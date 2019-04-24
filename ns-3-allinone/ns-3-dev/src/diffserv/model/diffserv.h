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
//template <typename Item>
class DiffServ : public Queue<Packet>{
public:
  static TypeId GetTypeId (void);
  DiffServ();
  ~DiffServ();
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };
  void SetMode (DiffServ::QueueMode mode);
 //  //do i need constant for this?
  DiffServ::QueueMode GetMode (void) const;
  virtual Ptr<Packet> Schedule (void)=0;
  virtual uint32_t Classify (Ptr<Packet> p)=0;
  virtual Ptr<Packet> Dequeue (void);
  virtual bool Enqueue (Ptr<Packet> p);
  virtual Ptr<const Packet> Peek (void) const;
  virtual Ptr<Packet> Remove (void);

private:
  // virtual bool DoEnqueue(Ptr<Packet> p);
  // virtual Ptr<Packet> DoDequeue (void);
  // virtual Ptr<const Packet> DoPeek (void) const;
  // virtual Ptr<Packet> DoRemove (void);
  using Queue<Packet>::Head;
  using Queue<Packet>::Tail;
  using Queue<Packet>::DoEnqueue;
  using Queue<Packet>::DoDequeue;
  using Queue<Packet>::DoRemove;
  using Queue<Packet>::DoPeek;

protected:
  std::vector<TrafficClass*> q_class;
  QueueMode m_mode;

};

}

#endif /* DIFFSERV_H */
