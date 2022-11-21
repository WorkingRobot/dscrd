#pragma once

#include <bit>
#include <cstdint>

namespace rapidetf
{
    enum class Constants : uint8_t
    {
        Version = 131,
        NewFloat = 'F',
        SmallInteger = 'a',
        Integer = 'b',
        Atom = 'd',
        SmallTuple = 'h',
        LargeTuple = 'i',
        Nil = 'j',
        String = 'k',
        List = 'l',
        Binary = 'm',
        SmallBig = 'n',
        SmallAtom = 's',
        Map = 't',
        AtomUtf8 = 'v',
        SmallAtomUtf8 = 'w',
    };

    template <std::integral T>
    T AsBigEndian(T Value) noexcept
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            return std::byteswap(Value);
        }
        return Value;
    }
}