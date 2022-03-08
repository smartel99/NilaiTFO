#include "processes/application.hpp"

class TestApp : public cep::Application
{
public:
    void Init() override {}
    bool DoPost() override { return true; }
    void Run() {}
};