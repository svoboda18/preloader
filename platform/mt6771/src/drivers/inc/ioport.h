
#ifndef _LINUX_IOPORT_H
#define _LINUX_IOPORT_H


/*
 * IO resources have these defined flags.
 */
#define IORESOURCE_BITS         0x000000ff      /* Bus-specific bits */

#define IORESOURCE_TYPE_BITS    0x00001f00      /* Resource type */
#define IORESOURCE_IO           0x00000100      /* PCI/ISA I/O ports */
#define IORESOURCE_MEM          0x00000200
#define IORESOURCE_REG          0x00000300      /* Register offsets */
#define IORESOURCE_IRQ          0x00000400
#define IORESOURCE_DMA          0x00000800
#define IORESOURCE_BUS          0x00001000

#define IORESOURCE_PREFETCH     0x00002000      /* No side effects */
#define IORESOURCE_READONLY     0x00004000
#define IORESOURCE_CACHEABLE    0x00008000
#define IORESOURCE_RANGELENGTH  0x00010000
#define IORESOURCE_SHADOWABLE   0x00020000

struct resource {
        //resource_size_t start;
        //resource_size_t end;
	unsigned long start;
	unsigned long end;
        const char *name;
        unsigned long flags;
        struct resource *parent, *sibling, *child;
};

#endif
