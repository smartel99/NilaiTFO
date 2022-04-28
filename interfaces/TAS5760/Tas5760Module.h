/**
 * @file    Tas5760Module.h
 * @author  Samuel Martel
 * @date    2022-03-03
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
#ifndef NILAI_TAS5760_TAS5760MODULE_H
#define NILAI_TAS5760_TAS5760MODULE_H

#include "../../defines/internalConfig.h"
#if defined(NILAI_USE_TAS5760)

#    if !defined(NILAI_USE_I2S)
#        error I2S must be enabled to use the TAS5760 module!
#    endif

#    include "../../drivers/i2sModule.h"
#    include "Enums.h"

class Tas5760Module : public I2sModule
{
public:
    Tas5760Module(I2S_HandleTypeDef* i2s, const std::string& label) : I2sModule(i2s, label) {}

    virtual void               ToggleSleep(bool s) = 0;
    [[nodiscard]] virtual bool IsAsleep() const    = 0;

    virtual void               ToggleShutdown(bool s) = 0;
    [[nodiscard]] virtual bool IsShutdown() const     = 0;
};

#endif

#endif    // NILAI_TAS5760_TAS5760MODULE_H
