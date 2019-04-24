/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "vector"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-header.h"
#include "ns3/inet-socket-address.h"
#include "filter.h"

namespace ns3 {

bool
Filter::match (Ptr<Packet> p)
{
  for (size_t i = 0; i < elements.size (); i++)
    {
      if (!elements[i].match (p))
        {
          return false;
        }
    }
  return true;
};

bool
Source_Ip_Address::match (Ptr<Packet> p)
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

bool
Source_Port_Number::match (Ptr<Packet> p)
{
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

bool
Source_Mask::match (Ptr<Packet> p)
{
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

bool
Destination_Ip_Address::match (Ptr<Packet> p)
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

bool
Destination_Port_Number::match (Ptr<Packet> p)
{
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

bool
Destination_Mask::match (Ptr<Packet> p)
{
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

bool
Protocol_Number::match (Ptr<Packet> p)
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
} // namespace ns3

// class Source_Ip_Address : public FilterElements
// {
// private:
//   Ipv4Address value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {

//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     Ipv4Address sourceAddress;
//     sourceAddress = iph.GetSource ();

//     if (value.IsEqual (sourceAddress))
//       {
//         return true;
//       }
//     else
//       {
//         return false;
//       }
//   }
// };

// class Source_Port_Number : public FilterElements
// {
// private:
//   uint16_t value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {

//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     Ipv4Address destAddress;
//     destAddress = iph.GetDestination ();

//     InetSocketAddress sock = InetSocketAddress::ConvertFrom (destAddress);
//     int16_t s = sock.GetPort;

//     if (value == s)
//       {
//         return true;
//       }
//     else
//       {
//         return false;
//       }
//   }
// };

// class Source_Mask : public FilterElements
// {
// private:
//   Ipv4Mask value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {

//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     Ipv4Address sourceAddr;
//     sourceAddr = iph.GetSource ();

//     int32_t maskInt;
//     maskInt = value.Get ();

//     Ipv4Address maskAddr;
//     maskAddr = Ipv4Address (maskInt);

//     if (value.IsMatch (maskAddr, sourceAddr))
//     {
//       return true;
//     }
//     else
//     {
//       return false;
//     }
//   }
// };

// class Destination_Ip_Address : public FilterElements
// {
// private:
//   Ipv4Address value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {

//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     Ipv4Address destAddress;
//     destAddress = iph.GetDestination ();

//     if (value.IsEqual (destAddress))
//       {
//         return true;
//       }
//     else
//       {
//         return false;
//       }
//   }
// };

// class Destination_Port_Number : public FilterElements
// {
// private:
//   uint16_t value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {

//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     Ipv4Address destAddress;
//     destAddress = iph.GetDestination ();

//     InetSocketAddress sock = InetSocketAddress::ConvertFrom (destAddress);
//     int16_t s = sock.GetPort;

//     if (value == s)
//       {
//         return true;
//       }
//     else
//       {
//         return false;
//       }
//   }
// };

// class Destination_Mask : public FilterElements
// {
// private:
//   Ipv4Mask value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {
//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     Ipv4Address destAddr;
//     destAddr = iph.GetDestination ();

//     int32_t maskInt;
//     maskInt = value.Get ();

//     Ipv4Address maskAddr;
//     maskAddr = Ipv4Address (maskInt);

//     if (value.IsMatch (maskAddr, destAddr))
//     {
//       return true;
//     }
//     else
//     {
//       return false;
//     }
//   }
// };

// class Protocol_Number : public FilterElements
// {
// private:
//   uint32_t value;

// public:
//   bool
//   match (Ptr<Packet> p)
//   {

//     Ptr<Packet> copy = p->Copy ();
//     Ipv4Header iph;
//     copy->RemoveHeader (iph);

//     uint32_t proto = iph.GetProtocol ();

//     if (value == proto)
//       {
//         return true;
//       }
//     else
//       {
//         return false;
//       }
//   }
// }; // namespace ns3
// } // namespace ns3