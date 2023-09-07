#!/usr/bin/env python3
from bcc import BPF

# Hello BPF Program
bpf_text = """ 
#include <net/inet_sock.h>
#include <bcc/proto.h>
#include <linux/ip.h>

#if 0
// 1. Attach kprobe to "inet_listen"
int kprobe__inet_listen(struct pt_regs *ctx, struct socket *sock, int backlog)
{
    bpf_trace_printk("Hello World!\\n");
    return 0;
}
#endif

#if 0
int kprobe__ip_local_out(struct pt_regs *ctx, struct sk_buff *skb)
{
    struct iphdr *iph = ip_hdr(skb);
    u8 proto = 0;
    bpf_probe_read(&proto, sizeof(proto), &iph->protocol);
    bpf_trace_printk("Hello World!\\n");
    return 0;
}
#endif

int kprobe____ip_finish_output(struct pt_regs *ctx, struct sk_buff *skb)
{
    //struct iphdr *iph = ip_hdr(skb);
    //u8 proto = 0;
    //bpf_probe_read(&proto, sizeof(proto), &iph->protocol);
    bpf_trace_printk("Hello World!\\n");
    return 0;
}
"""

# 2. Build and Inject program
b = BPF(text=bpf_text)

# 3. Print debug output
while True:
    print(b.trace_readline())



