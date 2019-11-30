# A generic "build me a kernel module" Makefile comes with FreeBSD, so we just
# need to include it.

KMOD=	hellokernel
SRCS=	hellokernel.c device_if.h bus_if.h opt_usb.h

.include <bsd.kmod.mk>
