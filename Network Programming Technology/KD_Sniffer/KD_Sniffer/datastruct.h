#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

typedef struct mac_header
{
	char destaddr[6];
	char souraddr[6];
	unsigned short frametype;
}mac_header, * pmac_header;

#pragma pack(push, 1)
typedef struct ip_header
{
	unsigned char ver4_hlen4;		// 版本 和 首部长度
	unsigned char diffserv;		// 区分服务
	unsigned short tlen;		// 总长度
	unsigned short flag;		// 标识
	unsigned short flag4_fraoff12; // 标志 和 片偏移
	unsigned char ttl;			// 生存时间
	unsigned char proto;					// 协议
	unsigned short crc;			// 首部检验和
	unsigned long souraddr;		// 源地址
	unsigned long destaddr;		// 目的地址
}ip_header, * pip_header;

typedef struct udp_header
{
	unsigned short sourport;
	unsigned short destport;
	unsigned short tlen;
	unsigned short crc;
}udp_header, * pudp_header;

typedef struct tcp_header
{
	unsigned short sourport;
	unsigned short destport;
	unsigned long snum;
	unsigned long acknum;
	unsigned short off4_reser6_flag6;
	unsigned short window;
	unsigned short crc;
	unsigned short emerp;
}tcp_header, * ptcp_header;

typedef struct iph_pack
{
	unsigned short hlen;
	unsigned short tlen;
	unsigned long souraddr;
	unsigned long destaddr;
}iph_pack;

#pragma pack(pop)

#endif