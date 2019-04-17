/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "vector"
#include "ns3/packet.h"
#include "ns3/ptr.h"

namespace ns3 {

    class FilterElements {
        public:
            bool
            FilterElements::match (Ptr<Packet> p) {
                
            };
    };

    class Filter {
        public:
            std::vector<FilterElements> elements;

            bool
            Filter::match (Ptr<Packet> p) {

            };
    };
}