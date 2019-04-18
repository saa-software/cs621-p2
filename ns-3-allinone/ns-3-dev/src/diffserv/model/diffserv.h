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
class DiffServ : public Queue<Item>{
public:
  static TypeId GetTypeId (void);
  DiffServ();
  ~DiffServ();
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,
    QUEUE_MODE_BYTES,
  };
  void SetMode (DiffServ<Item>::QueueMode mode);
 //  //do i need constant for this?
  DiffServ<Item>::QueueMode GetMode (void) const;
  virtual Ptr<Packet> Schedule (void);
  virtual uint32_t Classify (Ptr<Packet> p);

private:
  virtual bool DoEnqueue(Ptr<Packet> p);
  virtual Ptr<Packet> DoDequeue (void);
  virtual Ptr<const Packet> DoPeek (void) const;
  virtual Ptr<Packet> DoRemove (void);
  DiffServ<Item>::QueueMode m_mode;



protected:
  std::vector<TrafficClass*> q_class;

};

}

#endif /* DIFFSERV_H */
