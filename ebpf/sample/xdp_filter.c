#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
 
#define SEC(NAME) __attribute__((section(NAME), used))
 
SEC("mysection")
int filter(struct xdp_md *ctx) {
    int ipsize = 0;
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;
    struct ethhdr *eth = data;
    struct iphdr *ip;
 
    ipsize = sizeof(*eth);
    ip = data + ipsize;
 
    ipsize += sizeof(struct iphdr);
    if (data + ipsize > data_end) {
        return XDP_DROP;
    }
 
    if (ip->protocol == IPPROTO_ICMP) {
        return XDP_DROP;
    }
 
    return XDP_PASS;
}
 
