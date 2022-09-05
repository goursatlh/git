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

int kprobe__icmp_rcv(struct pt_regs *ctx, struct sk_buff *skb) 
{
    struct icmphdr *icmph = (struct icmphdr *)(skb->head + skb->transport_header);
    bpf_trace_printk("icmp_rcv skb len %d/%d dev %s\\n", skb->len, skb->data_len, skb->dev->name);
    bpf_trace_printk("icmp_rcv type: %d code %d transport_header %d\\n", icmph->type, icmph->code, skb->transport_header); 
    return 0;
}
"""

b = BPF(text=bpf_text)
b.trace_print()




