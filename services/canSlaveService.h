/**
 ******************************************************************************
 * @addtogroup canSlaveService
 * @{
 * @file    canSlaveService
 * @author  Samuel Martel
 * @brief   Header for the canSlaveService module.
 *
 * @date 9/17/2020 8:37:46 AM
 *
 ******************************************************************************
 */

/**
 * DEFINITION:
 * * Universe: A collection of [TBD] 8-bit channels.
 *
 * STEPS:
 * - MASTER board sends one or more channels
 * - Slave checks crc
 * - If crc is good:
 *  - Copies channels into local universe
 *  - Set new Universe Ready flag ??????
 *  - Waits a frame for modules to process it
 *      - Modules should check the Universe Ready flag during each frames
 *          - If it is set, the module should read all of its channel
 *            and re-set all of its values in the Universe
 *  - Clear new Universe Ready flag
 *  - Sends the Universe to the PC
 * - If CRC is bad:
 *  - Ignore Universe
 */
#ifndef _canSlaveModule
#    define _canSlaveModule

/*****************************************************************************/
/* Includes */
#    include "defines/module.hpp"

#    include "drivers/canModule.hpp"

#    include <array>
#    include <string>
#    include <vector>

/*****************************************************************************/
/* Exported defines */
#define SLAVE_CAN_ID 0
#define FIRST_CHANNEL_DRIVE_MASTER 0
#define MAX_CHANNELS_DRIVE_MASTER 25
// CHANNELS
// VARIABLE DC VOLTAGE
#define START_DC_POWER_1 0
#define START_DC_POWER_2 1
#define SET_DC_POWER_1_MSB 2
#define SET_DC_POWER_1_LSB 3
#define SET_DC_POWER_2_MSB 4
#define SET_DC_POWER_2_LSB 5
// RESISTOR LOAD BANK
#define SET_TRANS_POWER_BANK 6
#define SET_VBAT_BANK 7
#define SET_DIESEL_LOAD 8
// HIGH-VOLTAGE AC
#define SET_HVAC_SUPPLY 8 // ENABLE
#define SET_HVAC_VOLT_MSB 9
#define SET_HVAC_VOLT_LSB 10
#define SET_HVAC_FREQ 11
#define GET_HVAC_MA_MSB 12 
#define GET_HVAC_MA_LSB 13 
#define GET_HVAC_STATUS 14
// 30VAC VOLTAGE
#define SET_30VAC_SUPPLY 15 //ENABLE
#define SET_30VAC_MUX 16
#define SET_30VAC_VOLT_MSB 17
#define SET_30VAC_VOLT_LSB 18
#define SET_30VAC_FREQ 19
#define GET_30VAC_MA_MSB 20 
#define GET_30VAC_MA_LSB 21 
#define GET_30VAC_STATUS 22
// BUZZER
#define SET_BUZZER 23
//FAN
#define SET_FAN 24
// TEMPERATURE
#define GET_DRIVE_TEMP 25

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */

class CanSlaveModule : public cep::Module
{
public:
	CanSlaveModule(CanModule* can2, const std::string& label);
	virtual ~CanSlaveModule() override = default;

	virtual void Run() override;
	virtual const std::string& GetLabel() const override { return m_label; }

private:
	CanModule* m_can2  = nullptr;
	std::string m_label = "";

};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _canSlaveModule */
/**
 * @}
 */
/****** END OF FILE ******/
