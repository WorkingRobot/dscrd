#include "Encoder.h"

namespace rapidetf
{
    Encoder::Encoder(size_t InitialCapacity) :
        Writer(InitialCapacity)
    {
        Writer.WriteVersion();
    }

    std::span<const std::byte> Encoder::Get() const noexcept
    {
        return Writer.Get();
    }

    void Encoder::Reset()
    {
        Writer.Reset();
    }

    template <std::integral T>
    static bool DoesOverlap(auto Value)
    {
        return std::numeric_limits<T>::min() <= Value && Value <= std::numeric_limits<T>::max();
    }

    void Encoder::Write(int8_t Value)
    {
        Writer.WriteInt32(Value);
    }

    void Encoder::Write(uint8_t Value)
    {
        Writer.WriteUInt8(Value);
    }

    void Encoder::Write(int16_t Value)
    {
        if (DoesOverlap<uint8_t>(Value))
        {
            Writer.WriteUInt8(Value);
        }
        else
        {
            Writer.WriteInt32(Value);
        }
    }

    void Encoder::Write(uint16_t Value)
    {
        if (DoesOverlap<uint8_t>(Value))
        {
            Writer.WriteUInt8(Value);
        }
        else
        {
            Writer.WriteInt32(Value);
        }
    }

    void Encoder::Write(int32_t Value)
    {
        if (DoesOverlap<uint8_t>(Value))
        {
            Writer.WriteUInt8(Value);
        }
        else
        {
            Writer.WriteInt32(Value);
        }
    }

    void Encoder::Write(uint32_t Value)
    {
        if (DoesOverlap<uint8_t>(Value))
        {
            Writer.WriteUInt8(Value);
        }
        else if (DoesOverlap<int32_t>(Value))
        {
            Writer.WriteInt32((int32_t)Value);
        }
        else
        {
            Writer.WriteUInt64(Value);
        }
    }

    void Encoder::Write(int64_t Value)
    {
        if (DoesOverlap<uint8_t>(Value))
        {
            Writer.WriteUInt8(Value);
        }
        else if (DoesOverlap<int32_t>(Value))
        {
            Writer.WriteInt32((int32_t)Value);
        }
        else if (DoesOverlap<uint64_t>(Value))
        {
            Writer.WriteUInt64(Value);
        }
        else
        {
            Writer.WriteInt64(Value);
        }
    }

    void Encoder::Write(uint64_t Value)
    {
        if (DoesOverlap<uint8_t>(Value))
        {
            Writer.WriteUInt8(Value);
        }
        else if (DoesOverlap<int32_t>(Value))
        {
            Writer.WriteInt32((int32_t)Value);
        }
        else
        {
            Writer.WriteUInt64(Value);
        }
    }

    void Encoder::Write(bool Value)
    {
        if (Value)
        {
            Writer.WriteTrue();
        }
        else
        {
            Writer.WriteFalse();
        }
    }

    void Encoder::Write(double Value)
    {
        Writer.WriteDouble(Value);
    }

    void Encoder::Write(const std::string& Value)
    {
        Writer.WriteBinary(std::as_bytes(std::span(Value)));
    }

    void Encoder::Write(const std::string_view Value)
    {
        Writer.WriteBinary(std::as_bytes(std::span(Value)));
    }

    void Encoder::Write(std::nullptr_t Ptr)
    {
        Writer.WriteNil();
    }

    void Encoder::Write(std::nullopt_t Opt)
    {
        Writer.WriteNil();
    }
}