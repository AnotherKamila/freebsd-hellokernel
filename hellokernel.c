#include <sys/errno.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/sysctl.h>
#include <sys/conf.h>
#include <sys/bus.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>
/* #include <dev/usb/usbdi_util.h> */

#include <machine/bus.h>

struct hello_softc {
	device_t dev;
};

/*
 * Device methods.
 * See driver(9).
 */
static int hello_usb_probe(device_t dev);
static int hello_usb_attach(device_t dev);
static int hello_usb_detach(device_t dev);

static const struct hello_device {
	uint16_t idVendor;
	uint16_t idProduct;
	const char *desc;
} hello_devices[] = {
	{ 0x1d50, 0x60e6, "Hello Kernel Magic Gadget (the expensive one)" },
	{ 0, 0, "--SENTINEL--" }
};

/*
 * Implementation
 */

static int hello_usb_probe(device_t dev) {
	struct usb_attach_arg *uaa = device_get_ivars(dev);

	/* Ensure we are in host mode */
	if (uaa->usb_mode != USB_MODE_HOST) return (ENXIO);

	/* Is this a device we know? */
	for (int i = 0; hello_devices[i].idVendor != 0; i++) {
		if (hello_devices[i].idVendor  == uaa->info.idVendor &&
				hello_devices[i].idProduct == uaa->info.idProduct) {
			device_set_desc(dev, hello_devices[i].desc);
			return BUS_PROBE_DEFAULT;
		}
	}

	return ENXIO;
}
static int hello_usb_attach(device_t dev) {
	struct hello_softc *sc = device_get_softc(dev);
	sc->dev = dev;
	device_printf(dev, "I have no idea what I'm doing!\n");
	return (0);
}
static int hello_usb_detach(device_t dev) {
	device_printf(dev, "good bye cruel world\n");
	return (0);
}

/*
 * Driver declaration.
 */

static device_method_t hello_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe, hello_usb_probe),
	DEVMETHOD(device_attach, hello_usb_attach),
	DEVMETHOD(device_detach, hello_usb_detach),

	DEVMETHOD_END
};

static driver_t hello_driver = {
																"hello",
																hello_methods,
																sizeof(struct hello_softc),
};

static devclass_t hello_devclass;
DRIVER_MODULE(hello, uhub, hello_driver, hello_devclass, NULL, NULL);
MODULE_DEPEND(hello, usb, 1, 1, 1);
