/**
 * @file    sw_tas5760_impl.cpp
 * @author  Samuel Martel
 * @date    2022-05-04
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */

#if __INCLUDE_LEVEL__
#    error "Do not include this file!"
#endif
#if defined(NILAI_USE_SW_TAS5760) && !defined(NILAI_TEST)
// Include the cpp file on purpose.
#    include "sw_tas5760.cpp"

#    if defined(NILAI_USE_I2S)
#        include "../../drivers/i2s_module.h"
#    endif
#    if defined(NILAI_USE_SAI)
#        include "../../drivers/sai_module.h"
#    endif

namespace Nilai::Interfaces
{
#    if defined(NILAI_USE_I2S)
template class SwTas5760<Drivers::I2sModule>;

#    endif
#    if defined(NILAI_USE_SAI)
template class SwTas5760<Drivers::SaiModule>;
#    endif
}    // namespace Nilai::Interfaces
#endif
