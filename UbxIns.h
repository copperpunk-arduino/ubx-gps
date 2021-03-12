#include "UbxParser.h"

class UbxIns : public UbxParser
{
public:
    void ProcessMessage();
    unsigned long iTOW();
    double TimeFix();
    unsigned long Nano();
    unsigned char FixType();
    long Latitude();
    long Longitude();
    long Height();
    long HMsl();
    long VNorth();
    long VEast();
    long VDown();
    bool IsNewFix();
    void ClearFix();
    long Dt();

    // IMU getters
    double TimeFixImu();
    void Accel(float data[]);
    void Gyro(float data[]);
    bool IsNewImu();
    void ClearImu();

    double TimeFixRelPos();
    // float RelPosN();
    // float RelPosE();
    float RelHeading();
    bool IsNewRelPos();
    void ClearRelPos();

private:
    unsigned long iTOW_;
    unsigned short year_;
    unsigned char month_;
    unsigned char day_;
    unsigned char hour_;
    unsigned char min_;
    unsigned char sec_;
    unsigned char validity_flags_;
    unsigned long time_acc_est_;
    long time_ns_;
    unsigned char fix_type_ = 0;
    unsigned char num_sv_ = 0;
    long longitude_;
    long latitude_;
    long height_;
    long hMSL_;
    long v_north_;
    long v_east_;
    long v_down_;
    long start_time_;
    long start_byte_time_;
    long end_time_;
    bool new_fix_ = false;

    // IMU data fields
    unsigned long iTOW_imu_;
    long time_ns_imu_;
    float accel_[3];
    float gyro_[3];
    bool new_imu_ = false;

    // RelPosNED data fields
    // unsigned long rel_pos_length_exp_mm_em1_ = 13653;
    unsigned long rel_pos_length_exp_mm_em1_ = 18225;
    unsigned long iTOW_rel_;
    // float rel_pos_n_ = 0.0F;
    // float rel_pos_e_ = 0.0F;
    // float rel_pos_d_ = 0.0F;
    float rel_heading_ = 0.0F;
    bool new_rel_pos_ = false;
};
