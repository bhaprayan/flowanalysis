#include <iostream>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unordered_map>

using namespace std;

static unordered_map<string, int> umap;

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) 
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

          if (ipHeader->ip_p == IPPROTO_TCP) 
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
              cout << KEY << endl;
              if(umap.find(KEY) == umap.end())
              {
                umap.insert({KEY,0});
              }
              else
              {
                umap[KEY]++;
              }
          }
      }
}

int main()
{
  pcap_t *descr;
  char errbuf[PCAP_ERRBUF_SIZE];

  descr = pcap_open_offline("http.cap", errbuf);
  if (descr == NULL) 
  {
    cout << "pcap_open_live() failed: " << errbuf << endl;
    return 1;
  }
  
  descr = pcap_open_offline("http.cap", errbuf);
  if (descr == NULL) 
  {
    cout << "pcap_open_live() failed: " << errbuf << endl;
    return 1;
  }
  if(pcap_loop(descr, 0, packetHandler, NULL)< 0)
  {
    cout << "pcap_loop() failed: " << pcap_geterr(descr);
    return 1;
  }
  cout << "capture finished" << endl;
  unordered_map<string, int>::iterator itr;
  cout << "Displaying all the elements:" << endl;
  for(itr=umap.begin(); itr != umap.end(); itr++)
  {
    cout << itr->first << " " << itr->second << endl;
  }
}