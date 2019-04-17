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
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
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
  Json::Value root;
  Json::CharReaderBuilder rbuilder;
  // Configure the Builder, then ...
  std::string errs;
  std::ifstream config_doc("scratch/config.json", std::ifstream::binary);
  config_doc >> root;
  // printf("1\n");
  // bool parsingSuccessful = Json::parseFromStream (rbuilder, config_doc, &root, &errs);
  Json::parseFromStream (rbuilder, config_doc, &root, &errs);
  // printf("success: %d\n", parsingSuccessful);
  // if (!parsingSuccessful)
    // {
      // report to the user the failure and their locations in the document.
      // std::cout << "Failed to parse configuration\n" << errs;
      // return 0;
    // }
  // printf("2\n");
  // ...
  Json::StreamWriterBuilder wbuilder;
  // Configure the Builder, then ...
  // std::string outputConfig = Json::writeString (wbuilder, root);
  // std::cout << "JSON: " << outputConfig << std::endl;
  const Json::Value outputCompressionProtocol = root["compression_protocol"];
  std::string compressionProtocol = Json::writeString (wbuilder, outputCompressionProtocol);
  std::cout << "COMPRESSION PROTO: " << compressionProtocol << std::endl;
  int proto = stoi (compressionProtocol);
  // End json parsing
  
  Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("6000p"));
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
  LogComponentEnable ("Cda", LOG_LEVEL_INFO);
  LogComponentEnable ("CdaServerApplication", LOG_LEVEL_ALL);

  CommandLine cmd;

  uint32_t capacity = 1;
  bool compressionEnabled = 1;

  cmd.AddValue ("capacity", "Capacity of compression link in Mbps", capacity);
  cmd.AddValue ("compressionEnabled", "Enable or disable compression link", compressionEnabled);
  cmd.Parse (argc, argv);

  std::cout << "Capacity of Compression link: " << capacity << std::endl;
  std::cout << "Compression Enabled: " << compressionEnabled << std::endl;

  // Explicitly create the nodes required by the topology (shown above).

  NodeContainer n;
  n.Create (4);

  //
  NodeContainer n0n1 = NodeContainer (n.Get (0), n.Get (1));
  NodeContainer n1n2 = NodeContainer (n.Get (1), n.Get (2));
  NodeContainer n2n3 = NodeContainer (n.Get (2), n.Get (3));

  PointToPointHelper p2p1;
  p2p1.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
  p2p1.SetDeviceAttribute ("CompressionEnabled", BooleanValue (0));

  PointToPointHelper p2p2;
  p2p2.SetDeviceAttribute ("DataRate", StringValue (std::to_string (capacity) + "Mbps"));
  p2p2.SetDeviceAttribute ("CompressionEnabled", BooleanValue (compressionEnabled));
  p2p2.SetDeviceAttribute ("CompressionProtocol", IntegerValue (proto));

  PointToPointHelper p2p3;
  p2p3.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
  p2p3.SetDeviceAttribute ("CompressionEnabled", BooleanValue (0));

  NetDeviceContainer c0c1 = p2p1.Install (n0n1);
  NetDeviceContainer c1c2 = p2p2.Install (n1n2);
  NetDeviceContainer c2c3 = p2p3.Install (n2n3);

  InternetStackHelper stack;
  stack.Install (n);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer i0i1 = address.Assign (c0c1);
  Ipv4InterfaceContainer i1i2 = address.Assign (c1c2);
  Ipv4InterfaceContainer i2i3 = address.Assign (c2c3);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //
  // Create a CdaServer application on node one.
  //

  Packet::EnablePrinting ();
  double start = 1.0;
  double stop = 3000.0;
  uint16_t port = 9; // well-known echo port number

  CdaServerHelper server (port);
  ApplicationContainer apps = server.Install (n.Get (3));
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  //
  // Create a CdaClient application to send UDP datagrams from node zero to
  // node three.
  //
  uint32_t packetSize = 1100;
  uint32_t maxPacketCount = 12000;

  Time interPacketInterval = MicroSeconds (0);
  CdaClientHelper client (i2i3.GetAddress (1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps = client.Install (n.Get (0));
  apps.Start (Seconds (start + 1));
  apps.Stop (Seconds (stop));

  AsciiTraceHelper ascii;
  // p2p.EnableAsciiAll (ascii.CreateFileStream ("cda.tr"));

  p2p1.EnablePcap ("l1-cda", n0n1, false);
  p2p2.EnablePcap ("l1-cda", n1n2, false);
  p2p3.EnablePcap ("l1-cda", n2n3, false);

  if (compressionEnabled)
    {
      std::string fileName = "cda-" + std::to_string (capacity) + "-compression-";
      p2p1.EnablePcapAll (fileName, false);
      p2p2.EnablePcapAll (fileName, false);
      p2p3.EnablePcapAll (fileName, false);
    }
  else
    {
      std::string fileName = "cda-" + std::to_string (capacity) + "-noCompression-";
      p2p1.EnablePcapAll (fileName, false);
      p2p2.EnablePcapAll (fileName, false);
      p2p1.EnablePcapAll (fileName, false);
    }

  //
  // Now, do the actual simulation.
  //
  Simulator::Run ();
  Simulator::Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}
