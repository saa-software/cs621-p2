/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <iostream>
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include "cda-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CdaServerApplication");

NS_OBJECT_ENSURE_REGISTERED (CdaServer);

TypeId
CdaServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CdaServer")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<CdaServer> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&CdaServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&CdaServer::m_rxTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("RxWithAddresses", "A packet has been received",
                     MakeTraceSourceAccessor (&CdaServer::m_rxTraceWithAddresses),
                     "ns3::Packet::TwoAddressTracedCallback")
  ;
  return tid;
}

CdaServer::CdaServer ()
{
  m_t1Start = 0.0;
  m_t1End = 0.0;
  m_t2Start = 0.0;
  m_t2End = 0.0;
  m_lastSentPacket = 0.0;
  m_nPackets = 0;
}

CdaServer::~CdaServer()
{
  m_socket = 0;
  m_socket6 = 0;
  m_t1Start = 0.0;
  m_t1End = 0.0;
  m_t2Start = 0.0;
  m_t2End = 0.0;
  m_lastSentPacket = 0.0;
}

void
CdaServer::DoDispose (void)
{
  Application::DoDispose ();
}

void 
CdaServer::StartApplication (void)
{
  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      if (m_socket->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local6 = Inet6SocketAddress (Ipv6Address::GetAny (), m_port);
      if (m_socket6->Bind (local6) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
      if (addressUtils::IsMulticast (local6))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket6);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, local6);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&CdaServer::HandleRead, this));
  m_socket6->SetRecvCallback (MakeCallback (&CdaServer::HandleRead, this));
}

void 
CdaServer::StopApplication ()
{
  if (m_socket != 0) 
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  if (m_socket6 != 0) 
    {
      m_socket6->Close ();
      m_socket6->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }

    m_t2End = m_lastSentPacket;
    double t1Time = fabs (m_t1Start - m_t1End);
    double t2Time = fabs (m_t2Start - m_t2End);
    double delta = fabs (t1Time - t2Time);

    std::cout << "t1start = " << m_t1Start << std::endl;
    std::cout << "t1End = " << m_t1End << std::endl;
    std::cout << "t2Start = " << m_t2Start << std::endl;
    std::cout << "t2End = " << m_t2End << std::endl;

    std::cout << "High Entropy Train Time = " << t1Time << std::endl;
    std::cout << "Low Entropy Train Time = " << t2Time << std::endl;
    
    if (delta > 100)
    {
      std::cout << "Compression detected!\ndelta = " << delta << "ms" << std::endl;
    } else {
      std::cout << "No compression was detected\ndelta = " << delta << "ms" << std::endl;
    }
}

void 
CdaServer::HandleRead (Ptr<Socket> socket)
{

  Ptr<Packet> packet;
  Address from;
  Address localAddress;
  while ((packet = socket->RecvFrom (from)))
    {
      socket->GetSockName (localAddress);
      m_rxTrace (packet);
      m_rxTraceWithAddresses (packet, from, localAddress);

      double recTime = Simulator::Now ().GetMilliSeconds ();
      if (m_nPackets == 0)
        {
          m_t1Start = recTime;
          m_lastSentPacket = recTime;
        }
      if (fabs(recTime - m_lastSentPacket) > 1000.0)
      {
        m_t1End = m_lastSentPacket;
        m_t2Start = recTime;
      }
      m_lastSentPacket = recTime;
      m_nPackets++;
    }
}

} // Namespace ns3
