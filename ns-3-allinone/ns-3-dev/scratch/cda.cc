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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Cda");

int
main (int argc, char *argv[])
{
  // Read the json config file to get the compression protocol
  // Json::Value root;
  // Json::CharReaderBuilder rbuilder;
  // std::ifstream file ("config.json");
  // file >> root;
  // const Json::Value CompressionProtocol = root["compression_protocol"];
  // Json::Value root;
  // Json::CharReaderBuilder rbuilder;
  // Configure the Builder, then ...
  // std::string errs;
  // std::ifstream config_doc("scratch/config.json", std::ifstream::binary);
  // config_doc >> root;
  // printf("1\n");
  // bool parsingSuccessful = Json::parseFromStream (rbuilder, config_doc, &root, &errs);
  // Json::parseFromStream (rbuilder, config_doc, &root, &errs);
  // printf("success: %d\n", parsingSuccessful);
  // if (!parsingSuccessful)
    // {
      // report to the user the failure and their locations in the document.
      // std::cout << "Failed to parse configuration\n" << errs;
      // return 0;
    // }
  // printf("2\n");
  // ...
  // Json::StreamWriterBuilder wbuilder;
  // Configure the Builder, then ...
  // std::string outputConfig = Json::writeString (wbuilder, root);
  // std::cout << "JSON: " << outputConfig << std::endl;
  // const Json::Value outputCompressionProtocol = root["compression_protocol"];
  // std::string compressionProtocol = Json::writeString (wbuilder, outputCompressionProtocol);
  // std::cout << "COMPRESSION PROTO: " << compressionProtocol << std::endl;
  // int proto = stoi (compressionProtocol);
  // End json parsing
  
  // Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("6000p"));
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
  LogComponentEnable ("Cda", LOG_LEVEL_INFO);
  LogComponentEnable ("CdaServerApplication", LOG_LEVEL_ALL);

  CommandLine cmd;

  std::string inputFile = "";

  cmd.AddValue ("inputFile", "inputFile for SPQ Application", inputFile);
  cmd.Parse (argc, argv);

  std::cout << "inputFile: " << inputFile << std::endl;

  // Explicitly create the nodes required by the topology.

  NodeContainer n;
  n.Create (3);

  NodeContainer n0n1 = NodeContainer (n.Get (0), n.Get (1));
  NodeContainer n1n2 = NodeContainer (n.Get (1), n.Get (2));;

  PointToPointHelper p0p1;
  p0p1.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));


  PointToPointHelper p1p2;
  p1p2.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
  p1p2.SetQueue("ns3::SPQ", "MaxPackets", StringValue ("10"));

  NetDeviceContainer c0c1 = p0p1.Install (n0n1);
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
  double stop = 3000.0;
  uint16_t port1 = 9; // well-known echo port number
  uint16_t port2 = 10; // well-known echo port number

  CdaServerHelper server1 (port1);
  CdaServerHelper server2 (port2);

  ApplicationContainer apps = server1.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  apps = server2.Install (n.Get (2));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  //
  // Create a CdaClient application to send UDP datagrams from node zero to
  // node two.
  //
  uint32_t packetSize = 1100;
  uint32_t maxPacketCount = 12000;
  Time interPacketInterval = MicroSeconds (0);

  CdaClientHelper client1 (i1i2.GetAddress (1), port1);
  client1.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client1.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client1.SetAttribute ("PacketSize", UintegerValue (packetSize));

  apps = client1.Install (n.Get (0));
  apps.Start (Seconds (start + 1));
  apps.Stop (Seconds (stop));


  CdaClientHelper client2 (i1i2.GetAddress (1), port2);
  client2.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client2.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client2.SetAttribute ("PacketSize", UintegerValue (packetSize));

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
