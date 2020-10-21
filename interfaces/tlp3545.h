/**
 * @addtogroup tlp3545.h
 * @{
 *******************************************************************************
 * @file	tlp3545.h
 * @author	Samuel Martel
 * @brief
 * Created on: Oct. 20, 2020
 *******************************************************************************
 */

#ifndef TLP3545_H_
#define TLP3545_H_

#if defined(NILAI_USE_TLP3545)
/***********************************************/
/* Includes */
#    include <functional>

/***********************************************/
/* Defines */
class TLP3545
{
public:
    TLP3545( ) = default;
    TLP3545(const std::function<void(bool)>& func) : m_ctrlFunc(func) {}

    void SetCtrlFunc(const std::function<void(bool)>& func) { m_ctrlFunc = func; }

    void Set( ) const
    {
        if (m_ctrlFunc)
        {
            m_ctrlFunc(true);
        }
    }

    void Reset( ) const
    {
        if (m_ctrlFunc)
        {
            m_ctrlFunc(false);
        }
    }

private:
    std::function<void(bool)> m_ctrlFunc = {};
};

/***********************************************/
/* Function declarations */

#endif
/**
 * @}
 */
/* END OF FILE */
#endif /* TLP3545_H_ */
