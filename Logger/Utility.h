#pragma once

template<typename OutStream, typename T>
class FormatOutput final
{
private:
    const T& ref_;

public:
    FormatOutput(const T& obj) : ref_(obj) {}

    friend OutStream& operator<<(OutStream& os, const FormatOutput<OutStream, T>& arg)
    {
        os << arg.ref_ << ' ';
        return os;
    }
};
