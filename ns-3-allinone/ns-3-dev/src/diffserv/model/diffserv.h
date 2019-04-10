/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DIFFSERV_H
#define DIFFSERV_H

 #include "ns3/queue.h"
 #include <queue>
 #include "ns3/packet.h"

 #include <vector>


namespace ns3 {

/* ... */
template <typename Item>
class DiffServ : public Queue<Item>{
public:
  DiffServ();
  ~DiffServ();
//  void SetMode (DiffServ::QueueMode mode);
  //do i need constant for this?
//  DiffServ::QueueMode GetMode (void) const;
  Ptr<Packet> Schedule (void);
  uint32_t Classify (Ptr<Packet> p);
private:
  virtual bool DoEnqueue(Ptr<Packet> p);
  virtual Ptr<Packet> DoDequeue (void);
  virtual Ptr<const Packet> DoPeek (void) const;
  virtual Ptr<Packet> DoRemove (void);

//  QueueMode m_mode;
  std::vector<TrafficClass*> q_class;

};

}

#endif /* DIFFSERV_H */
