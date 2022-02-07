/**
 * @file    Tas5707Module.cpp
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
#include "Tas5707Module.h"

#include "Enums.h"

Tas5707Module::Tas5707Module(const cep::Tas5707::HardwareConfig& hw,
                             const cep::Tas5707::SoftwareConfig& sw,
                             std::string                         label)
: m_hw(hw), m_sw(sw), m_label(std::move(label))
{

}

Tas5707Module::~Tas5707Module()
{
}

bool Tas5707Module::DoPost()
{
    return false;
}

void Tas5707Module::Run()
{
}
