
/*
* ���� �۶�
* ѧ�� 919106840420
*/
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <pcap.h>
#include "datastruct.h"
#include "procdata.h"
#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define BUF 256
#define RECVBUF 1500

int main()
{
	// Declare some variables
	WSADATA wsaData;
	int iResult = 0;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		return 1;
	}

	SOCKET sock = INVALID_SOCKET;
	// ��������socket �����������趨Ϊ�ƶ�Э�����ͣ���0x0800��0x0806�����ȵȣ�������Ի�ȡ
	// ������·������ݣ�����������̫��֡���ײ������ݲ��֣���window �������һ�㣬���Ի�ȡ
	// ����ֻ��ȥ��ip�㡣�����ȡ��̫��֡����ʹ��winpcap �����⣬linux����libpcap����ע�⣺
	// Linux�¿���socket�׽��ֻ�ȡ������·�㣩
	if ((sock = socket(AF_INET, SOCK_RAW, /*htons(0x0800)*/IPPROTO_IP)) == INVALID_SOCKET)
	{
		printf("socket error:%d\n", GetLastError());
		WSACleanup();
		return 1;
	}

	// hostent �ṹ�������������ץȡ����������Ӧ�ĵ�ַ
	struct sockaddr_in addr;
	char hostname[BUF] = { 0 };
	int namelen = gethostname(hostname, BUF);
	struct hostent* phost = gethostbyname(hostname);
	if (phost != NULL)
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(0);
		char* paddr;
		int i = 0;
		for (paddr = *phost->h_addr_list; paddr != NULL; ++i, paddr = *(phost->h_addr_list + i))
		{
			memcpy(&(addr.sin_addr), paddr, phost->h_length);
			printf("[%d] %s\n", i, inet_ntoa(addr.sin_addr));
		}
		printf("Please chose a device for Sniffer:");
		int index;
	reenter:
		scanf("%d", &index);
		if (index < 0 || index >= i)
			goto reenter;
		paddr = *(phost->h_addr_list + index);
		memcpy(&(addr.sin_addr), paddr, phost->h_length);
	}
	else
	{
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	cout << "ʹ��ԭʼ�׽��ֱ���޷�̽��MAC���ݶΣ����������ṩ���ַ���\n";
	cout << "����1ʹ��ԭʼ�׽��֣�����ip���ݱ�\n";
	cout << "������������ʹ��winpcap������mac+ip���ݱ�\n";
	cout << "������>>>";
	int slectMethod = 1;
	cin >> slectMethod;
	if (slectMethod != 1) {
		//////////////////
		pcap_t* kp;
		pcap_if_t* alldevs;
		int  i = 0;
		char  errbuf[PCAP_ERRBUF_SIZE];
		char szSource[] = PCAP_SRC_IF_STRING;
		if (pcap_findalldevs_ex(szSource, NULL, &alldevs, errbuf) == 0) {
			while (!(alldevs == NULL)) {
				//cout << " �豸 " << i << " ������ " << (alldevs->name) << endl;
				//cout << " �豸 " << i << " ������ " << (alldevs->description) << endl;
				//if (alldevs->addresses) {
				//	cout << ((struct sockaddr_in*)(alldevs->addresses->netmask))->sin_addr.S_un.S_addr << endl;
				//}
				if (i != 3) {
					i++;
					alldevs = alldevs->next;
					continue;
				}
				char  perrbuf[PCAP_ERRBUF_SIZE];
				pcap_t* p = pcap_open(alldevs->name, 65535, PCAP_OPENFLAG_PROMISCUOUS, 1500, NULL, perrbuf);

				/* ���������·�㣬Ϊ�˼򵥣�����ֻ������̫�� */
				if (pcap_datalink(p) != DLT_EN10MB)
				{
					fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
					/* �ͷ��豸�б� */
					pcap_freealldevs(alldevs);
					return -1;
				}

				int netmask;
				if (alldevs->addresses != NULL)
					/* ��ýӿڵ�һ����ַ������ */
					netmask = ((struct sockaddr_in*)(alldevs->addresses->netmask))->sin_addr.S_un.S_addr;
				else
					/* ����ӿ�û�е�ַ����ô���Ǽ���һ��C������� */
					netmask = 0xffffff;
				char packet_filter[] = "ip";
				struct bpf_program fcode;
				if (pcap_compile(p, &fcode, packet_filter, 1, netmask) < 0)
				{
					fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
					/* �ͷ��豸�б� */
					pcap_freealldevs(alldevs);
					return -1;
				}

				//���ù�����
				if (pcap_setfilter(p, &fcode) < 0)
				{
					fprintf(stderr, "\nError setting the filter.\n");
					/* �ͷ��豸�б� */
					pcap_freealldevs(alldevs);
					return -1;
				}


				if (!(p == NULL))
				{
					kp = p;
					//pcap_next()
					pcap_loop(p, 0, packet_handler, NULL);
				}
				pcap_close(p);
				alldevs = alldevs->next;
				i++;
			}
		}
		pcap_if_t* d;
		pcap_freealldevs(alldevs);
		system("pause");
		return 0;
	}
	//////////////////
	else
	{
		if (bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			closesocket(sock);
			WSACleanup();
			return 1;
		}

		// ��������Ϊ����ģʽ
		unsigned long flag = 1;
		if (ioctlsocket(sock, SIO_RCVALL, &flag) != 0)
		{
			closesocket(sock);
			WSACleanup();
			return 1;
		}

		char recvbuf[RECVBUF];
		size_t size = 0;
		pip_header pip;

		int count = 0;
		while (1)
		{	// ���ܵ�IP���ݰ�
			size = recv(sock, recvbuf, RECVBUF, 0);
			if (size == 0 || size == SOCKET_ERROR)
			{
				if (size == SOCKET_ERROR)
				{
					closesocket(sock);
					WSACleanup();
					return 1;
				}
				continue;
			}
			count++;
			//
			//printf("[%0.4d]\t", count);
			pip = (pip_header)recvbuf;
			switch (pip->proto)
			{
			case IPPROTO_TCP:
				procTcpPack(recvbuf);
				break;
			case IPPROTO_UDP:
				procUdpPack(recvbuf);
				break;
			case IPPROTO_ICMP:
				procIcmpPack(recvbuf);
				break;
			case IPPROTO_IGMP:
				printf("Catch a IGMP, len : %d.\n", ntohs(pip->tlen));
				break;
			case IPPROTO_EGP:
				printf("Catch a EGP, len : %d.\n", ntohs(pip->tlen));
				break;
			case IPPROTO_IGP:
				printf("Catch a IGP, len : %d.\n", ntohs(pip->tlen));
				break;
			case IPPROTO_ESP:
				printf("Catch a ESP, len : %d.\n", ntohs(pip->tlen));
				break;
			case MIB_IPPROTO_OSPF:
				printf("Catch a OSPF, len : %d.\n", ntohs(pip->tlen));
				break;

			default:
				printf("Unknow proto.\n");
				break;
			};
			memset(recvbuf, 0, sizeof(recvbuf));
		}
		system("pause");
		return 0;
	}
}
