#pragma once
#include "base.hpp"

#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)

class UartModuleIt : public UartModule {
  public:
    UartModuleIt(UART_HandleTypeDef* uart, const std::string& label, size_t rxLen = 512);

    void Run() override;

    void SetExpectedRxLen(size_t len) override;
    void ClearExpectedRxLen() override;

    using UartModule::SetStartOfFrameSequence;
    void SetStartOfFrameSequence(const std::string& sof) override;
    using UartModule::SetEndOfFrameSequence;
    void SetEndOfFrameSequence(const std::string& eof) override;
    void ClearEndOfFrameSequence() override;
    void HandleReceptionIRQ();
};

#endif
