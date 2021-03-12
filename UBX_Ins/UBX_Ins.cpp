#include "UBX_Ins.h"

void UBX_Ins::ProcessMessage()
{
    switch (msgid_)
    {
    case 0x07:
    {
        // debug_port.println("NAV-PVT");
        iTOW_ = UnpackUint32(0);
        year_ = UnpackUint16(4);
        month_ = UnpackUint8(6);
        day_ = UnpackUint8(7);
        hour_ = UnpackUint8(8);
        min_ = UnpackUint8(9);
        sec_ = UnpackUint8(10);
        validity_flags_ = UnpackUint8(11);
        time_acc_est_ = UnpackUint32(12);
        time_ns_ = UnpackInt32(16);
        fix_type_ = UnpackUint8(20);
        longitude_ = UnpackInt32(24);
        latitude_ = UnpackInt32(28);
        height_ = UnpackInt32(32);
        hMSL_ = UnpackInt32(36);
        v_north_ = UnpackInt32(48);
        v_east_ = UnpackInt32(52);
        v_down_ = UnpackInt32(56);
        new_fix_ = true;
        // debug_port.println("itow nav: " + String(iTOW_*(1e-3),1));
        break;
    }
    
    case 0x3C:
    {
        // debug_port.println("REL");
        iTOW_rel_ = UnpackUint32(4);
        // long rel_pos_n_cm = UnpackInt32(8);
        // long rel_pos_e_cm = UnpackInt32(12);
        // long rel_pos_d_cm = UnpackInt32(16);
        // signed char rel_pos_n_mm = UnpackInt8(20);
        // signed char rel_pos_e_mm = UnpackInt8(21);
        // signed char rel_pos_d_mm = UnpackInt8(22);
        long relPosLength_cm = UnpackInt32(20);
        long relPosHeading_deg_em5 = UnpackInt32(24);
        char relPosHPLength_mm_em1 = UnpackInt8(25);
        // unsigned long accN_mm_em1 = UnpackUint32(36);
        // unsigned long accE_mm_em1 = UnpackUint32(40);
        unsigned long accLength_mm_em1 = UnpackUint32(48);
        unsigned long accHeading_deg_em5 = UnpackUint32(52);
        unsigned long flags = UnpackUint32(60);
        // float rel_pos_n = (rel_pos_n_cm + rel_pos_n_mm * 0.01F) * 0.01F;
        // float rel_pos_e = (rel_pos_e_cm + rel_pos_e_mm * 0.01F) * 0.01F;
        // float rel_pos_d = (rel_pos_d_cm + rel_pos_d_mm * 0.01F) * 0.01F;
        // debug_port.println("itow rel: " + String(iTOW_rel_*1e-3,1));
        long rel_pos_length_mm_em1 = relPosLength_cm * 100 + relPosHPLength_mm_em1;
        // debug_port.println("len: " + String(rel_pos_length_mm_em1));
        // debug_port.println("flags: " + String(flags,BIN));
        // debug_port.println("accL/accH: " + String(accLength_mm_em1*(1e-4),3) + "/" + String(accHeading_deg_em5*(1e-5),2));
        // Flags expected at bits 0,2,8 => 261
        if ((flags & 261 == 261) && (abs(rel_pos_length_mm_em1 - rel_pos_length_exp_mm_em1_) < 2000))
        {
            rel_heading_ = relPosHeading_deg_em5 * (1e-5) * DEG2RAD;
            new_rel_pos_ = true;
        }
        break;
    }
    default:
        break;
    }
}

unsigned long UBX_Ins::iTOW() { return iTOW_; }
double UBX_Ins::TimeFix()
{
    unsigned long itow_sec = (unsigned long)(iTOW_ * (1e-3));
    // // g fix_us = time_ns_ * (1e-3);
    return (double(itow_sec) + (double)time_ns_ * (1e-9));
}
unsigned long UBX_Ins::Nano()
{
    return time_ns_;
}
unsigned char UBX_Ins::FixType() { return fix_type_; }
long UBX_Ins::Latitude() { return latitude_; }
long UBX_Ins::Longitude() { return longitude_; }
long UBX_Ins::Height() { return height_; }
long UBX_Ins::HMsl() { return hMSL_; }
long UBX_Ins::VNorth() { return v_north_; }
long UBX_Ins::VEast() { return v_east_; }
long UBX_Ins::VDown() { return v_down_; }
bool UBX_Ins::IsNewFix() { return new_fix_; }
void UBX_Ins::ClearFix() { new_fix_ = false; }
long UBX_Ins::Dt() { return end_time_ - start_byte_time_; }

// IMU getters
double UBX_Ins::TimeFixImu()
{
    unsigned long itow_sec = (unsigned long)(iTOW_imu_ * (1e-3));
    // long fix_us = time_ns_imu_ * (1e-3);
    return (double(itow_sec) + double(time_ns_imu_) * (1e-9));
}

void UBX_Ins::Accel(float data[])
{
    data[0] = accel_[0];
    data[1] = accel_[1];
    data[2] = accel_[2];
}

void UBX_Ins::Gyro(float data[])
{
    data[0] = gyro_[0];
    data[1] = gyro_[1];
    data[2] = gyro_[2];
}
bool UBX_Ins::IsNewImu() { return new_imu_; }
void UBX_Ins::ClearImu() { new_imu_ = false; }

// Rel Pos getters
double UBX_Ins::TimeFixRelPos()
{
    return (double(iTOW_rel_) * (1e-3));
}

float UBX_Ins::RelHeading() { return rel_heading_; }
bool UBX_Ins::IsNewRelPos() { return new_rel_pos_; }
void UBX_Ins::ClearRelPos() { new_rel_pos_ = false; }