
#include <linux/bpf.h>
#include <linux/kernel.h>
#include <linux/pkt_cls.h>
#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <linux/if_ether.h>
#include <linux/in.h>    
#include <linux/ip.h>

//#define SEC(NAME) __attribute__((section(NAME), used))

SEC("simple")
int handle_ingress(struct __sk_buff *skb)
{
    char msg[] = "ICMP drop";
	void *data_end = (void *)(long)skb->data_end;
	void *data = (void *)(long)skb->data;
    struct ethhdr *eth = data;
    int nh_off = 0;
    nh_off = sizeof(*eth);
	struct iphdr *iph = data + nh_off;

	if (iph + 1 > data_end) 
    {
		return 0;
	}
	if (iph->protocol == IPPROTO_ICMP) 
    {
        bpf_printk("Drop ICMP packets");
        bpf_trace_printk(msg, sizeof(msg));
		return TC_ACT_SHOT;
	}
    return TC_ACT_OK;
}

char __license[] SEC("license") = "GPL";
