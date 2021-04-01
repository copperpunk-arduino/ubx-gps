#include "UbxGps.h"

#define debug_port SerialUSB
#define gps_port Serial1

#define DEBUG
#ifdef DEBUG
#define DebugPrint(x) debug_port.print(x)
#define DebugPrintDec(x) debug_port.print(x, DEC)
#define DebugPrintHex(x) debug_port.print(x, HEX)
#define DebugPrintln(x) debug_port.println(x)
#else
#define DebugPrint(x)
#define DebugPrintDec(x)
#define DebugPrintHex(x)
#define DebugPrintln(x)
#endif

const float kDeg2Rad = 0.017453293f;

// GPS Definitions
UbxGps gps_; // This parses received characters
const long kGpsBaud = 115200;
double gps_latitude_deg_;
double gps_longitude_deg_;
double gps_height_m_;
float gps_velocity_mps_[3] = {0.f, 0.f, 0.f};
double gps_fix_time_sec_;
double gps_fix_time_prev_sec_ = 0.;
uint8_t gps_status_ = 0;

// Heading Definitions
const float kExpectedRelPositionDistanceMm = 1822.5f;
const float kRelPositionAccMm = 200.f;
double rel_hdg_time_sec_;
double rel_hdg_time_prev_sec_ = 0.;
float rel_hdg_deg_ = 0.F;

void setup()
{
#ifdef DEBUG
    debug_port.begin(115200);
    while (!debug_port)
    {
    }
#endif
    DebugPrint(F("Gps Setup..."));
    gps_port.begin(kGpsBaud);
    gps_port.setTimeout(1);
    // Clear GPS buffer
    while (gps_.read(&gps_port))
        ;
    DebugPrintln(F("Done!"));
}

void loop()
{
    while (gps_.read(&gps_port))
    {
        gps_.processMessage();
    }
    if (gps_.isNewFix())
    {
        gps_.clearFix();
        updateGpsFix();
        printPosition();
        printVelocity();
    }
    if (gps_.isNewRelHdg())
    {
        gps_.clearRelHdg();
        if (abs(gps_.relPositionDistanceMm() - kExpectedRelPositionDistanceMm) < kRelPositionAccMm)
        {
            updateRelativeHeading();
        }
        else
        {
            DebugPrintln("antenna relative dist[mm] out of range: " + String(gps_.relPositionDistanceMm()));
        }
        printRelativeHeading();
    }
}

void updateGpsFix()
{
    gps_status_ = gps_.fixType();
    gps_fix_time_sec_ = gps_.timeFixSec();
    float dt = (gps_fix_time_sec_ - gps_fix_time_prev_sec_);
    if (dt > 0)
    {
        gps_fix_time_prev_sec_ = gps_fix_time_sec_;
        if (gps_status_ > 2)
        {
            gps_latitude_deg_ = gps_.latitudeDeg() * (1e-7);
            gps_longitude_deg_ = gps_.longitudeDeg() * (1e-7);
            gps_height_m_ = gps_.heightMm() * (1e-3);
            gps_velocity_mps_[0] = gps_.vNorthMmps() * 1e-3;
            gps_velocity_mps_[1] = gps_.vEastMmps() * 1e-3;
            gps_velocity_mps_[2] = -gps_.vDownMmps() * 1e-3;
        }
    }
}

void updateRelativeHeading()
{
    rel_hdg_time_sec_ = gps_.timeFixRelHdgSec();
    float dt = (rel_hdg_time_sec_ - rel_hdg_time_prev_sec_);
    if (dt > 0)
    {
        rel_hdg_time_prev_sec_ = rel_hdg_time_sec_;
        rel_hdg_deg_ = gps_.relHeadingDeg();
    }
}

void printPosition()
{
    DebugPrintln("lat[deg]/lon[deg]/hgt[m]: " + String(gps_latitude_deg_, 7) + "/" + String(gps_longitude_deg_, 7) + "/" + String(gps_height_m_, 2));
}

void printVelocity()
{
    DebugPrintln("vel[mps] X/Y/Z: " + String(gps_velocity_mps_[0], 2) + "/" + String(gps_velocity_mps_[1], 2) + "/" + String(gps_velocity_mps_[2], 2));
}

void printRelativeHeading()
{
    DebugPrintln("Rel Hdg[deg]: " + String(rel_hdg_deg_, 1));
}