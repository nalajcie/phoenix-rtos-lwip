#include <stdio.h>
#include "pci.h"
#include "physmmap.h"


int pci_parseDevnum(const char *s)
{
	unsigned int bus, dev, fn;

	if (sscanf(s, "%x:%x.%u", &bus, &dev, &fn) != 3)
		return -1;

	if (bus > 0xFF || dev > 0x1F || fn > 7)
		return -1;

	return pci_makeDevNum(bus, dev, fn);
}


void pci_setBusMaster(u16 devnum, int enable)
{
	u32 cmds;

	cmds = pci_configRead(devnum, 0x04);
	cmds |= 0x04;
	pci_configWrite(devnum, 0x04, cmds);
}


volatile void *pci_mapMemBAR(u16 devnum, int bar)
{
	volatile void *p;
	u32 sz, pa;
	u64 v;

	v = pci_configReadBAR(devnum, bar);
	sz = v >> 32;
	pa = v;

	if (!sz)
		return NULL;

	p = physmmap(pa, sz + 1);
	if (p == MAP_FAILED)
		return NULL;

	return p;
}
