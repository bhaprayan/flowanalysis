#include "packet_analysis.h"

using namespace std;

PCounter* pcounter = nullptr;

void global_packet_handler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
	pcounter->packet_handler(userData, pkthdr, packet);
}

PCounter::PCounter()
{
	pcounter = this;
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

		if (ipHeader->ip_p == IPPROTO_TCP || ipHeader-> ip_p == IPPROTO_UDP) 
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
			if(PCounter::umap.find(KEY) == PCounter::umap.end())
			{
				umap.insert({KEY,1});
			}
			else
			{
				umap[KEY]++;
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
	if(pcap_loop(descr, 0, global_packet_handler, NULL)< 0)
	{
		cout << "pcap_loop() failed: " << pcap_geterr(descr);
		return 1;
	}
	cout << "capture finished" << endl;
}

//Iterate over all values in the map
//Also write all unique flows (i.e. keys) to a text file
//This well help to automate the estimation stage of the CMS (instead of manual entry)
void PCounter::view_map(char* argv[])
{
	ofstream FLOW_FILE;
	ofstream HIST_FILE;
	FLOW_FILE.open("../TaskTwo/FLOW_FILE.txt");
	HIST_FILE.open("HIST_FILE.txt");
	unordered_map<string, int>::iterator itr;
	cout << "Displaying all the elements:" << endl;
	for(itr=umap.begin(); itr != umap.end(); itr++)
	{
		cout << "Flow: " << itr->first << " " << "Count: " << itr->second << endl;
		FLOW_FILE << itr->first << endl;
		HIST_FILE << "Flow: " << itr->first << " " << "Count: " << itr->second << endl;
	}
	FLOW_FILE.close();
	HIST_FILE.close();
}

int main(int argc, char** argv) 
{
	PCounter test = PCounter();
	test.open_offline(argv);
	test.process_packets();
	test.view_map(argv);
	return 0;
}

