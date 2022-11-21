#include "Writer.h"

#include <stdexcept>

namespace rapidetf
{
    Writer::Writer(size_t InitialCapacity)
    {
        Buffer.reserve(InitialCapacity);
    }

    std::span<const std::byte> Writer::Get() const noexcept
    {
        return Buffer;
    }

    void Writer::Reset()
    {
        Buffer.clear();
    }

    void Writer::WriteVersion()
    {
        Write(Constants::Version);
    }

    void Writer::WriteNil()
    {
        static constexpr std::array<char, 5> Data { (char)Constants::SmallAtom, 3, 'n', 'i', 'l' };
        static constexpr auto Span = std::span { Data };
        Write(Span);
    }

    void Writer::WriteFalse()
    {
        static constexpr std::array<char, 7> Data { (char)Constants::SmallAtom, 5, 'f', 'a', 'l', 's', 'e' };
        static constexpr auto Span = std::span { Data };
        Write(Span);
    }

    void Writer::WriteTrue()
    {
        static constexpr std::array<char, 6> Data { (char)Constants::SmallAtom, 4, 't', 'r', 'u', 'e' };
        static constexpr auto Span = std::span { Data };
        Write(Span);
    }

    void Writer::WriteUInt8(uint8_t Value)
    {
        Write(Constants::SmallInteger);
        Write(Value);
    }

    void Writer::WriteInt32(int32_t Value)
    {
        Write(Constants::Integer);
        Write(AsBigEndian(Value));
    }

    static uint8_t GetByteCount(uint64_t Value)
    {
        static constexpr uint8_t AlignAmt = std::numeric_limits<uint8_t>::digits - 1;

        return uint8_t(std::bit_width(Value) + AlignAmt) >> 3;
    }

    void Writer::WriteUInt64(uint64_t Value)
    {
        uint8_t ByteCount = GetByteCount(Value);
        Value = AsBigEndian(Value);

        Write(Constants::SmallBig);
        Write(ByteCount);
        Write((uint8_t)0); // uint64_t is always unsigned
        Write(std::as_bytes(std::span(&Value, 1)).subspan(0, ByteCount));
    }

    void Writer::WriteInt64(int64_t Value)
    {
        uint64_t AbsValue = std::abs(Value);

        uint8_t ByteCount = GetByteCount(AbsValue);
        AbsValue = AsBigEndian(AbsValue);

        Write(Constants::SmallBig);
        Write(ByteCount);
        Write((uint8_t)std::signbit(Value));
        Write(std::as_bytes(std::span(&AbsValue, 1)).subspan(0, ByteCount));
    }

    void Writer::WriteDouble(double Value)
    {
        Write(Constants::NewFloat);
        Write(AsBigEndian(std::bit_cast<uint64_t>(Value)));
    }

    static constexpr auto MaxSmallAtomSize = 0xFF;
    static constexpr auto MaxAtomSize = 0xFFFF;
    static constexpr auto MaxStringSize = 0xFFFF;
    static constexpr auto MaxSmallTupleSize = 0xFF;
    static constexpr auto MaxContainerSize = 0xFFFFFFFF;

    void Writer::WriteAtom(const std::string& Value)
    {
        if (Value.size() < MaxSmallAtomSize)
        {
            Write(Constants::SmallAtom);
            Write((uint8_t)Value.size());
            Write(std::span(Value.data(), Value.size()));
        }
        else if (Value.size() <= MaxAtomSize)
        {
            Write(Constants::Atom);
            Write(AsBigEndian((uint16_t)Value.size()));
            Write(std::span(Value.data(), Value.size()));
        }
        else
        {
            throw std::length_error("Atom is too big");
        }
    }

    void Writer::WriteAtomUtf8(const std::u8string& Value)
    {
        if (Value.size() < MaxSmallAtomSize)
        {
            Write(Constants::SmallAtomUtf8);
            Write((uint8_t)Value.size());
            Write(std::span(Value.data(), Value.size()));
        }
        else if (Value.size() <= MaxAtomSize)
        {
            Write(Constants::AtomUtf8);
            Write(AsBigEndian((uint16_t)Value.size()));
            Write(std::span(Value.data(), Value.size()));
        }
        else
        {
            throw std::length_error("Atom value is too big");
        }
    }

    template <size_t Extent>
    void Writer::WriteBinary(std::span<const std::byte, Extent> Value)
    {
        if (Value.size() > MaxContainerSize)
        {
            throw std::length_error("Binary value is too big");
        }

        Write(Constants::Binary);
        Write(AsBigEndian((uint32_t)Value.size()));
        WriteBytes(Value);
    }

    void Writer::WriteString(const std::string& Value)
    {
        if (Value.size() > MaxStringSize)
        {
            throw std::length_error("Binary value is too big");
        }

        Write(Constants::String);
        Write(AsBigEndian((uint16_t)Value.size()));
        Write(std::span(Value.data(), Value.size()));
    }

    void Writer::WriteTupleHeader(size_t Size)
    {
        if (Size <= MaxSmallTupleSize)
        {
            Write(Constants::SmallTuple);
            Write((uint8_t)Size);
        }
        else if (Size <= MaxContainerSize)
        {
            Write(Constants::LargeTuple);
            Write(AsBigEndian((uint32_t)Size));
        }
        else
        {
            throw std::length_error("Tuple value is too big");
        }
    }

    void Writer::WriteNilExt()
    {
        Write(Constants::Nil);
    }

    void Writer::WriteListHeader(size_t Size)
    {
        if (Size <= MaxContainerSize)
        {
            Write(Constants::List);
            Write(AsBigEndian((uint32_t)Size));
        }
        else
        {
            throw std::length_error("List value is too big");
        }
    }

    void Writer::WriteMapHeader(size_t Size)
    {
        if (Size <= MaxContainerSize)
        {
            Write(Constants::Map);
            Write(AsBigEndian((uint32_t)Size));
        }
        else
        {
            throw std::length_error("Map value is too big");
        }
    }

    template <class T>
    void Writer::Write(T Data)
    {
        WriteBytes(std::as_bytes(std::span(&Data, 1)));
    }

    template <class T, size_t Extent>
    void Writer::Write(std::span<T, Extent> Data)
    {
        WriteBytes(std::as_bytes(Data));
    }

    template <size_t Extent>
    void Writer::WriteBytes(std::span<const std::byte, Extent> Data)
    {
        Buffer.reserve(Buffer.size() + Data.size());
        std::ranges::copy(Data, std::back_inserter(Buffer));
    }
}