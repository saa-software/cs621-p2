/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-header.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

class FilterElements
{
public:
  bool virtual match (Ptr<Packet> p);
};

class Filter
{
public:
  std::vector<FilterElements> elements;

  bool match (Ptr<Packet> p);
};

class Source_Ip_Address : public FilterElements
{
private:
  Ipv4Address value;

public:
  bool match (Ptr<Packet> p);
};

class Source_Port_Number : public FilterElements
{
private:
  uint16_t value;

public:
  bool match (Ptr<Packet> p);
};

class Source_Mask : public FilterElements
{
private:
  Ipv4Mask value;

public:
  bool match (Ptr<Packet> p);
};

class Destination_Ip_Address : public FilterElements
{
private:
  Ipv4Address value;

public:
  bool match (Ptr<Packet> p);
};

class Destination_Port_Number : public FilterElements
{
private:
  uint16_t value;

public:
  bool match (Ptr<Packet> p);
};

class Destination_Mask : public FilterElements
{
private:
  Ipv4Mask value;

public:
  bool match (Ptr<Packet> p);
};

class Protocol_Number : public FilterElements
{
private:
  uint32_t value;

public:
  bool match (Ptr<Packet> p);
};
} // namespace ns3

#endif /* FILTER_H */