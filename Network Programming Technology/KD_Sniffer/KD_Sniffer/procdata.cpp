#include "procdata.h"
// 处理ip首部
struct iph_pack procIpHeader(char* ipdata)
{
	struct iph_pack len;
	memset(&len, 0, sizeof(len));
	if (!ipdata)
		return len;
	struct ip_header* iph = (struct ip_header*)ipdata;
	len.hlen = (iph->ver4_hlen4 & 0xf) * 4;
	//printf("header len : %d ", len.hlen);
	len.tlen = iph->tlen;
	len.souraddr = iph->souraddr;
	len.destaddr = iph->destaddr;
	printf("CheckSum:%-6X  ", ntohs(iph->crc) & 0xFFFF);

	return len;
}
struct iph_pack procIpHeader(const u_char* ipdata)
{
	struct iph_pack len;
	memset(&len, 0, sizeof(len));
	if (!ipdata)
		return len;
	struct ip_header* iph = (struct ip_header*)ipdata;
	len.hlen = (iph->ver4_hlen4 & 0xf) * 4;
	//printf("header len : %d ", len.hlen);
	len.tlen = iph->tlen;
	len.souraddr = iph->souraddr;
	len.destaddr = iph->destaddr;
	printf("CheckSum:%-6X  ", ntohs(iph->crc) & 0xFFFF);
	return len;
}

void procUdpPack(char* ipdata)
{
	struct iph_pack hPack = procIpHeader(ipdata);
	// 随便找一个变量作判断
	if (hPack.hlen == 0)
		return;

	struct udp_header* uh = (struct udp_header*)(ipdata + hPack.hlen);
	struct in_addr addr1, addr2;
	printf("IP_len(%-4d)\t", ntohs(hPack.tlen));
	char buf[16];
	memset(buf, 0, 16);
	switch (ntohs(uh->destport))
	{
		// DNS
	case 53:
		strncpy(buf, "DNS", 3);
		break;

		// TFTP
	case 69:
		strncpy(buf, "TFTP", 4);
		break;

		// SNMP
	case 161:
		strncpy(buf, "SNMP", 4);
		break;

	case 520:
		strncpy(buf, "RIP", 3);
		break;

		// 非常用的熟知端口号
	default:
		strncpy(buf, "UDP", 3);
		break;
	}
	printf("%-6s\t", buf);
	addr1.s_addr = hPack.souraddr;
	addr2.s_addr = hPack.destaddr;
	printf("%-15s:%5d  ->  ",
		inet_ntoa(addr1), ntohs(uh->sourport)
	);
	printf("%-15s:%5d\n", inet_ntoa(addr2), ntohs(uh->destport));
}
void procUdpPack(const u_char* ipdata)
{
	struct iph_pack hPack = procIpHeader(ipdata);
	// 随便找一个变量作判断
	if (hPack.hlen == 0)
		return;

	struct udp_header* uh = (struct udp_header*)(ipdata + hPack.hlen);
	struct in_addr addr1, addr2;
	printf("IP_len(%-4d)\t", ntohs(hPack.tlen));
	char buf[16];
	memset(buf, 0, 16);
	switch (ntohs(uh->destport))
	{
		// DNS
	case 53:
		strncpy(buf, "DNS", 3);
		break;

		// TFTP
	case 69:
		strncpy(buf, "TFTP", 4);
		break;

		// SNMP
	case 161:
		strncpy(buf, "SNMP", 4);
		break;

	case 520:
		strncpy(buf, "RIP", 3);
		break;

		// 非常用的熟知端口号
	default:
		strncpy(buf, "UDP", 3);
		break;
	}
	printf("%-6s\t", buf);
	addr1.s_addr = hPack.souraddr;
	addr2.s_addr = hPack.destaddr;
	printf("%-15s:%5d  ->  ",
		inet_ntoa(addr1), ntohs(uh->sourport)
	);
	printf("%-15s:%5d\n", inet_ntoa(addr2), ntohs(uh->destport));
}

void procTcpPack(char* ipdata)
{
	struct iph_pack hPack = procIpHeader(ipdata);
	// 随便找一个变量作判断
	if (hPack.hlen == 0)
		return;

	struct tcp_header* th = (struct tcp_header*)(ipdata + hPack.hlen);
	struct in_addr addr1, addr2;
	printf("IP_len(%-4d)\t", ntohs(hPack.tlen));
	char buf[16];
	memset(buf, 0, 16);
	switch (ntohs(th->destport))
	{
		// FTP
	case 21:
		strncpy(buf, "FTP", 3);
		break;

		// TELNET
	case 23:
		strncpy(buf, "TELNET", 6);
		break;

		// SMTP
	case 161:
		strncpy(buf, "SMTP", 4);
		break;

		// HTTP
	case 80:
		strncpy(buf, "HTTP", 4);
		break;

		// 非常用的熟知端口号
	default:
		strncpy(buf, "TCP", 3);
		break;
	}
	printf("%-6s\t", buf);
	addr1.s_addr = hPack.souraddr;
	addr2.s_addr = hPack.destaddr;
	printf("%-15s:%5d  ->  ",
		inet_ntoa(addr1), ntohs(th->sourport)
	);
	printf("%-15s:%5d\n", inet_ntoa(addr2), ntohs(th->destport));
}

