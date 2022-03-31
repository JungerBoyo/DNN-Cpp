#include "ADVar.hpp"
#include <vector>
#include <fmt/format.h>

int main()
{   
    /// Tangent AD
    AD::Var<float> Tx1(-2.f);
    AD::Var<float> Tx2(-3.f);

    Tx1.primValue = 1.f;
    auto Tyx1 = Tx1 + Tx2;//((Tx1 * Tx2) + Tx2) * Tx1;

    Tx1.primValue = 0.f;
    Tx2.primValue = 1.f;
    auto Tyx2 = Tx1 + Tx2;//((Tx1 * Tx2) + Tx2) * Tx1;

    fmt::print("Tangent AD:: \n x1 = {}\n x2 = {}\n\n", Tyx1.primValue, Tyx2.primValue);

    /// Adjoint AD 
    auto Ax1 = std::make_shared<AD::Var<float>>(-2.f);
    auto Ax2 = std::make_shared<AD::Var<float>>(-3.f);

    auto Ay = ReLU(Ax1 + Ax2);//((Ax1 * Ax2) + Ax2) * Ax1;

    Ay->primValue = 1.f;
    Ay->Evaluate();

    fmt::print("Adjoint AD:: \n x1 = {}\n x2 = {}\n", Ax1->primValue, Ax2->primValue);


    ///
    std::vector<std::shared_ptr<AD::Var<float>>> args{
        std::make_shared<AD::Var<float>>(-0.1f)  ,
        std::make_shared<AD::Var<float>>(5.5f)  ,
        std::make_shared<AD::Var<float>>(1.44f) ,
        std::make_shared<AD::Var<float>>(3.f)   ,
        std::make_shared<AD::Var<float>>(-0.12f) 
    };

    auto y = args[0]*(args[1] + args[2]) + ReLU(2.f*args[3]*args[4]) + args[3] * args[3] * args[3] + 2.f*args[0]*args[0];

    y->primValue = 1.f;
    y->Evaluate();

    for(const auto arg : args)
    {
        fmt::print("{}\n", arg->primValue);
    }
}