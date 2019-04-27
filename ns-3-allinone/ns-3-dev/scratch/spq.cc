/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

// - Tracing of queues and packet receptions to file "cda.tr"

#include <fstream>
#include <string>
#include <iostream>
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "src/json/json.h"
#include "ns3/spq.h"
#include "ns3/vector.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SPQ");

int
main (int argc, char *argv[])
{
  // Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("6000p"));
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
  LogComponentEnable ("SPQ", LOG_LEVEL_INFO);
  LogComponentEnable ("CdaServerApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("SPQ", LOG_LEVEL_ALL);
  LogComponentEnable ("TrafficClass", LOG_LEVEL_ALL);
  LogComponentEnable ("DropTailQueue", LOG_LEVEL_ALL);
  LogComponentEnable ("Filter", LOG_LEVEL_ALL);

  CommandLine cmd;

  std::string inputFile = "";

  cmd.AddValue ("i", "inputFile for SPQ Application", inputFile);
  cmd.Parse (argc, argv);

  std::cout << "inputFile: " << inputFile << std::endl;

  // Read the json config file
  Json::Value root;
  Json::CharReaderBuilder rbuilder;
  std::string errs;
  std::ifstream config_doc(inputFile, std::ifstream::binary);
  config_doc >> root;
  Json::parseFromStream (rbuilder, config_doc, &root, &errs);
  Json::StreamWriterBuilder wbuilder;
  // Get number of queues
  const Json::Value outputNumQueues = root["numerOfQueues"];
  std::string numQueuesStr = Json::writeString (wbuilder, outputNumQueues);
  int numQueues = stoi (numQueuesStr);
  Ptr<SPQConfig> config = CreateObject<SPQConfig> ();
  config->SetNumberOfQueues (numQueues);
  // SPQConfig spqConfig (numQueues);
  // Get level for each queue
  std:: vector<int> queueLevels;
  int i;
  for (i = 0; i < numQueues; i++) {
      std::string q = "q" + to_string(i);
      const Json::Value outputQ0 = root[q];
      std::string q0Str = Json::writeString (wbuilder, outputQ0);
      int qVal = stoi (q0Str);
      queueLevels.push_back(qVal);
  }
  // End json parsing

  std::cout << "queuelevels " << queueLevels[1] << std::endl;
  for (i = 0; i < numQueues; i++) {
    printf("%d ", queueLevels[i]);
  }
  printf("\n");

  // Explicitly create the nodes required by the topology.
  NodeContainer n;
  n.Create (3);

  NodeContainer n0n1 = NodeContainer (n.Get (0), n.Get (1));
  NodeContainer n1n2 = NodeContainer (n.Get (1), n.Get (2));;

  PointToPointHelper p0p1;
  p0p1.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));

  NetDeviceContainer c0c1 = p0p1.Install (n0n1);

  PointToPointHelper p1p2;
  p1p2.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
  // p1p2.SetQueue("ns3::SPQ",
                // "Mode", StringValue ("QUEUE_MODE_PACKETS"));
  p1p2.SetQueue("ns3::SPQ",
                "Mode", StringValue ("QUEUE_MODE_PACKETS"),
                "SPQConfig", PointerValue (config));
  // p1p2.SetQueue("ns3::SPQ",
  //               "Mode", StringValue ("QUEUE_MODE_PACKETS"),
  //               "NumberOfQueues", IntegerValue (numQueues),
  //               "QueueLevels", VectorValue (&queueLevels));

  NetDeviceContainer c1c2 = p1p2.Install (n1n2);

  InternetStackHelper stack;
  stack.Install (n);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer i0i1 = address.Assign (c0c1);
  Ipv4InterfaceContainer i1i2 = address.Assign (c1c2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //
  // Create a CdaServer application on node 2.
  //

  Packet::EnablePrinting ();
  double start = 1.0;
  double stop = 300.0;
  uint16_t port1 = 9;
  uint16_t port2 = 10;

  CdaServerHelper server1 (port1);
  CdaServerHelper server2 (port2);

  ApplicationContainer apps = server1.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server2.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  //
  // Create a SPQ application to send UDP datagrams from node zero to
  // node two.
  //
  uint32_t packetSize1 = 1000;
  uint32_t maxPacketCount1 = 2;
  Time interPacketInterval1 = MicroSeconds (0);

  uint32_t packetSize2 = 1000;
  uint32_t maxPacketCount2 = 2;
  Time interPacketInterval2 = MicroSeconds (0);

  CdaClientHelper client1 (i1i2.GetAddress (1), port1);
  client1.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount1));
  client1.SetAttribute ("Interval", TimeValue (interPacketInterval1));
  client1.SetAttribute ("PacketSize", UintegerValue (packetSize1));

  apps = client1.Install (n.Get (0));
  apps.Start (Seconds (start + 1));
  apps.Stop (Seconds (stop));


  CdaClientHelper client2 (i1i2.GetAddress (1), port2);
  client2.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount2));
  client2.SetAttribute ("Interval", TimeValue (interPacketInterval2));
  client2.SetAttribute ("PacketSize", UintegerValue (packetSize2));

  apps = client2.Install (n.Get (0));
  apps.Start (Seconds (start + 2));
  apps.Stop (Seconds (stop));


  AsciiTraceHelper ascii;

  std::string fileName = "spq-";
  p0p1.EnablePcapAll (fileName, false);
  p1p2.EnablePcapAll (fileName, false);

  //
  // Now, do the actual simulation.
  //
  Simulator::Run ();
  Simulator::Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}
