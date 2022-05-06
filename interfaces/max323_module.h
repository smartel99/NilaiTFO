/**
 * @addtogroup max323Module.h
 * @{
 *******************************************************************************
 * @file	max323Module.h
 * @author	Samuel Martel
 * @brief
 * Created on: Oct. 19, 2020
 *******************************************************************************
 */

#ifndef MAX323MODULE_H_
#define MAX323MODULE_H_

#if defined(NILAI_USE_MAX323)
/***********************************************/
/* Includes */

#    include <functional>

/***********************************************/
/* Defines */
namespace Nilai
{
class MAX323
{
public:
    MAX323() = default;
    MAX323(const std::function<void(bool)>& set1, const std::function<void(bool)>& set2)
    : m_set1(set1), m_set2(set2)
    {
    }

    void SetSet1Func(const std::function<void(bool)>& func) { m_set1 = func; }
    void SetSet2Func(const std::function<void(bool)>& func) { m_set2 = func; }

    void Set1(bool state)
    {
        if (m_set1)
        {
            m_set1(state);
        }
    }
    void Set2(bool state)
    {
        if (m_set2)
        {
            m_set2(state);
        }
    }

private:
    std::function<void(bool)> m_set1 = {};
    std::function<void(bool)> m_set2 = {};
};
}    // namespace Nilai

/***********************************************/
/* Function declarations */

#endif
/**
 * @}
 */
/* END OF FILE */
#endif /* MAX323MODULE_H_ */
