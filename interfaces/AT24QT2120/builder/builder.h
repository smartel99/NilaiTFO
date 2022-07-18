/**
 * @file    builder.h
 * @author  Samuel Martel
 * @date    2022-07-12
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

#ifndef GUARD_AT24QT2120_BUILDER_H
#define GUARD_AT24QT2120_BUILDER_H

#if defined(NILAI_USE_AT24QT2120)

#    include "../../../defines/pin.h"
#    include "../../../drivers/i2c_module.h"

#    include "../registers/registers.h"

#    include "vendor/NilaiTFO/interfaces/AT24QT2120/default_values.h"

#    include "vendor/NilaiTFO/interfaces/AT24QT2120/at24qt2120.h"

namespace Nilai::Interfaces
{
class At24Qt2120;

}    // namespace Nilai::Interfaces

#endif

#endif    // GUARD_AT24QT2120_BUILDER_H
