#ifndef _MAGNETOMETER_H
#define _MAGNETOMETER_H
#include "System/RunableModule.h"

#define MAG_FILTER_SIZE (5)

class Magnetometer: public RunableModule {
  private:
    /***********************************/
    /* Private Enumerations            */
    /***********************************/
    /* Magnetometer Register Enumeration */
    enum magReg {
      MR_CFG_REG_A,  //Configuration Register A (R/W)
      MR_CFG_REG_B,  //Configuration Register B (R/W)
      MR_MODE_REG,   //Mode Register (R/W)
      MR_DATA_START_REG,  //Start of X,Y,Z Output
      MR_X_MSB_REG = MR_DATA_START_REG,  //Data Out X LSB (R)
      MR_X_LSB_REG,  //Data Out X MSB (R)
      MR_Z_MSB_REG,  //Data Out Z LSB (R)
      MR_Z_LSB_REG,  //Data Out Z MSB (R)
      MR_Y_MSB_REG,  //Data Out Y LSB (R)
      MR_Y_LSB_REG,  //Data Out Y MSB (R)
      MR_DATA_END_REG = MR_Y_LSB_REG, //End of X,Y,Z Output
      MR_STATUS_REG, //Status Register (R)
      MR_ID_REG_A,   //Identification Register A (R)
      MR_ID_REG_B,   //Identification Register B (R)
      MR_ID_REG_C,   //Identification Register C (R)
      NUM_MAG_REG
    };

    /* Configuration Register A Enumerations */
    enum dataRate {
      DR_0_75_HZ,     // 0.75   Hz
      DR_1_5_HZ,      // 1.5    Hz
      DR_3_HZ,        // 3      Hz
      DR_7_5_HZ,      // 7.5    Hz
      DR_15_HZ,       // 15     Hz (Default)
      DR_30_HZ,       // 30     Hz
      DR_75_HZ,       // 75     Hz
      DR_RESERVED,    // Reserved by HW
      NUM_DATA_RATES,
    };

    enum samplesAveraged {
      SA_1,  // 1 Samples Averaged (Default)
      SA_2,  // 2 Samples Averaged
      SA_4,  // 4 Samples Averaged
      SA_8,  // 8 Samples Averaged
      NUM_SA_OPTIONS,
    };

    /* Configuration Register B Enumerations */
    enum gains{
      G_0_88_GA, // 0.88 Ga, 0.73 mG/LSB
      G_1_3_GA,  // 1.3  Ga, 0.92 mG/LSB (Default)
      G_1_9_GA,  // 1.9  Ga, 1.22 mG/LSB
      G_2_5_GA,  // 2.5  Ga, 1.52 mG/LSB
      G_4_0_GA,  // 4.0  Ga, 2.27 mG/LSB
      G_4_7_GA,  // 4.7  Ga, 2.56 mG/LSB
      G_5_6_GA,  // 5.6  Ga, 3.03 mG/LSB
      G_8_1_GA,  // 8.1  Ga, 4.35 mG/LSB
      NUM_GAIN_OPTIONS,
    };

    /* Mode Register Enumerations */
    enum magModes {
      MM_CMM,       //Continuous Measurment Mode
      MM_SMM,       //Single Measurement Mode
      MM_CMM_IDLE,  //Continuous Measurement Mode and Idle
      MM_SMM_IDLE,  //Single Measurement Mode and Idle
      NUM_MODES,
    };
    /***********************************/
      /* End of Private Enumerations     */
    /***********************************/
    /***********************************/
    /* Private Type Definitions        */
    /***********************************/

    /* Register 0x00 - Configuration Register A */
    typedef struct ConfigA {
      union {
      struct {
        uint8_t meas_cfg    :2; //Measurement Configuration Bits (0-1)
        uint8_t data_rate   :3; //Data Output Rate Bits          (2-4)
        uint8_t avg         :2; //Num Sample Averaged            (5-6)
        uint8_t reserved    :1; //Reserved by HW for future use  (7)
      };
      uint8_t all;
      };
    } ConfigA;

    /* Register 0x01 - Configuration Register B */
    typedef struct ConfigB {
      union {
      struct {
        uint8_t zero   :5; //These bits must be zeroized    (0-4)
        uint8_t gain   :3; //Gain Cfg Bits (digital Resolution) (5-7)
      };
      uint8_t all;
      };
    } ConfigB;

    /* Register 0x02 - Mode Register */
    typedef struct ModeReg {
      union {
      struct {
        uint8_t mode_sel    :2; //Mode Selection Bits
        uint8_t reserved    :5; //These bits are reserved by HW
        uint8_t high_speed  :1; //High Speed I2C (3400 kHz)
      };
      uint8_t all;
      };
    } ModeReg;
    /***********************************/
    /* End of Private Type Definitions */
    /***********************************/

    /***********************************/
    /* Private Data Members            */
    /***********************************/
    uint8_t	   addr;
    int16_t    data_x;
    int16_t    data_y;
    int16_t    data_z;
    double     filter[MAG_FILTER_SIZE];
    uint8_t    index;
    float   rawHeading;
    float   filteredHeading;

    /* Register Structure */
    union {
      struct {
        ConfigA  cfg_A;
        ConfigB  cfg_B;
        ModeReg  mode;
        int8_t   x_msb;
        uint8_t  x_lsb;
        int8_t   z_msb;
        uint8_t  z_lsb;
        int8_t   y_msb;
        uint8_t  y_lsb;
        uint8_t  stat;
        uint8_t  id_reg_A;
        uint8_t  id_reg_B;
        uint8_t	 id_reg_C;
      };
      uint8_t data[NUM_MAG_REG];
    };

    /***********************************/
    /* End of Private Data Members     */
    /***********************************/

    /***********************************/
    /* End of Private Functions        */
    /***********************************/
    void readMagnetometer(uint8_t,uint8_t);
    void updateXYZ(void);
    void sendByte(uint8_t, uint8_t);
    /***********************************/
    /* End of Private Functions        */
    /***********************************/

  public:
    Magnetometer(uint8_t);
    void init(void);
    uint16_t calcHeading(void);
    float getRawHead(void);
    float getFiltHead(void);
    bool RunTick();
    void DebugOutput(HardwareSerial *serialPort);
};

#endif
