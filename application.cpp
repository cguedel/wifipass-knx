#include <qrcode.h>
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2.h>

#define MAX_DISPAY_BUFFER_SIZE 15000ul
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8))

#define QRD_CS  11
#define QRD_DC  7
#define QRD_RST 12
#define QRD_BSY MISO

GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(QRD_CS, QRD_DC, QRD_RST, QRD_BSY));

#define KONNEKTING_SYSTEM_TYPE_SIMPLE
#include <KonnektingDevice.h>
#include "kdevice_wifipass-knx.h"
#include "flashmemory.h"

#define KNX_SERIAL Serial1
#define KNX_PROG_PIN 3
#define KNX_PROG_LED LED_BUILTIN
#define KNX_DEBUGSERIAL Serial
#define KDEBUG // comment this line to disable DEBUG mode
#ifdef KDEBUG
#include <DebugUtil.h>
#endif

bool needsDisplayUpdate = false;
String ssid1;
String ssid2;

void setup()
{
    display.init(115200);

#ifdef KDEBUG
    // Start debug serial with 115200 bauds
    KNX_DEBUGSERIAL.begin(115200);

    // wait for serial port to connect. Needed for Leonardo/Micro/ProMicro/Zero only
    while (!KNX_DEBUGSERIAL)
        ;

    // make debug serial port known to debug class
    // Means: KONNEKTING will sue the same serial port for console debugging
    Debug.setPrintStream(&KNX_DEBUGSERIAL);
#endif

    Konnekting.setMemoryReadFunc(&readMemory);
    Konnekting.setMemoryWriteFunc(&writeMemory);
    Konnekting.setMemoryUpdateFunc(&updateMemory);
    Konnekting.setMemoryCommitFunc(&commitMemory);

    Konnekting.init(KNX_SERIAL,
                    KNX_PROG_PIN,
                    KNX_PROG_LED,
                    MANUFACTURER_ID,
                    DEVICE_ID,
                    REVISION);

    if (!Konnekting.isFactorySetting())
    {
        String ssid1 = Konnekting.getSTRING11Param(PARAM_ssid1);
        String ssid2 = Konnekting.getSTRING11Param(PARAM_ssid2);

        needsDisplayUpdate = true;
    }
}

void loop()
{
    Knx.task();

    if (needsDisplayUpdate)
    {
        showQRCode("foobar", "WPA");
        needsDisplayUpdate = false;
    }
}

void showQRCode(char password[], char security[])
{
    QRCode qrcode;
    int version = 3;
    uint8_t qrcodeBytes[qrcode_getBufferSize(version)];

    char data[256];
    snprintf(data, sizeof(data), "WIFI:S:%s%s;T;%s;P:%s;;", ssid1.c_str(), ssid2.c_str(), security, password);
    qrcode_initText(&qrcode, qrcodeBytes, version, ECC_LOW, data);

    int factor = 190 / qrcode.size;
    int offset = (200 - (qrcode.size * factor)) / 2;

    display.firstPage();

    do
    {
        display.fillScreen(GxEPD_WHITE);

        for (int qry = 0; qry < qrcode.size; qry++)
        for (int qrx = 0; qrx < qrcode.size; qrx++)           
            if (qrcode_getModule(&qrcode, qrx, qry))
                for (int x = qrx * factor; x < qrx * factor + factor; x++)
                for (int y = qry * factor; y < qry * factor + factor; y++)
                    display.drawPixel(offset + x, offset + y, GxEPD_BLACK);
    } while (display.nextPage());

    display.hibernate();
}

void knxEvents(byte index)
{
    switch (index)
    {
    case COMOBJ_ssid1:
        byte ssid1bytes[16];
        if (Konnekting.getUINT8Param(PARAM_comobject_ssid) == 1 && Knx.read(COMOBJ_ssid1, ssid1bytes) == KNX_DEVICE_OK)
        {
            ssid1 = String(reinterpret_cast<const char*>(ssid1bytes));
            Knx.write(COMOBJ_ssid1_status, (byte*)ssid1.c_str());
            needsDisplayUpdate = true;
        }
        break;
    case COMOBJ_ssid2: 
        byte ssid2bytes[16];
        if (Konnekting.getUINT8Param(PARAM_comobject_ssid) == 1 && Knx.read(COMOBJ_ssid2, ssid2bytes) == KNX_DEVICE_OK)
        {
            ssid2 = String(reinterpret_cast<const char*>(ssid2bytes));
            Knx.write(COMOBJ_ssid2_status, (byte*)ssid2.c_str());
            needsDisplayUpdate = true;
        }
        break;

    default:
        break;
    }
};