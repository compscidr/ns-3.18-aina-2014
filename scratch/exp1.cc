/*
 * Jason Ernst, University of Guelph
 * 
 * This experiment uses a multi-hop wireless network (mesh)
 * to show how a strategy which optimizes for a single
 * metric performs poorly in terms of QoS or QoE
 * 
 * In the fully PDR optimized strategy, we ensure each packet is recvd
 * at the next hop before continuing
 * 
 * In the fully delay optimized strategy we never retransmit
 * 
 * In order to establish a Pareto front, we can also obtain a mixture of
 * these results by adopting one strategy with probability p and the
 * other strategy with probability (p-1). This is computed for each
 * packet that is forwarded through a node.
 * 
 * This is the topology
 * 
 * S ----- H ----- H ----- H ----- D
 */

#include <iomanip>		//required to set precision of iosteam

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/bridge-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"

using namespace ns3;
using namespace std;

int main(int argc, char * argv[])
{
	//double P = 0;		//probability of high PDR approach
						//1-P is probability of low delay approach
	
	//size of the simulation area
	double max_x = 200.0;
	double max_y = 200.0;
	double total_time = 10.0;
	
	NodeContainer nodes;
	nodes.Create(3);
	
	MobilityHelper node_mobility;
	Ptr<ListPositionAllocator> node_positionAlloc = CreateObject<ListPositionAllocator> ();
	node_positionAlloc->Add (Vector (0, max_y / 2, 0.0));
	node_positionAlloc->Add (Vector (max_x / 4, max_y / 2, 0.0));
	node_positionAlloc->Add (Vector (max_x / 2, max_y / 2, 0.0));
	node_positionAlloc->Add (Vector ((max_x / 4) * 3, max_y / 2, 0.0));
	node_positionAlloc->Add (Vector (max_x, max_y / 2, 0.0));
	node_mobility.SetPositionAllocator (node_positionAlloc);
	node_mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	node_mobility.Install(nodes);
	
	cout << 0 << "," << max_x / 4 << "," << max_x / 2 << "," << (max_x / 4) * 3 << "," << max_x << endl;
	
	YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
	YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
	phy.SetChannel (wifiChannel.Create ());
	WifiHelper wifi = WifiHelper::Default ();
	wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", StringValue ("OfdmRate54Mbps"));
	wifi.SetStandard (WIFI_PHY_STANDARD_holland);
	wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
	NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
	wifiMac.SetType ("ns3::AdhocWifiMacJason","QosSupported", BooleanValue (false));
	NetDeviceContainer devices = wifi.Install (phy, wifiMac, nodes);
	
	// Enable OLSR
	OlsrHelper olsr;
	Ipv4StaticRoutingHelper staticRouting;

	Ipv4ListRoutingHelper list;
	list.Add (staticRouting, 0);
	list.Add (olsr, 10);
	
	InternetStackHelper stack;
	stack.SetRoutingHelper (list); // has effect on the next Install ()
	stack.Install(nodes);
	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer wifiInterfaces = address.Assign(devices);
	
	//create traffic sink at destination
	ApplicationContainer videoApps;
	uint16_t videoPort = 6970;
	PacketSinkHelper videoSink("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), videoPort));
	videoApps.Add(videoSink.Install(nodes.Get(nodes.GetN()-1)));
	videoApps.Start(Seconds(1.0));
	videoApps.Stop(Seconds(total_time - 0.1));

	//create traffic source
	Ipv4Address destinationAddress = wifiInterfaces.GetAddress(wifiInterfaces.GetN()-1);
	OnOffHelper videoSource ("ns3::UdpSocketFactory", InetSocketAddress (destinationAddress, videoPort));
	videoSource.SetAttribute ("PacketSize", UintegerValue (1024));
	videoSource.SetConstantRate (DataRate ("5.0Mbps"));
	videoSource.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	videoSource.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
	videoApps.Add(videoSource.Install(nodes.Get(0)));
	videoApps.Start(Seconds(1.0));
	videoApps.Stop(Seconds(total_time - 0.1));

	//install flow monitor to keep stats on traffic
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
	
	Simulator::Stop (Seconds (total_time));
	Simulator::Run ();
	
	monitor->CheckForLostPackets ();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
	
	//dumps to xml file for verification
	//monitor->SerializeToXmlFile("myglobalroutin.flowmonitor", true, true);
	
	double pdr = 0.0;
	double total_e2e_delay = 0;
	
	std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.end();
	i--;
	pdr = (double)i->second.rxPackets / (double)i->second.txPackets;
	
	if(i->second.rxPackets == 0)
		total_e2e_delay = 0;
	else
		total_e2e_delay += (i->second.delaySum).GetMicroSeconds() / i->second.rxPackets;
		
	cout << "PDR: " << pdr << " delay: " << total_e2e_delay << endl;
	
	Simulator::Destroy ();
	
	return 0;
}
