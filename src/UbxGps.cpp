#include "UbxGps.h"

UbxGps::UbxGps()
{
    // Did not specify antenna separation, therefore assume that dual GPS antennas are not being used
    rel_pos_acc_thresh_mm_em1_ = 0;
    rel_pos_sep_exp_mm_em1_ = 0;
}

// UbxGps::UbxGps(uint32_t antenna_separation_mm, uint32_t relative_position_accuracy_threshold_mm)
// {
//     rel_pos_sep_exp_mm_em1_ = antenna_separation_mm * 10;
//     rel_pos_acc_thresh_mm_em1_ = relative_position_accuracy_threshold_mm * 10;
// }

void UbxGps::processMessage()
{
    switch (msgClass())
    {
    case 0x01:
    {
        switch (msgId())
        {
        case 0x02:
        {
            // NAV-POSLLH
            itow_ms_ = unpackUint32(0);
            longitude_deg_ = unpackInt32(4);
            latitude_deg_ = unpackInt32(8);
            height_mm_ = unpackInt32(12);
            h_msl_mm_ = unpackInt32(16);
            h_acc_mm_ = unpackInt32(20);
            v_acc_mm_ = unpackInt32(24);
            new_fix_ = true;
            break;
        }
        case 0x07:
        {
            // NAV-PVT
            itow_ms_ = unpackUint32(0);
            year_ = unpackUint16(4);
            month_ = unpackUint8(6);
            day_ = unpackUint8(7);
            hour_ = unpackUint8(8);
            min_ = unpackUint8(9);
            sec_ = unpackUint8(10);
            validity_flags_ = unpackUint8(11);
            time_acc_est_ = unpackUint32(12);
            time_ns_ = unpackInt32(16);
            fix_type_ = unpackUint8(20);
            longitude_deg_ = unpackInt32(24);
            latitude_deg_ = unpackInt32(28);
            height_mm_ = unpackInt32(32);
            h_msl_mm_ = unpackInt32(36);
            v_north_mmps_ = unpackInt32(48);
            v_east_mmps_ = unpackInt32(52);
            v_down_mmps_ = unpackInt32(56);
            new_fix_ = true;
            break;
        }

        case 0x3C:
        {
            // NAV-RELPOLNED
            itow_rel_ms_ = unpackUint32(4);
            int32_t relPosDistance_cm = unpackInt32(20);
            int32_t relHeading_deg_em5 = unpackInt32(24);
            int8_t relPosHpDistance_mm_em1 = unpackInt8(25);
            uint32_t flags = unpackUint32(60);
            rel_pos_distance_mm_ = relPosDistance_cm * 10 + relPosHpDistance_mm_em1 * 0.1f;
            // Flags expected at bits 0,2,8 => 261
            if (flags & 261 == 261) // && (abs(rel_pos_length_mm_em1 - rel_pos_sep_exp_mm_em1_) < rel_pos_acc_thresh_mm_em1_))
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