#include<linux/kernel.h>
#include<linux/syscalls.h>
#include<linux/smp.h>

SYSCALL_DEFINE0(get_cpuid)
{
	int cpu = raw_smp_processor_id();
	return cpu;
}
