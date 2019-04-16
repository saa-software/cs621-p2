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
  Ptr<Packet> Schedule (void);

private:

};

}

#endif /* SPQ_H */
