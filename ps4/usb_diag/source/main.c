// usb_diag: plugin de DIAGNOSTICO para GoldHEN. Solo LEE descriptores USB
// (no abre, no reclama interfaces, no envia datos a ningun dispositivo).
// Escribe en /data/GoldHEN/usb_diag.log que teclados/ratones ve la consola.
// Estructura basada en gamepad_helper (GoldHEN_Plugins_Repository).

#include "plugin_common.h"
#include "Common.h"
#include <orbis/Usbd.h>

attr_public const char* g_pluginName = "usb_diag";
attr_public const char* g_pluginDesc = "USB keyboard/mouse diagnostic";
attr_public const char* g_pluginAuth = "ps4-kbm";
attr_public u32 g_pluginVersion = 0x00000100; // 1.00

#define DIAG_LOG GOLDHEN_PATH "/usb_diag.log"

static FILE* g_log = NULL;
#define LOG(...) do { final_printf(__VA_ARGS__); \
  if (g_log) { fprintf(g_log, __VA_ARGS__); fflush(g_log); } } while (0)

static const char* hid_kind(uint8_t cls, uint8_t sub, uint8_t proto) {
  if (cls != 3) return "no-HID";
  if (sub == 1 && proto == 1) return "HID boot TECLADO";
  if (sub == 1 && proto == 2) return "HID boot RATON";
  if (sub == 1) return "HID boot (otro)";
  return "HID (formato propio, no boot)";
}

static void dump_device(int idx, libusb_device* dev) {
  struct libusb_device_descriptor dd;
  if (sceUsbdGetDeviceDescriptor(dev, &dd) != 0) {
    LOG("[%d] no se pudo leer el descriptor del dispositivo\n", idx);
    return;
  }
  LOG("[%d] VID=%04x PID=%04x clase=%u bus=%u dir=%u\n", idx, dd.idVendor, dd.idProduct,
      dd.bDeviceClass, sceUsbdGetBusNumber(dev), sceUsbdGetDeviceAddress(dev));

  struct libusb_config_descriptor* cfg = NULL;
  if (sceUsbdGetActiveConfigDescriptor(dev, &cfg) != 0 || cfg == NULL) {
    LOG("    sin configuracion activa legible\n");
    return;
  }
  for (int i = 0; i < cfg->bNumInterfaces; i++) {
    const struct libusb_interface* itf = &cfg->interface[i];
    for (int a = 0; a < itf->num_altsetting; a++) {
      const struct libusb_interface_descriptor* d = &itf->altsetting[a];
      LOG("    interfaz %u: clase=%u sub=%u proto=%u -> %s\n", d->bInterfaceNumber,
          d->bInterfaceClass, d->bInterfaceSubClass, d->bInterfaceProtocol,
          hid_kind(d->bInterfaceClass, d->bInterfaceSubClass, d->bInterfaceProtocol));
      for (int e = 0; e < d->bNumEndpoints; e++) {
        const struct libusb_endpoint_descriptor* ep = &d->endpoint[e];
        LOG("      endpoint 0x%02x tipo=%u paquete=%u intervalo=%u\n", ep->bEndpointAddress,
            ep->bmAttributes & 3, ep->wMaxPacketSize, ep->bInterval);
      }
    }
  }
  sceUsbdFreeConfigDescriptor(cfg);
}

s32 attr_public plugin_load(s32 argc, const char* argv[]) {
  final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
  g_log = fopen(DIAG_LOG, "w");

  char module[256];
  snprintf(module, 256, "/%s/common/lib/%s", sceKernelGetFsSandboxRandomWord(), "libSceUsbd.sprx");
  int h = 0;
  int r = sys_dynlib_load_prx(module, &h);
  LOG("cargar libSceUsbd.sprx: ret=0x%x handle=%d\n", r, h);

  int32_t rc = sceUsbdInit();
  LOG("sceUsbdInit: %d\n", rc);

  libusb_device** list = NULL;
  int32_t n = sceUsbdGetDeviceList(&list);
  LOG("dispositivos USB encontrados: %d\n", n);
  for (int i = 0; i < n && list; i++) dump_device(i, list[i]);
  if (list) sceUsbdFreeDeviceList(list);

  LOG("fin del diagnostico\n");
  if (g_log) { fclose(g_log); g_log = NULL; }
  return 0;
}

s32 attr_public plugin_unload(s32 argc, const char* argv[]) {
  final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
  return 0;
}

s32 attr_module_hidden module_start(s64 argc, const void* args) { return 0; }
s32 attr_module_hidden module_stop(s64 argc, const void* args) { return 0; }
