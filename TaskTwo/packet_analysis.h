#include <iostream>
#include <fstream>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string>
#include "count_min_sketch.h"
using namespace std;
class PCounter 
{
private:
  pcap_t *descr;
  char errbuf[PCAP_ERRBUF_SIZE];
  CountMinSketch cms;

public:
  PCounter();
  void packet_handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet); 
  //Open capture file for offline processing
  int open_offline(char *argv[]);
  //Start packet processing loop
  int process_packets();
  //Iterate over all values in the map
  int estimate_cms();
};