#include "UbxParser.h"
#include <cstdlib>

class UbxGps : public UbxParser
{
public:
    UbxGps();
    // UbxGps(uint32_t antenna_separation_mm, uint32_t relative_position_accuracy_threshold_mm);
    void processMessage();
    uint32_t iTowMs();
    double timeFixSec();
    uint32_t nano();
    uint8_t fixType();
    int32_t latitudeDeg();
    int32_t longitudeDeg();
    int32_t heightMm();
    int32_t hMslMm();
    int32_t vNorthMmps();
    int32_t vEastMmps();
    int32_t vDownMmps();
    uint32_t hAccMm();
    uint32_t vAccMm();
    bool isNewFix();
    void clearFix();

    double timeFixRelHdgSec();
    float relHeadingDeg();
    float relPositionDistanceMm();
    bool isNewRelHdg();
    void clearRelHdg();

private:
    const float kDeg2Rad = 0.0174532925f;

    uint32_t itow_ms_;
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
    int32_t longitude_deg_;
    int32_t latitude_deg_;
    int32_t height_mm_;
    int32_t h_msl_mm_;
    int32_t v_north_mmps_;
    int32_t v_east_mmps_;
    int32_t v_down_mmps_;
    int32_t h_acc_mm_;
    int32_t v_acc_mm_;
    bool new_fix_ = false;

    // RelPosNED data fields
    uint32_t rel_pos_sep_exp_mm_em1_; //[millimeters * 1e-1]
    uint32_t rel_pos_acc_thresh_mm_em1_; //[millimeters * 1e-1]
    uint32_t itow_rel_ms_;
    float rel_pos_distance_mm_ = 0.f;
    float rel_heading_deg_ = 0.f;
    bool new_rel_hdg_ = false;
};
