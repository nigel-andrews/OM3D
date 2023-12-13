#ifndef TYPEDBUFFER_H
#define TYPEDBUFFER_H

#include <ByteBuffer.h>
#include <cstring>
#include <vector>

namespace OM3D
{

    template <typename T>
    class TypedBuffer : public ByteBuffer
    {
    public:
        TypedBuffer() = default;

        TypedBuffer(Span<const T> data)
            : TypedBuffer(data.data(), data.size())
        {}

        TypedBuffer(const T* data, size_t count)
            : ByteBuffer(data, count * sizeof(T))
        {}

        bool is_equal(const TypedBuffer<T>& other) const
        {
            if (this->byte_size() != other.byte_size())
                return false;

            char* lhs =
                static_cast<char*>(calloc(this->byte_size(), sizeof(char)));
            this->read_into(lhs);

            char* rhs =
                static_cast<char*>(calloc(other.byte_size(), sizeof(char)));
            other.read_into(rhs);

            return std::memcmp(lhs, rhs, this->byte_size()) == 0;
        }

        size_t element_count() const
        {
            DEBUG_ASSERT(byte_size() % sizeof(T) == 0);
            return byte_size() / sizeof(T);
        }

        BufferMapping<T> map(AccessType access = AccessType::ReadWrite)
        {
            return BufferMapping<T>(ByteBuffer::map_internal(access),
                                    byte_size(), handle());
        }
    };

} // namespace OM3D

#endif // TYPEDBUFFER_H
