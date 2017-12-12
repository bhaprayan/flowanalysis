#include <iostream>
#include <fstream>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unordered_map>

using namespace std;

class PCounter 
{
private:
  unordered_map<string, int> umap;
  pcap_t *descr;
  char errbuf[PCAP_ERRBUF_SIZE];

public:
  PCounter();
  void packet_handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet); 
  //Open capture file for offline processing
  int open_offline(char* argv[]);
  //Start packet processing loop
  int process_packets();
  //Iterate over all values in the map
  void view_map(char* argv[]);
};
