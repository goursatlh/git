#!/usr/bin/env python3

from bcc import BPF

bpf_text = """
#include <linux/fs.h> 
#include <bcc/proto.h>
#include <linux/skbuff.h>
#include <net/inet_sock.h>
#include <bcc/proto.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <uapi/linux/ptrace.h>
#include <net/sock.h>

#if 0
int kprobe__ip_rcv_finish(struct pt_regs *ctx, struct net *net, struct sock *sk, struct sk_buff *skb) 
{
    struct iphdr *iph = (struct iphdr *)(skb->head + skb->network_header);
    bpf_trace_printk("ip_rcv_finish skb len %d/%d dev %s\\n", skb->len, skb->data_len, skb->dev->name);
    bpf_trace_printk("ip_rcv_finish dst: %08x src %08x protocol %d\\n", 
                      bpf_ntohl(iph->daddr), bpf_ntohl(iph->saddr), iph->protocol);
    return 0;
}
#endif

int kprobe__icmp_rcv(struct pt_regs *ctx, struct sk_buff *skb) 
{
    struct icmphdr *icmph = (struct icmphdr *)(skb->head + skb->transport_header);
    bpf_trace_printk("icmp_rcv skb len %d/%d dev %s\\n", skb->len, skb->data_len, skb->dev->name);
    bpf_trace_printk("icmp_rcv type: %d code %d transport_header %d\\n", icmph->type, icmph->code, skb->transport_header); 
    return 0;
}

#if 0
int kprobe__tcp_v4_connect(struct pt_regs *ctx, struct sock *sk, struct sockaddr *uaddr, int addr_len) 
{
    //bpf_trace_printk("%s/%d : hello world\\n", __FUNCTION__, __LINE__);
    bpf_trace_printk("tcp_v4_connect dst: %x \\n", ((struct sockaddr_in *)uaddr)->sin_addr.s_addr);
    return 0;
}
#endif
"""

b = BPF(text=bpf_text)
b.trace_print()




