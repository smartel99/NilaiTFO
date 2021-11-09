/**
 ******************************************************************************
 * @addtogroup max14763Module
 * @{
 * @file    max14763Module
 * @author  Lou-Gabriel Gaucher
 * @brief   Header for the max14763Module module.
 *
 * @date 9/23/2020 3:03:33 PM
 *
 ******************************************************************************
 */

#ifndef _max14763Module
#    define _max14763Module
#    if defined(NILAI_USE_MAX14763)
/*****************************************************************************/
/* Includes */
#        include <functional>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */

class Max14763Module
{
public:
    Max14763Module() = default;
    Max14763Module(const std::function<void(bool)>& setFunc) : m_setFunc(setFunc) {}

    void SetFunc(const std::function<void(bool)>& func) { m_setFunc = func; }

    void Set()
    {
        if (m_setFunc)
        {
            m_setFunc(true);
        }
    }

    void Reset()
    {
        if (m_setFunc)
        {
            m_setFunc(false);
        }
    }

private:
    std::function<void(bool)> m_setFunc;
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#    endif /* _max14763Module */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
