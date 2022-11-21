#pragma once

#include "Writer.h"

#include <optional>

namespace rapidetf
{
    class Encoder
    {
    public:
        explicit Encoder(size_t InitialCapacity);

        [[nodiscard]] std::span<const std::byte> Get() const noexcept;

        void Reset();

        void Write(int8_t Value);

        void Write(uint8_t Value);

        void Write(int16_t Value);

        void Write(uint16_t Value);

        void Write(int32_t Value);

        void Write(uint32_t Value);

        void Write(int64_t Value);

        void Write(uint64_t Value);

        void Write(bool Value);

        void Write(double Value);

        void Write(const std::string& Value);

        void Write(std::string_view Value);

        void Write(std::nullptr_t Ptr);

        void Write(std::nullopt_t Opt);

        template <class T>
        void Write(const std::optional<T>& Value)
        {
            if (Value)
            {
                Write(Value.value());
            }
            else
            {
                Write(std::nullopt);
            }
        }

        template <class... ArgTs>
        void Write(const std::tuple<ArgTs...>& Value)
        {
            Writer.WriteTupleHeader(sizeof...(ArgTs));
            WriteTupleImpl(Value, std::make_index_sequence<sizeof...(ArgTs)>());
        }

        template <class T, size_t Size>
        void Write(const std::array<T, Size>& Value)
        {
            if constexpr (Size != 0)
            {
                Writer.WriteListHeader(Size);
                for (auto& Element : Value)
                {
                    Write(Element);
                }
            }
            Writer.WriteNilExt();
        }

        template <class T>
        void Write(const std::vector<T>& Value)
        {
            if (!Value.empty())
            {
                Writer.WriteListHeader(Value.size());
                for (auto& Element : Value)
                {
                    Write(Element);
                }
            }
            Writer.WriteNilExt();
        }

        template <class K, class V>
        void Write(const std::pair<K, V>& Value)
        {
            Write(Value.first);
            Write(Value.second);
        }

        template <class K, class V, class Hash, class KeyEqual, class Allocator>
        void Write(const std::unordered_map<K, V, Hash, KeyEqual, Allocator>& Value)
        {
            Writer.WriteMapHeader(Value.size());
            for (auto& Element : Value)
            {
                Write(Element);
            }
        }

    private:
        template <class T, size_t... Idxs>
        void WriteTupleImpl(const T& Value, std::index_sequence<Idxs...> Seq)
        {
            (Write(std::get<Idxs>(Value)), ...);
        }

        Writer Writer;
    };
}