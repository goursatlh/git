#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in_systm.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<arpa/inet.h>

#define CHKADDERESS(_saddr_) \
        { \
         u_char *p = ( char *) & ( _saddr_); \
         if((p[0]==10) \
		 ||  ( p[0] == 168 && 16 <= p[1] && p[1] <= 31) \
		 ||  (p[0] == 192 && p[1] == 168)) \
		 ; \
		 else \
		 	{ \
		 	fprintf(stderr," IP address error.\n"); \
			exit(EXIT_FAILURE); \
		 	} \
          }

enum
{ CMD_NAME, START_IP, LAST_IP };

#define BUFSIZE   4096
#define PACKET_LEN  72

void make_icmp8_packet(struct icmp *icmp, int len, int n);
void tvsub(struct timeval *out, struct timeval *in);
u_short checksum(u_short * data, int len);

int main(int argc, char **argv)
{
	struct sockaddr_in send_sa;
	int s;
	char send_buff[PACKET_LEN];
	char recv_buff[BUFSIZE];
	int endip;
	int dstip;
	int startip;

	struct timeval tv;
	int i;
	fd_set readfd;
	struct ip *ip;
	int hlen;

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s start_ip last_ip\n", argv[CMD_NAME]);
		exit(EXIT_FAILURE);
	}
	startip = ntohl(inet_addr(argv[START_IP]));
	endip = ntohl(inet_addr(argv[LAST_IP]));

	memset((char *)&send_sa, 0, sizeof(send_sa));
	send_sa.sin_family = AF_INET;

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	for (dstip = startip; dstip <= endip; dstip++)
	{
		send_sa.sin_addr.s_addr = htonl(dstip);
		// CHKADDERESS(send_sa.sin_addr);

		for (i = 0; i < 3; i++)
		{
			printf("scan %s (%d) \r", inet_ntoa(send_sa.sin_addr),
			       i + 1);
			fflush(stdout);

			make_icmp8_packet((struct icmp *)send_buff, PACKET_LEN,
					  i);
			if (sendto
			    (s, (char *)&send_buff, PACKET_LEN, 0,
			     (struct sockaddr *)&send_sa, sizeof(send_sa)) < 0)
			{
				perror(" send error");
				exit(EXIT_FAILURE);
			}

			tv.tv_sec = 0;
			tv.tv_usec = 200 * 1000;
			while (1)
			{
				FD_ZERO(&readfd);
				FD_SET(s, &readfd);

				if (select(s + 1, &readfd, NULL, NULL, &tv) <=
				    0)
					break;
				if (recvfrom
				    (s, recv_buff, BUFSIZE, 0, NULL, NULL) < 0)
				{
					perror(" recvfrom error");
					exit(EXIT_FAILURE);
				}
				ip = (struct ip *)recv_buff;
				hlen = ip->ip_hl << 2;

				if (ip->ip_src.s_addr ==
				    send_sa.sin_addr.s_addr)
				{
					struct icmp *icmp;

					icmp =
					    (struct icmp *)(recv_buff + hlen);

					if (icmp->icmp_type == ICMP_ECHOREPLY)
					{
						printf("%-15s",
						       inet_ntoa(*
								 (struct in_addr
								  *)&(ip->
								      ip_src.
								      s_addr)));

						gettimeofday(&tv,
							     (struct timezone *)
							     0);
						tvsub(&tv,
						      (struct timeval *)(icmp->
									 icmp_data));

						printf(":RTT = %8.4d ms\n",
						       tv.tv_sec * 1000.0 +
						       tv.tv_usec / 1000.0);
						goto exit_loop;
					}
				}
			}
		}
	      exit_loop:;
	}
	close(s);
	return EXIT_FAILURE;
}

void make_icmp8_packet(struct icmp *icmp, int len, int n)
{
	memset((char *)icmp, 0, len);

	gettimeofday((struct timeval *)(icmp->icmp_data), (struct timezone *)0);


	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = 0;
	icmp->icmp_seq = n;

	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *) icmp, len);
}


void tvsub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		out->tv_sec--;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}


u_short checksum(u_short * date, int len)
{
	u_long sum = 0;

	for (; len > 1; len -= 2)
	{
		sum += *date++;
		if (sum & 0x80000000)
			sum = (sum & 0xfff) + (sum >> 16);
	}

	if (len == 1)
	{
		u_short i = 0;

		*(u_char *) (&i) = *(u_char *) date;
		sum += i;
	}
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	return (sum == 0xffff) ? sum : ~sum;
}
