#ifndef AD_ADVAR_HPP
#define AD_ADVAR_HPP

#include <memory>

namespace AD
{
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct Var
    {
        Var() = default;
        Var(T value) 
            : value(value) {}

        T value     { static_cast<T>(0) };
        T primValue { static_cast<T>(0) };

        virtual void Evaluate(){}
    };

    namespace Op
    {
        template<typename T>
        struct Add : public Var<T>
        {   
            Add(std::shared_ptr<Var<T>> lhsArg, std::shared_ptr<Var<T>> rhsArg)
                : _lhsArg(std::move(lhsArg)), _rhsArg(std::move(rhsArg))
            {
                this->value = _lhsArg->value + _rhsArg->value;
            }

            void Evaluate() override
            {
                _rhsArg->primValue += this->primValue; // * 1
                _lhsArg->primValue += this->primValue; // * 1

                _rhsArg->Evaluate();
                _lhsArg->Evaluate();
            }

            private:
                std::shared_ptr<Var<T>> _lhsArg { nullptr };
                std::shared_ptr<Var<T>> _rhsArg { nullptr };
        };

        template<typename T>
        struct Mul : public Var<T>
        {
            Mul(std::shared_ptr<Var<T>> lhsArg, std::shared_ptr<Var<T>> rhsArg)
                : _lhsArg(std::move(lhsArg)), _rhsArg(std::move(rhsArg))
            {
                this->value = _lhsArg->value * _rhsArg->value;
            }

            void Evaluate() override
            {
                _rhsArg->primValue += this->primValue * _lhsArg->value;
                _lhsArg->primValue += this->primValue * _rhsArg->value;

                _rhsArg->Evaluate();
                _lhsArg->Evaluate();
            }

            private:
                std::shared_ptr<Var<T>> _lhsArg { nullptr };
                std::shared_ptr<Var<T>> _rhsArg { nullptr };
        };

        template<typename T>
        struct ReLU : public Var<T>
        {
            ReLU(std::shared_ptr<Var<T>> arg)
                : _rectified(arg->value <= static_cast<T>(0)),
                  _arg(std::move(arg))                
            {
                this->value = _rectified ? static_cast<T>(0) : _arg->value;
            }

            void Evaluate() override
            {
                if(!_rectified)
                    _arg->primValue += this->primValue; // * 1

                _arg->Evaluate();
            }   

            private:
                bool _rectified;
                std::shared_ptr<Var<T>> _arg { nullptr };
        };
    }

/**
 * operators for adjoint
 */

    template<typename T>
    std::shared_ptr<Var<T>> operator*(std::shared_ptr<Var<T>> lhs, std::shared_ptr<Var<T>> rhs)
    {
        return std::make_shared<Op::Mul<T>>(lhs, rhs);
    }  

    template<typename T>
    std::shared_ptr<Var<T>> operator+(std::shared_ptr<Var<T>> lhs, std::shared_ptr<Var<T>> rhs)
    {
        return std::make_shared<Op::Add<T>>(lhs, rhs);
    }  

    template<typename T>
    std::shared_ptr<Var<T>> operator*(T lhs, std::shared_ptr<Var<T>> rhs)
    {
        return std::make_shared<Op::Mul<T>>(std::make_shared<Var<T>>(lhs), rhs);
    }  

    template<typename T>
    std::shared_ptr<Var<T>> operator+(std::shared_ptr<Var<T>> lhs, T rhs)
    {
        return std::make_shared<Op::Add<T>>(lhs, std::make_shared<Var<T>>(rhs));
    }  

    template<typename T>
    std::shared_ptr<Var<T>> ReLU(Var<T> arg)
    {
        return std::make_shared<Op::ReLU<T>>(std::make_shared<Var<T>>(arg));
    }

    template<typename T>
    std::shared_ptr<Var<T>> ReLU(std::shared_ptr<Var<T>> arg)
    {
        return std::make_shared<Op::ReLU<T>>(arg);
    }

/**
 * operators for tangent
 */
    template<typename T>
    constexpr Var<T> operator*(Var<T> lhs, Var<T> rhs)
    {
        Var<T> out;
        out.value = lhs.value * rhs.value;
        out.primValue = lhs.primValue * rhs.value + rhs.primValue * lhs.value;

        return out;
    }  

    template<typename T>
    constexpr Var<T> operator+(Var<T> lhs, Var<T> rhs)
    {
        Var<T> out;
        out.value = lhs.value + rhs.value;
        out.primValue = lhs.primValue + rhs.primValue;

        return out;
    }  

    template<typename T>
    constexpr Var<T> operator*(T lhs, Var<T> rhs)
    {
        Var<T> out;
        out.value = lhs * rhs.value;
        out.primValue = rhs.primValue * lhs.value;

        return out;
    }  

    template<typename T>
    constexpr Var<T> operator+(Var<T> lhs, T rhs)
    {
        Var<T> out;
        out.value = lhs + rhs.value;
        out.primValue = rhs.primValue;

        return out;
    }  
}

#endif