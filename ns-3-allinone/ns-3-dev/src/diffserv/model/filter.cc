/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "vector"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/inet-socket-address.h"
#include "filter.h"

namespace ns3 {
  
  NS_LOG_COMPONENT_DEFINE ("Filter");

void
Filter::set(std::vector<FilterElements*> e) {
  NS_LOG_FUNCTION (this);
  elements = e;
} 

bool
Filter::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  for (size_t i = 0; i < elements.size (); i++)
    {
      if (!elements[i]->match (p))
        {
          return false;
        }
    }
  return true;
};

bool
Source_Ip_Address::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
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

void
Source_Ip_Address::set (Ipv4Address ip)
{
  NS_LOG_FUNCTION (this);
  value = ip;
}

bool
Source_Port_Number::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> copy = p->Copy ();
  Ipv4Header iph;
  copy->RemoveHeader (iph);

  Ipv4Address destAddress;
  destAddress = iph.GetDestination ();

  InetSocketAddress sock = InetSocketAddress::ConvertFrom (destAddress);
  int16_t s = sock.GetPort ();

  if (value == s)
    {
      return true;
    }
  else
    {
      return false;
    }
}

void
Source_Port_Number::set (u_int16_t port)
{
  NS_LOG_FUNCTION (this);
  value = port;
}

bool
Source_Mask::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> copy = p->Copy ();
  Ipv4Header iph;
  copy->RemoveHeader (iph);

  Ipv4Address sourceAddr;
  sourceAddr = iph.GetSource ();

  int32_t maskInt;
  maskInt = value.Get ();

  Ipv4Address maskAddr;
  maskAddr = Ipv4Address (maskInt);

  if (value.IsMatch (maskAddr, sourceAddr))
    {
      return true;
    }
  else
    {
      return false;
    }
}

void
Source_Mask::set (Ipv4Mask mask)
{
  NS_LOG_FUNCTION (this);
  value = mask;
}

bool
Destination_Ip_Address::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
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

void
Destination_Ip_Address::set (Ipv4Address ip)
{
  NS_LOG_FUNCTION (this);
  value = ip;
}

bool
Destination_Port_Number::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> copy = p->Copy ();
  UdpHeader uh;
  copy->RemoveHeader (uh);
  NS_LOG_FUNCTION (uh);

  uint16_t port = uh.GetDestinationPort();

  NS_LOG_FUNCTION (port);
  if (value == port)
    {
      return true;
    }
  else
    {
      return false;
    }
}

Destination_Port_Number::Destination_Port_Number (u_int16_t port)
{
  NS_LOG_FUNCTION (this);
  value = port;
}

void
Destination_Port_Number::set (u_int16_t port)
{
  NS_LOG_FUNCTION (this);
  value = port;
}

bool
Destination_Mask::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> copy = p->Copy ();
  Ipv4Header iph;
  copy->RemoveHeader (iph);

  Ipv4Address destAddr;
  destAddr = iph.GetDestination ();

  int32_t maskInt;
  maskInt = value.Get ();

  Ipv4Address maskAddr;
  maskAddr = Ipv4Address (maskInt);

  if (value.IsMatch (maskAddr, destAddr))
    {
      return true;
    }
  else
    {
      return false;
    }
}

void
Destination_Mask::set (Ipv4Mask mask)
{
  NS_LOG_FUNCTION (this);
  value = mask;
}

bool
Protocol_Number::match (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
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

void
Protocol_Number::set (uint32_t protocol)
{
  NS_LOG_FUNCTION (this);
  value = protocol;
}
} // namespace ns3