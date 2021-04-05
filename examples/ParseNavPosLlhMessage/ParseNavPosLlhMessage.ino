#include "UbxGps.h"

/*
    This sketch sends the same U-blox NAV-POSLLH message once per second.
    An example of the message definition can be found in the ZED-F9P Interface Description:
    https://www.u-blox.com/sites/default/files/ZED-F9P_InterfaceDescription_%28UBX-18010854%29.pdf

    The TX and RX pins on the data_port should be connected.
*/

#define debug_port Serial
#define data_port Serial1

// const int kMessageLength = 36;
// uint8_t kTestMessage[] = {181, 98, 1, 2, 28, 0, 229, 207, 145, 8, 0, 71, 72, 183, 128, 116, 210, 26, 120, 224, 1, 0, 64, 245, 6, 0, 252, 3, 0, 0, 248, 7, 0, 0, 36, 18};

long system_time_prev_ms_;

UbxGps gps_;

int msg_class_ = 1;
int msg_id_ = 2;
uint32_t itow_ = 150000000;
float latitude_deg_ = 45.0f;
float lontidue_deg_ = -120.0f;
int32_t height_mm_ = 123000;
int32_t h_msl_mm_ = 456000;
uint32_t h_acc_mm_ = 1020;
uint32_t v_acc_mm_ = 2040;
uint16_t kPayloadLength = 28;

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
system_time_prev_ms_ = millis();
}

void loop()
{
    // Update system time
    long current_time = millis();
    itow_ += (current_time - system_time_prev_ms_);
    system_time_prev_ms_ = current_time;
    
    // Build Message
    gps_.buildNavPosLlhMessage(itow_, latitude_deg_, lontidue_deg_, height_mm_, h_msl_mm_, h_acc_mm_, v_acc_mm_);

    // Output message to debug port
    debug_port.print(F("\nwrite message: "));
    gps_.printMessageInTxBuffer(&debug_port, HEX);

    // Write message to data port
    gps_.writeMessageInTxBuffer(&data_port);
    data_port.flush();

    // Check for new data
    while (data_port.available())
    {
        if (gps_.checkForMessage(&data_port))
        {
            debug_port.print("Message rx'd with class ");
            debug_port.print(gps_.msgClass());
            debug_port.print(F(" and ID "));
            debug_port.print(gps_.msgId());
            // gps_.processMessage();
            if ((gps_.msgClass() == 0x01) && (gps_.msgId() == 0x02))
            {
                // See earlier value definitions for expected results
                debug_port.print(F("\niTOW: "));
                debug_port.print(gps_.iTowMs());
                debug_port.print(F("\nlatitude: "));
                debug_port.print(gps_.latitudeDeg() * 1e-7);
                debug_port.print(F("\nlongitude: "));
                debug_port.print(gps_.longitudeDeg() * 1e-7);
                debug_port.print(F("\nheight above ellipsoid: "));
                debug_port.print(gps_.heightMm());
                debug_port.print(F("\nheight above MSL: "));
                debug_port.print(gps_.hMslMm());
                debug_port.print(F("\nhAcc: "));
                debug_port.print(gps_.hAccMm());
                debug_port.print(F("\nvAcc: "));
                debug_port.print(gps_.vAccMm());
                gps_.clearFix();
            }
            debug_port.print(F("\n\n"));
        }
    }
    latitude_deg_ += 0.1;
    lontidue_deg_ -= 0.05;
    height_mm_ -= 5;
    h_msl_mm_ += 10;
    h_acc_mm_++;
    v_acc_mm_ += 2;
    delay(1000);
}