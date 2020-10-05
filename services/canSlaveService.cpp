/**
 ******************************************************************************
 * @addtogroup canSlaveService
 * @{
 * @file    canSlaveService
 * @author  Lou-Gabriel Gaucher
 * @brief   Sources for the canSlaveService module.
 *
 * @date 9/17/2020 8:37:46 AM
 *
 ******************************************************************************
 */
#include "canSlaveService.h"
#include "processes/MasterApplication.h"
#include "shared/services/umoModule.h"
#include "defines/macros.hpp"

CanSlaveModule::CanSlaveModule(CanModule *can2, const std::string &label) :
        m_can2(can2), m_label(label)
{
    // Ensure that the pointer is valid.
    CEP_ASSERT(can2 != nullptr, "In canSlaveModule: Can2 is NULL!");
}

void CanSlaveModule::Run()
{
    // flag if universe was changed FLAG SET IN UNIVERSE MODULE
    if (UMO_MODULE->IsUniverseReady(0) == true)
    {
        //Instantiate vectors containing the data
        std::vector<uint8_t> dataToDrive = std::vector<uint8_t>(2);
        std::vector<uint8_t> dataToUniverse = std::vector<uint8_t>(25);

        for (uint8_t i = 0; i <= MAX_CHANNELS_DRIVE_MASTER; i++)
        {
            //Takes data from Universe
            std::vector<uint8_t> dataFromUniverse = UMO_MODULE->GetChannels(0, i, 1);
            // Put channel ID in frame
            dataToDrive[0] = i;
            // Put channel data in frame
            dataToDrive[1] = dataFromUniverse[0];
            //Sends Channel to Drive
            CAN2_MODULE->TransmitFrame(SLAVE_CAN_ID, dataToDrive);
            HAL_Delay(1);
            // Wait for CAN reception
            while(CAN2_MODULE->GetNumberOfAvailableFrames() > 0)
            {
                CAN::Frame frame = CAN2_MODULE->ReceiveFrame();
                std::array<uint8_t, 8> dataFromDrive = frame.data;
                //Is channel received the same as channel sent?
                if(i == dataFromDrive[0])
                {
                    // CAN Exchange successfull!
                    //Parse CAN data to set into universe
                    dataToUniverse[i] = dataFromDrive[1];
                }
                else
                {
                    //Bad can transmission!
                    //Re-Sends Channel to Drive until OK
                    CAN2_MODULE->TransmitFrame(SLAVE_CAN_ID, dataToDrive);
                    HAL_Delay(1);
                }
            }
        }
        // Write channels received in universe
        UMO_MODULE->SetChannels(0, FIRST_CHANNEL_DRIVE_MASTER, dataToUniverse);
    }
}

