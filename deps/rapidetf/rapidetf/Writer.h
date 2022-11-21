#pragma once

#include "Constants.h"

#include <span>
#include <string>
#include <vector>

namespace rapidetf
{
    class Writer
    {
    public:
        explicit Writer(size_t InitialCapacity);

        [[nodiscard]] std::span<const std::byte> Get() const noexcept;

        void Reset();

        void WriteVersion();

        void WriteNil();

        void WriteFalse();

        void WriteTrue();

        void WriteUInt8(uint8_t Value);

        void WriteInt32(int32_t Value);

        void WriteUInt64(uint64_t Value);

        void WriteInt64(int64_t Value);

        void WriteDouble(double Value);

        void WriteAtom(const std::string& Value);

        void WriteAtomUtf8(const std::u8string& Value);

        template <size_t Extent>
        void WriteBinary(std::span<const std::byte, Extent> Value);

        void WriteString(const std::string& Value);

        void WriteTupleHeader(size_t Size);

        void WriteNilExt();

        void WriteListHeader(size_t Size);

        void WriteMapHeader(size_t Size);

    private:
        template <class T>
        void Write(T Data);

        template <class T, size_t Extent>
        void Write(std::span<T, Extent> Data);

        template <size_t Extent>
        void WriteBytes(std::span<const std::byte, Extent> Data);

        std::vector<std::byte> Buffer;
    };
}