void procTcpPack(const u_char* ipdata)
{
	struct iph_pack hPack = procIpHeader(ipdata);
	// 随便找一个变量作判断
	if (hPack.hlen == 0)
		return;

	struct tcp_header* th = (struct tcp_header*)(ipdata + hPack.hlen);
	struct in_addr addr1, addr2;
	printf("IP_len(%-4d)\t", ntohs(hPack.tlen));
	char buf[16];
	memset(buf, 0, 16);
	switch (ntohs(th->destport))
	{
		// FTP
	case 21:
		strncpy(buf, "FTP", 3);
		break;

		// TELNET
	case 23:
		strncpy(buf, "TELNET", 6);
		break;

		// SMTP
	case 161:
		strncpy(buf, "SMTP", 4);
		break;

		// HTTP
	case 80:
		strncpy(buf, "HTTP", 4);
		break;

		// 非常用的熟知端口号
	default:
		strncpy(buf, "TCP", 3);
		break;
	}
	printf("%-6s\t", buf);
	addr1.s_addr = hPack.souraddr;
	addr2.s_addr = hPack.destaddr;
	printf("%-15s:%5d  ->  ",
		inet_ntoa(addr1), ntohs(th->sourport)
	);
	printf("%-15s:%5d\n", inet_ntoa(addr2), ntohs(th->destport));
}

void procIcmpPack(char* ipdata)
{
	struct iph_pack hPack = procIpHeader(ipdata);
	struct in_addr addr1, addr2;
	char buf[16] = "ICMP";

	printf("Icmp_len(%-4d)\t%-6s\t%-22s ->  ",
		ntohs(hPack.tlen),
		buf,
		inet_ntoa((addr1.s_addr = hPack.souraddr, addr1))
	);
	printf("%-22s\n", inet_ntoa((addr2.s_addr = hPack.destaddr, addr2)));
}

void procIcmpPack(const u_char* ipdata)
{
	struct iph_pack hPack = procIpHeader(ipdata);
	struct in_addr addr1, addr2;
	char buf[16] = "ICMP";

	printf("Icmp_len(%-4d)\t%-6s\t%-22s ->  ",
		ntohs(hPack.tlen),
		buf,
		inet_ntoa((addr1.s_addr = hPack.souraddr, addr1))
	);
	printf("%-22s\n", inet_ntoa((addr2.s_addr = hPack.destaddr, addr2)));
}

char* iptos(u_long in)
{
	static char output[12][3 * 4 + 3 + 1];
	static short which;
	u_char* p;
	p = (u_char*)&in;
	which = (which + 1 == 12 ? 0 : which + 1);
	sprintf_s(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

/* 回调函数，当收到每一个数据包时会被libpcap所调用 */
void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	struct tm* ltime;
	char timestr[16];
	ip_header* ih;
	udp_header* uh;
	tcp_header* th;

	u_int ip_len;
	u_short sport, dport;
	time_t local_tv_sec;

	/* 将时间戳转换成可识别的格式 */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* 打印数据包的时间戳和长度 */
	//printf("\n%s.%.6d MAC+IP_len:%-4d ", timestr, header->ts.tv_usec, header->len);
	printf("\nMAC+IP_len:%-4d ", header->len);
	/* 获得MAC数据包头部的位置 */
	mac_header* mc = (mac_header*)pkt_data;
	printf("SourceMAC:%X:%X:%X:%X:%X:%X  DesteMAC:%X:%X:%X:%X:%X:%X\n",
		mc->souraddr[0] & 0xFF, mc->souraddr[1] & 0xFF, mc->souraddr[2] & 0xFF, mc->souraddr[3] & 0xFF, mc->souraddr[4] & 0xFF, mc->souraddr[5] & 0xFF,
		mc->destaddr[0] & 0xFF, mc->destaddr[1] & 0xFF, mc->destaddr[2] & 0xFF, mc->destaddr[3] & 0xFF, mc->destaddr[4] & 0xFF, mc->destaddr[5] & 0xFF);
	//cout<<mc->souraddr[0]<<endl;

	/* 获得IP数据包头部的位置 */
	ih = (ip_header*)(pkt_data + 14);
	switch (ih->proto)
	{
	case IPPROTO_TCP:
		procTcpPack(pkt_data + 14);
		break;
	case IPPROTO_UDP:
		procUdpPack(pkt_data + 14);
		break;
	case IPPROTO_ICMP:
		procIcmpPack(pkt_data + 14);
		break;
	case IPPROTO_IGMP:
		printf("Catch a IGMP, len : %d.\n", ntohs(ih->tlen));
		break;
	case IPPROTO_EGP:
		printf("Catch a EGP, len : %d.\n", ntohs(ih->tlen));
		break;
	case IPPROTO_IGP:
		printf("Catch a IGP, len : %d.\n", ntohs(ih->tlen));
		break;
	case IPPROTO_ESP:
		printf("Catch a ESP, len : %d.\n", ntohs(ih->tlen));
		break;
	case MIB_IPPROTO_OSPF:
		printf("Catch a OSPF, len : %d.\n", ntohs(ih->tlen));
		break;

	default:
		printf("Unknow proto.\n");
		break;
	}
}