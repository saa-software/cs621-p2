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
  virtual bool match (Ptr<Packet> p) = 0;
};

class Filter
{
public:
  std::vector<FilterElements*> elements;

  bool match (Ptr<Packet> p);
  void set(std::vector<FilterElements*> e);
};

class Source_Ip_Address : public FilterElements
{
private:
  Ipv4Address value;

public:
  bool match (Ptr<Packet> p);
  void set (Ipv4Address ip);
};

class Source_Port_Number : public FilterElements
{
private:
  uint16_t value;

public:
  bool match (Ptr<Packet> p);
  void set (uint16_t port);
};

class Source_Mask : public FilterElements
{
private:
  Ipv4Mask value;

public:
  bool match (Ptr<Packet> p);
  void set (Ipv4Mask mask);
};

class Destination_Ip_Address : public FilterElements
{
private:
  Ipv4Address value;

public:
  bool match (Ptr<Packet> p);
  void set (Ipv4Address ip);
};

class Destination_Port_Number : public FilterElements
{
private:
  uint16_t value;

public:
  Destination_Port_Number (uint16_t port);
  bool match (Ptr<Packet> p);
  void set (uint16_t port);
};

class Destination_Mask : public FilterElements
{
private:
  Ipv4Mask value;

public:
  bool match (Ptr<Packet> p);
  void set (Ipv4Mask mask);
};

class Protocol_Number : public FilterElements
{
private:
  uint32_t value;

public:
  bool match (Ptr<Packet> p);
  void set (u_int32_t protocol);
};
} // namespace ns3

#endif /* FILTER_H */