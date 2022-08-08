/**
 * @addtogroup Nilai
 * @{
 *******************************************************************************
 * @file	Nilai.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr. 21, 2021
 *******************************************************************************
 */

#ifndef NILAI_CORE_H_
#define NILAI_CORE_H_

#include "defines/assertion.h"
#include "defines/compiler_defines.h"
#include "defines/globaldef.h"
#include "defines/internal_config.h"
#include "defines/led_pattern.h"
#include "defines/macros.h"
#include "defines/misc.h"
#include "defines/module.h"
#include "defines/smart_pointers.h"
#include "defines/type_templates.h"
#include "defines/version.h"

#include "drivers/adc_module.h"
#include "drivers/audio_device.h"
#include "drivers/can_module.h"
#include "drivers/i2c_module.h"
#include "drivers/i2s_module.h"
#include "drivers/pwm_module.h"
#include "drivers/rtc_module.h"
#include "drivers/sai_module.h"
#include "drivers/spi_module.h"
#include "drivers/uart_module.h"

#include "interfaces/ADS131/ads131_module.h"
#include "interfaces/heartbeat_module.h"
#include "interfaces/ltc2498_module.h"
#include "interfaces/max14763_module.h"
#include "interfaces/max14778_module.h"
#include "interfaces/max323_module.h"
#include "interfaces/pca9505_module.h"
#include "interfaces/relay_module.h"
#include "interfaces/TAS5707/tas5707_module.h"
#include "interfaces/TAS5760/sw_tas5760.h"
#include "interfaces/TAS5760/tas5760_module.h"
#include "interfaces/tlp3545.h"

#include "processes/application.h"

#include "services/file.h"
#include "services/file_logger_module.h"
#include "services/filesystem.h"
#include "services/ini_parser.h"
#include "services/logger.h"
#include "services/system_module.h"
#include "services/time.h"
#include "services/umo_module.h"


/**
 * @}
 */
/* END OF FILE */
#endif /* NILAI_CORE_H_ */
