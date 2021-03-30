#include "UbxIns.h"

UbxIns::UbxIns()
{
    rel_pos_acc_thresh_mm_em1_ = 0;
    rel_pos_sep_exp_mm_em1_ = 0;
}

UbxIns::UbxIns(uint32_t antenna_separation_mm, uint32_t relative_position_accuracy_threshold_mm)
{
    rel_pos_sep_exp_mm_em1_ = antenna_separation_mm * 10;
    rel_pos_acc_thresh_mm_em1_ = relative_position_accuracy_threshold_mm * 10;
}

void UbxIns::processMessage()
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
            iTOW_ = unpackUint32(0);
            longitude_ = unpackInt32(4);
            latitude_ = unpackInt32(8);
            height_ = unpackInt32(12);
            hMSL_ = unpackInt32(16);
            h_acc_ = unpackInt32(20);
            v_acc_ = unpackInt32(24);
            new_fix_ = true;
            break;
        }
        case 0x07:
        {
            // NAV-PVT
            iTOW_ = unpackUint32(0);
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
            longitude_ = unpackInt32(24);
            latitude_ = unpackInt32(28);
            height_ = unpackInt32(32);
            hMSL_ = unpackInt32(36);
            v_north_ = unpackInt32(48);
            v_east_ = unpackInt32(52);
            v_down_ = unpackInt32(56);
            new_fix_ = true;
            break;
        }

        case 0x3C:
        {
            // NAV-RELPOLNED
            iTOW_rel_ = unpackUint32(4);
            int32_t relPosLength_cm = unpackInt32(20);
            int32_t relPosHeading_deg_em5 = unpackInt32(24);
            char relPosHPLength_mm_em1 = unpackInt8(25);
            uint32_t accLength_mm_em1 = unpackUint32(48);
            uint32_t accHeading_deg_em5 = unpackUint32(52);
            uint32_t flags = unpackUint32(60);
            int32_t rel_pos_length_mm_em1 = relPosLength_cm * 100 + relPosHPLength_mm_em1;
            // Flags expected at bits 0,2,8 => 261
            if ((flags & 261 == 261) && (abs(rel_pos_length_mm_em1 - rel_pos_sep_exp_mm_em1_) < rel_pos_acc_thresh_mm_em1_))
            {
                rel_heading_ = relPosHeading_deg_em5 * (1e-5) * kDeg2Rad;
                new_rel_pos_ = true;
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

uint32_t UbxIns::iTOW() { return iTOW_; }
double UbxIns::timeFix()
{
    uint32_t itow_sec = (uint32_t)(iTOW_ * (1e-3));
    return (double(itow_sec) + (double)time_ns_ * (1e-9));
}
uint32_t UbxIns::nano()
{
    return time_ns_;
}
uint8_t UbxIns::fixType() { return fix_type_; }
int32_t UbxIns::latitude() { return latitude_; }
int32_t UbxIns::longitude() { return longitude_; }
int32_t UbxIns::height() { return height_; }
int32_t UbxIns::hMsl() { return hMSL_; }
int32_t UbxIns::vNorth() { return v_north_; }
int32_t UbxIns::vEast() { return v_east_; }
int32_t UbxIns::vDown() { return v_down_; }
uint32_t UbxIns::hAcc() { return h_acc_;}
uint32_t UbxIns::vAcc() { return v_acc_;}
bool UbxIns::isNewFix() { return new_fix_; }
void UbxIns::clearFix() { new_fix_ = false; }
int32_t UbxIns::dt() { return end_time_ - start_byte_time_; }

// IMU getters
double UbxIns::timeFixImu()
{
    uint32_t itow_sec = (uint32_t)(iTOW_imu_ * (1e-3));
    return (double(itow_sec) + double(time_ns_imu_) * (1e-9));
}

void UbxIns::accel(float data[])
{
    data[0] = accel_[0];
    data[1] = accel_[1];
    data[2] = accel_[2];
}

void UbxIns::gyro(float data[])
{
    data[0] = gyro_[0];
    data[1] = gyro_[1];
    data[2] = gyro_[2];
}
bool UbxIns::isNewImu() { return new_imu_; }
void UbxIns::clearImu() { new_imu_ = false; }

double UbxIns::timeFixRelPos()
{
    return (double(iTOW_rel_) * (1e-3));
}

float UbxIns::relHeading() { return rel_heading_; }
bool UbxIns::isNewRelPos() { return new_rel_pos_; }
void UbxIns::clearRelPos() { new_rel_pos_ = false; }