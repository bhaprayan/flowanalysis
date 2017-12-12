//Optimize the addition of this library (taking up too much space!)
//Using an unordered map because we have no requirment to sort the underlying array (yet!), hence much faster.
#include<bits/stdc++.h>
#include<pcap.h>

using namespace std;

int main()
{
	/*
	unordered_map<string, double> umap;

	umap["PI"] = 3.14;
	umap["g"] = 9.8;

	string key = "PI";
	
	if(umap.find("PI") == umap.end())
	{
		cout << "Key not found" << endl;
	}
	else
	{
		cout << "Key found!" << endl;	
	}
	//Iterate over all values in the map
	unordered_map<string, double>::iterator itr;

	cout << "Displaying all the elements:" << endl;

	for(itr=umap.begin(); itr != umap.end(); itr++)
	{
		cout << itr->first << " " << itr->second << endl;
	}
	*/

	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];

	//Open the capture file for offline processing
	handle = pcap_open_offline("http.cap", errbuf);
	if(handle == NULL) {
		cout << "PCAP_OPEN_OFFLINE() Failed: " << errbuf << endl;
		return 1;
	}

	if(pcap_loop(handle,0,packetHandler, NULL) < 0) {
		cout << "PCAP_LOOP() failed: " << pcap_geterr(handle);
		return 1;
	}
	
	cout << "Capture finished!" << endl;

	return 0;
}

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
	const struct ether_header* ethernetHeader;
	const struct ip* ipHeader;
	const struct tcphdr* tcpHeader;
	char sourceIp[INET_ADDRSTRLEN];
	char destIp[INET_ADDRSTRLEN];
	u_int sourcePort, destPort;

	if (ipHeader->ip_p == IPPROTO_TCP) {
		tcpHeader = (tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
		sourcePort = ntohs(tcpHeader->source);
		destPort = ntohs(tcpHeader->dest);
		data = (u_char*)(packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
		dataLength = pkthdr->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));


