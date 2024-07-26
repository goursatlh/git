#!/usr/bin/env python3

from bcc import BPF

bpf_text = """
#include <linux/fs.h> 
#include <linux/kobject.h> 
#include <bcc/proto.h>
#include <uapi/linux/ptrace.h>

int kprobe__kobject_create_and_add(struct pt_regs *ctx, const char *name, struct kobject *parent) 
{
    bpf_trace_printk("kobject_create_and_add %s\\n", name);
    //bpf_trace_printk("kobject_create_and_add %s/%s\\n", name, parent->name);
    return 0;
}
"""

b = BPF(text=bpf_text)
b.trace_print()




