#define KBUILD_MODNAME "xdp_filter_tcp"
#include <linux/bpf.h>
#include <linux/if_ether.h>   
#include <linux/in.h>         
#include <linux/ip.h>         
#include <linux/tcp.h>


BPF_TABLE("percpu_array", uint32_t, long, packetcnt, 256);

int myprogram(struct xdp_md *ctx) 
{
	int ipsize = 0;
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	struct ethhdr *eth = data; // so when xdp hooks the input packet, it has been disencapsulated the hardware header.
	struct iphdr *ip;
	long *cnt;
	__u32 idx;
	ipsize = sizeof(*eth);
	ip = data + ipsize;
	ipsize += sizeof(struct iphdr);

	if (data + ipsize > data_end) 
    {
		return XDP_DROP;
	}
	
#if 1
    idx = ip->protocol;
	cnt = packetcnt.lookup(&idx);
	if (cnt) 
    {
		*cnt += 1;
	}
#endif
	if (ip->protocol == IPPROTO_ICMP) 
    {
        //printk("Drop all the ICMP packets.\n");
        bpf_trace_printk("Drop all the ICMP packets, ip len %d.\n", ip->tot_len);
        //bpf_trace_printk("Drop all the ICMP packets.\n");
		return XDP_DROP;
	}
	return XDP_PASS;
}
