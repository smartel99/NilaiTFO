#include "processes/application.h"

class TestApp : public cep::Application
{
public:
    void Init() override {}
    bool DoPost() override { return true; }
    void Run() {}
};