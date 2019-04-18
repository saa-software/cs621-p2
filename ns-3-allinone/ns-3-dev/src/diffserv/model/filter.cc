/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "vector"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-header.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

class Filter
{
public:
  std::vector<FilterElements> elements;

  bool
  Filter::match (Ptr<Packet> p)
  {
    for (size_t i = 0; i < elements.size; i++)
      {
        if (!elements[i].match (p))
          {
            return false;
          }
      }
    return true;
  };
};

class FilterElements
{
public:
  bool virtual FilterElements::match (Ptr<Packet> p);
};

class Source_Ip_Address : public FilterElements
{
private:
  Ipv4Address value;

public:
  bool
  match (Ptr<Packet> p)
  {

    Ptr<Packet> copy = p->Copy ();
    Ipv4Header iph;
    copy->RemoveHeader (iph);

    Ipv4Address sourceAddress;
    sourceAddress = iph.GetSource ();

    if (value.IsEqual (sourceAddress))
      {
        return true;
      }
    else
      {
        return false;
      }
  }
};

class Source_Port_Number : public FilterElements
{
private:
  uint16_t value;

public:
  bool
  match (Ptr<Packet> p)
  {

    Ptr<Packet> copy = p->Copy ();
    Ipv4Header iph;
    copy->RemoveHeader (iph);

    Ipv4Address destAddress;
    destAddress = iph.GetDestination ();

    InetSocketAddress sock = InetSocketAddress::ConvertFrom (destAddress);
    int16_t s = sock.GetPort;

    if (value == s)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
};

class Source_Mask : public FilterElements
{
private:
  Ipv4Mask value;

public:
  bool
  match (Ptr<Packet> p)
  {

    // int32_t v = value.Get ();

    //   Ptr<Ipv4Header> h = Create<Ipv4Header> ();
    //   h->GetAttribute ("Ipv4Mask", *value)

    // Ptr<Packet> pkt = Create<Packet> ();
    // // pkt->

    // Ptr<Packet> copy = p->Copy ();
    // Ipv4Header iph;
    // copy->RemoveHeader (iph);

    // AttributeValue ipm;
    // Ipv4Address sourceAddress;
    // sourceAddress = iph.GetSource ();
    // iph.GetAttribute ("Ipv4Mask", ipm);

    // ipm.IsMatch (sourceAddress, value)

    // if (value == mask)
    //   {
    //     return true;
    //   }
    // else
    //   {
    //     return false;
    //   }
    return true;
  }
};

class Destination_Ip_Address : public FilterElements
{
private:
  Ipv4Address value;

public:
  bool
  match (Ptr<Packet> p)
  {

    Ptr<Packet> copy = p->Copy ();
    Ipv4Header iph;
    copy->RemoveHeader (iph);

    Ipv4Address destAddress;
    destAddress = iph.GetDestination ();

    if (value.IsEqual (destAddress))
      {
        return true;
      }
    else
      {
        return false;
      }
  }
};

class Destination_Port_Number : public FilterElements
{
private:
  uint16_t value;

public:
  bool
  match (Ptr<Packet> p)
  {

    Ptr<Packet> copy = p->Copy ();
    Ipv4Header iph;
    copy->RemoveHeader (iph);

    Ipv4Address destAddress;
    destAddress = iph.GetDestination ();

    InetSocketAddress sock = InetSocketAddress::ConvertFrom (destAddress);
    int16_t s = sock.GetPort;

    if (value == s)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
};

class Destination_Mask : public FilterElements
{
private:
  Ipv4Mask value;

public:
  bool
  match (Ptr<Packet> p)
  {
    return true;
  }
};

class Protocol_Number : public FilterElements
{
private:
  uint32_t value;

public:
  bool
  match (Ptr<Packet> p)
  {

    Ptr<Packet> copy = p->Copy ();
    Ipv4Header iph;
    copy->RemoveHeader (iph);

    uint32_t proto = iph.GetProtocol ();

    if (value == proto)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
};
} // namespace ns3