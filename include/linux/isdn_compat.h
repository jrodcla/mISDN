#ifndef _LINUX_ISDN_COMPAT_H
#define _LINUX_ISDN_COMPAT_H

#ifdef __KERNEL__
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,2,18)

#define set_current_state(sta) (current->state = sta)
#define module_init(x)  int init_module(void) { return x(); }
#define module_exit(x)  void cleanup_module(void) { x(); }
#define BUG() do { printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); *(int
#define init_MUTEX(x)                           *(x)=MUTEX
#define init_MUTEX_LOCKED(x)                    *(x)=MUTEX_LOCKED
#define __devinit
#define __devinitdata

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)

#define COMPAT_HAS_2_2_PCI
#define get_pcibase(ps, nr) ps->base_address[nr]
#define pci_resource_start_io(pdev, nr) ((pdev)->base_address[nr] & PCI_BASE_ADDRESS_IO_MASK)
#define pci_resource_start_mem(pdev, nr) ((pdev)->base_address[nr] & PCI_BASE_ADDRESS_MEM_MASK)
#define pci_get_sub_vendor(pdev, id)	pci_read_config_word(pdev, PCI_SUBSYSTEM_VENDOR_ID, &id)
#define pci_get_sub_system(pdev, id)	pci_read_config_word(pdev, PCI_SUBSYSTEM_ID, &id)

#else /* 2.4.0 and later */

#define pci_resource_start_io(pdev, nr) pci_resource_start(pdev, nr)
#define pci_resource_start_mem(pdev, nr) pci_resource_start(pdev, nr)
#define get_pcibase(ps, nr) ps->resource[nr].start
#define pci_get_sub_system(pdev, id)	id = pdev->subsystem_device
#define pci_get_sub_vendor(pdev, id)	id = pdev->subsystem_vendor

#endif /* 2,4,0 */

#endif /* __KERNEL__ */
#endif /* _LINUX_ISDN_COMPAT_H */