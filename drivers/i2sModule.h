/**
 * @file    i2sModule.h
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
#ifndef NILAI_I2SMODULE_H
#define NILAI_I2SMODULE_H

#if defined(NILAI_USE_I2S)
#    include "../defines/internalConfig.h"
#    include NILAI_HAL_HEADER
#    if defined(HAL_I2S_MODULE_ENABLED)
#        include "Core/Inc/i2s.h"
#        include "../defines/module.hpp"

class I2sModule : public cep::Module
{
public:
    I2sModule(I2S_HandleTypeDef* handle, std::string label);
    ~I2sModule() override = default;

    bool               DoPost() override;
    void               Run() override;
    [[nodiscard]] const std::string& GetLabel() const override { return m_label;}

private:
    I2S_HandleTypeDef* m_handle = nullptr;
    std::string m_label;
};

#else
#        if WARN_MISSING_STM_DRIVERS
#            warning NilaiTFO I2S module is enabled, but HAL_I2S_MODULE_ENABLED is undefined!
#        endif
#endif
#endif
#endif    // NILAI_I2SMODULE_H
