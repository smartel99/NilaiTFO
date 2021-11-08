#pragma once
#include "base.hpp"

#if defined(NILAI_USE_UART) && defined(HAL_UART_MODULE_ENABLED)

class UartModuleDma : public UartModule {
  public:
    UartModuleDma(UART_HandleTypeDef* uart, const std::string& label, size_t rxLen = 1024);

    void Run() override;

    void SetExpectedRxLen(size_t len) override;
    void ClearExpectedRxLen() override;
    void SetStartOfFrameSequence(const std::string& sof) override;
    void SetEndOfFrameSequence(const std::string& eof) override;
    void ClearEndOfFrameSequence() override;

  private:
    bool ResizeDmaBuffer(size_t sofLen, size_t len, size_t eofLen);
};

#endif
