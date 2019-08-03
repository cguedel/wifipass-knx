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

void setup()
{
    display.init(115200);
    showQRCode("foobar", "WPA", "12345");
}

void loop()
{
}

void showQRCode(char ssid[], char password[], char security[])
{
    QRCode qrcode;
    int version = 3;
    uint8_t qrcodeBytes[qrcode_getBufferSize(version)];

    char data[256];
    snprintf(data, sizeof(data), "WIFI:S:%s;T;%s;P:%s;;", ssid, security, password);
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