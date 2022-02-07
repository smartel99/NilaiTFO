/**
 * @file    Tas5707Module.h
 * @author  Samuel Martel
 * @date    2022-02-07
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_TAS5707MODULE_H
#define NILAI_TAS5707MODULE_H

#if defined(NILAI_USE_TAS5707)
#    include "../../defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_I2S_MODULE_ENABLED) && defined(HAL_I2C_MODULE_ENABLED)
#        include "../../defines/module.hpp"
#        include "../../drivers/i2cModule.hpp"
#        include "../../drivers/i2sModule.h"

#include "HardwareConfig.h"
#include "SoftwareConfig.h"

class Tas5707Module : public cep::Module
{
public:
    Tas5707Module(const cep::Tas5707::HardwareConfig& hw, const cep::Tas5707::SoftwareConfig& sw, std::string label);
    ~Tas5707Module() override;

    bool                             DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const override { return m_label; }

private:
    cep::Tas5707::HardwareConfig m_hw = {};
    cep::Tas5707::SoftwareConfig m_sw = {};
    std::string m_label;
};

#    else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO I2S module is enabled, but HAL_I2S_MODULE_ENABLED is undefined!
#        endif
#    endif
#endif



#endif    // NILAI_TAS5707MODULE_H
