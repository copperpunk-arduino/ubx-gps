#include "UbxParser.h"

class UbxIns : public UbxParser
{
public:
    UbxIns();
    UbxIns(uint32_t antenna_separation_mm, uint32_t relative_position_accuracy_threshold_mm);
    void processMessage();
    uint32_t iTOW();
    double timeFix();
    uint32_t nano();
    uint8_t fixType();
    int32_t latitude();
    int32_t longitude();
    int32_t height();
    int32_t hMsl();
    int32_t vNorth();
    int32_t vEast();
    int32_t vDown();
    uint32_t hAcc();
    uint32_t vAcc();
    bool isNewFix();
    void clearFix();
    int32_t dt();

    // IMU getters
    double timeFixImu();
    void accel(float data[]);
    void gyro(float data[]);
    bool isNewImu();
    void clearImu();

    double timeFixRelPos();
    float relHeading();
    bool isNewRelPos();
    void clearRelPos();

private:
    const float kDeg2Rad = 0.0174532925f;

    uint32_t iTOW_;
    uint16_t year_;
    uint8_t month_;
    uint8_t day_;
    uint8_t hour_;
    uint8_t min_;
    uint8_t sec_;
    uint8_t validity_flags_;
    uint32_t time_acc_est_;
    int32_t time_ns_;
    uint8_t fix_type_ = 0;
    uint8_t num_sv_ = 0;
    int32_t longitude_;
    int32_t latitude_;
    int32_t height_;
    int32_t hMSL_;
    int32_t v_north_;
    int32_t v_east_;
    int32_t v_down_;
    int32_t h_acc_;
    int32_t v_acc_;
    int32_t start_time_;
    int32_t start_byte_time_;
    int32_t end_time_;
    bool new_fix_ = false;

    // IMU data fields
    uint32_t iTOW_imu_;
    int32_t time_ns_imu_;
    float accel_[3];
    float gyro_[3];
    bool new_imu_ = false;

    // RelPosNED data fields
    uint32_t rel_pos_sep_exp_mm_em1_; //[millimeters * 1e-1]
    uint32_t rel_pos_acc_thresh_mm_em1_; //[millimeters * 1e-1]
    uint32_t iTOW_rel_;
    float rel_heading_ = 0.0F;
    bool new_rel_pos_ = false;
};
