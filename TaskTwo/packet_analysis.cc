#include "packet_analysis.h"

//Use parallel implementation?
using namespace std;

PCounter* pcounter = nullptr;

void global_packet_handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
  pcounter->packet_handler(userData, pkthdr, packet);
}

PCounter::PCounter()
{
  pcounter = this;
  CountMinSketch cms();
}

void PCounter::packet_handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) 
{
  const struct ether_header* ethernetHeader;
  const struct ip* ipHeader;
  const struct tcphdr* tcpHeader;
  char sourceIp[INET_ADDRSTRLEN];
  char destIp[INET_ADDRSTRLEN];
  u_int sourcePort, destPort;
  string SRC_IP, DEST_IP;
  string SRC_PORT, DEST_PORT;
  string KEY;
  ethernetHeader = (struct ether_header*)packet;
  if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) 
  {
    ipHeader = (struct ip*)(packet + sizeof(struct ether_header));
    inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

    if (ipHeader->ip_p == IPPROTO_TCP || ipHeader->ip_p == IPPROTO_UDP) 
    {
      tcpHeader = (tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
      sourcePort = ntohs(tcpHeader->source);
      destPort = ntohs(tcpHeader->dest);
      // print the results
      SRC_IP = string(sourceIp);
      DEST_IP = string(destIp);
      SRC_PORT = to_string(sourcePort);
      DEST_PORT = to_string(destPort);
      KEY = "("+SRC_IP+","+SRC_PORT+","+DEST_IP+","+DEST_PORT+")";
      //cout << KEY << endl;
      if(string(reinterpret_cast<char*>(userData)) == "UPDATE")
      {
        cms.update(KEY,1);
      }
    }
  }
}
//Open capture file for offline processing
int PCounter::open_offline(char* argv[])
{
  descr = pcap_open_offline(argv[1], errbuf);
  if (descr == NULL) 
  {
  cout << "pcap_open_live() failed: " << errbuf << endl;
  return 1;
  }
}

//Start packet processing loop
int PCounter::process_packets()
{
  u_char function[7] = "UPDATE";
  if(pcap_loop(descr, 0, global_packet_handler, function)< 0)
  {
  cout << "pcap_loop() failed: " << pcap_geterr(descr);
  return 1;
  }
  cout << "capture finished" << endl;
}

//Iterate over all values in the map
int PCounter::estimate_cms()
{
  string KEY;
  ifstream FLOW_FILE;
  FLOW_FILE.open("FLOW_FILE.txt");
  if(FLOW_FILE.is_open())
  {
    while(getline(FLOW_FILE, KEY))
    {
      cout << "Flow: " << KEY << " " << "Estimate: " << cms.estimate(KEY) << endl;
    }
  }
}

int main(int argc, char* argv[]) 
{
  PCounter test = PCounter();

  test.open_offline(argv);
  test.process_packets();
  test.estimate_cms();
  return 0;
}

