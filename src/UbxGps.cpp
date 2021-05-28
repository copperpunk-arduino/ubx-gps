#include "UbxGps.h"

UbxGps::UbxGps()
{
    // Did not specify antenna separation, therefore assume that dual GPS antennas are not being used
    rel_pos_acc_thresh_mm_em1_ = 0;
    rel_pos_sep_exp_mm_em1_ = 0;
}

bool UbxGps::checkForMessage(Stream *port)
{
    if (ubx_.read(port))
    {
        processMessage();
        return true;
    }
    return false;
}

void UbxGps::processMessage()
{
    msg_class_ = ubx_.msgClass();
    msg_id_ = ubx_.msgId();
    switch (msg_class_)
    {
    case 0x01:
    {
        switch (msg_id_)
        {
        case 0x02:
        {
            // NAV-POSLLH
            itow_ms_ = ubx_.unpackUint32(0);
            longitude_deg_ = ubx_.unpackInt32(4);
            latitude_deg_ = ubx_.unpackInt32(8);
            height_mm_ = ubx_.unpackInt32(12);
            h_msl_mm_ = ubx_.unpackInt32(16);
            h_acc_mm_ = ubx_.unpackUint32(20);
            v_acc_mm_ = ubx_.unpackUint32(24);
            new_fix_ = true;
            break;
        }
        case 0x07:
        {
            // NAV-PVT
            itow_ms_ = ubx_.unpackUint32(0);
            year_ = ubx_.unpackUint16(4);
            month_ = ubx_.unpackUint8(6);
            day_ = ubx_.unpackUint8(7);
            hour_ = ubx_.unpackUint8(8);
            min_ = ubx_.unpackUint8(9);
            sec_ = ubx_.unpackUint8(10);
            validity_flags_ = ubx_.unpackUint8(11);
            time_acc_est_ = ubx_.unpackUint32(12);
            time_ns_ = ubx_.unpackInt32(16);
            fix_type_ = ubx_.unpackUint8(20);
            longitude_deg_ = ubx_.unpackInt32(24);
            latitude_deg_ = ubx_.unpackInt32(28);
            height_mm_ = ubx_.unpackInt32(32);
            h_msl_mm_ = ubx_.unpackInt32(36);
            v_north_mmps_ = ubx_.unpackInt32(48);
            v_east_mmps_ = ubx_.unpackInt32(52);
            v_down_mmps_ = ubx_.unpackInt32(56);
            new_fix_ = true;
            break;
        }

        case 0x3C:
        {
            // NAV-RELPOLNED
            itow_rel_ms_ = ubx_.unpackUint32(4);
            int32_t relPosDistance_cm = ubx_.unpackInt32(20);
            int32_t relHeading_deg_em5 = ubx_.unpackInt32(24);
            int8_t relPosHpDistance_mm_em1 = ubx_.unpackInt8(35);
            uint32_t flags = ubx_.unpackUint32(60);
            rel_pos_distance_mm_ = relPosDistance_cm * 10 + relPosHpDistance_mm_em1 * 0.1f;
            // Flags expected at bits 0,2,8 => 261
            if (flags & 261 == 261)
            {
                rel_heading_deg_ = relHeading_deg_em5 * (1e-5);
                new_rel_hdg_ = true;
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    }
}

uint8_t UbxGps::msgClass() { return msg_class_;}
uint8_t UbxGps::msgId() { return msg_id_;}

uint32_t UbxGps::iTowMs() { return itow_ms_; }
double UbxGps::timeFixSec()
{
    uint32_t itow_sec = (uint32_t)(itow_ms_ * (1e-3));
    return (double(itow_sec) + (double)time_ns_ * (1e-9));
}
uint32_t UbxGps::nano()
{
    return time_ns_;
}
uint8_t UbxGps::fixType() { return fix_type_; }
int32_t UbxGps::latitudeDeg() { return latitude_deg_; }
int32_t UbxGps::longitudeDeg() { return longitude_deg_; }
int32_t UbxGps::heightMm() { return height_mm_; }
int32_t UbxGps::hMslMm() { return h_msl_mm_; }
int32_t UbxGps::vNorthMmps() { return v_north_mmps_; }
int32_t UbxGps::vEastMmps() { return v_east_mmps_; }
int32_t UbxGps::vDownMmps() { return v_down_mmps_; }
uint32_t UbxGps::hAccMm() { return h_acc_mm_; }
uint32_t UbxGps::vAccMm() { return v_acc_mm_; }
bool UbxGps::isNewFix() { return new_fix_; }
void UbxGps::clearFix() { new_fix_ = false; }

double UbxGps::timeFixRelHdgSec()
{
    return (double(itow_rel_ms_) * (1e-3));
}

// NAV-RELPOSNED
float UbxGps::relHeadingDeg() { return rel_heading_deg_; }
float UbxGps::relPositionDistanceMm() { return rel_pos_distance_mm_; }
bool UbxGps::isNewRelHdg() { return new_rel_hdg_; }
void UbxGps::clearRelHdg() { new_rel_hdg_ = false; }

bool UbxGps::isRelPositionDistanceWithinThreshold(float expected_distance_mm, float accuracy_threshold_mm)
{
    return (abs(expected_distance_mm - rel_pos_distance_mm_) < accuracy_threshold_mm);
}
void UbxGps::buildNavPosLlhMessage(uint32_t itow_ms, float latitude_deg, float longitude_deg, int32_t height_mm, int32_t h_msl_mm, uint32_t h_acc_mm, uint32_t v_acc_mm)
{
    ubx_.setHeaderValues(0x01, 0x02, 28);
    ubx_.packValue(itow_ms, 0);
    ubx_.packValue((int32_t)(longitude_deg * 10000000), 4);
    ubx_.packValue((int32_t)(latitude_deg * 10000000), 8);
    ubx_.packValue(height_mm, 12);
    ubx_.packValue(h_msl_mm, 16);
    ubx_.packValue(h_acc_mm, 20);
    ubx_.packValue(v_acc_mm, 24);
    ubx_.prepareMessage();
}

void UbxGps::printMessageInTxBuffer(Stream * port, int output_type)
{
    ubx_.printWriteBuffer(port, output_type);
}

int UbxGps::writeMessageInTxBuffer(Stream * port)
{
    return ubx_.writeMessage(port);
}