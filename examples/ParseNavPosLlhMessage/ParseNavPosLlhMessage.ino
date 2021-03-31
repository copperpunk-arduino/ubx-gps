#include "UbxIns.h"

/*
    This sketch sends the same U-blox NAV-POSLLH message once per second.
    An example of the message definition can be found in the ZED-F9P Interface Description:
    https://www.u-blox.com/sites/default/files/ZED-F9P_InterfaceDescription_%28UBX-18010854%29.pdf

    The TX and RX pins on the data_port should be connected.
*/

#define debug_port Serial
#define data_port Serial1

const int kMessageLength = 36;
uint8_t kTestMessage[] = {181, 98, 1, 2, 28, 0, 229, 207, 145, 8, 0, 71, 72, 183, 128, 116, 210, 26, 120, 224, 1, 0, 64, 245, 6, 0, 252, 3, 0, 0, 248, 7, 0, 0, 36, 18};

UbxIns ins_;

void setup()
{
    debug_port.begin(115200);
    while (!debug_port)
    {
    }
    data_port.begin(115200);
    while (!data_port)
    {
    }
}

void loop()
{
    debug_port.print(F("\nwrite message\n"));
    data_port.write(kTestMessage, kMessageLength);
    data_port.flush();
    while (data_port.available())
    {
        if (ins_.parse(data_port.read()))
        {
            debug_port.print("Message rx'd with class ");
            debug_port.print(ins_.msgClass());
            debug_port.print(F(" and ID "));
            debug_port.print(ins_.msgId());
            debug_port.print(F("\nTime: "));
            debug_port.print(millis() * 1e-3, 3);

            if ((ins_.msgClass() == 0x01) && (ins_.msgId() == 0x02))
            {
                // Expected values are as follows (see U-blox Interface Description
                // for appropriate scaling and units):
                // iTOW: 143773669 [ms]
                // Latitude: 45.00 [deg]
                // Longitude: -122.00 [deg]
                // Height above ellipsoid: 123000 [mm]
                // Height above MSL: 456000 [mm]
                // hAcc: 1020 [mm]
                // vAcc: 2040 [mm]
                ins_.processMessage();
                debug_port.print(F("\niTOW: "));
                debug_port.print(ins_.iTOW());
                debug_port.print(F("\nlatitude: "));
                debug_port.print(ins_.latitude() * 1e-7);
                debug_port.print(F("\nlongitude: "));
                debug_port.print(ins_.longitude() * 1e-7);
                debug_port.print(F("\nheight above ellipsoid: "));
                debug_port.print(ins_.height());
                debug_port.print(F("\nheight above MSL: "));
                debug_port.print(ins_.hMsl());
                debug_port.print(F("\nhAcc: "));
                debug_port.print(ins_.hAcc());
                debug_port.print(F("\nvAcc: "));
                debug_port.print(ins_.vAcc());
            }
            debug_port.print(F("\n\n"));
        }
    }
    delay(1000);
}
