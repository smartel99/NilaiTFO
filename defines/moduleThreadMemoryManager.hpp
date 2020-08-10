//#pragma once
//
//#include "_pch.hpp"
//
//
//#define DEFAULT_STACK_SIZE 512
//
//namespace cep
//{
//
//// template<typename ElementType>
//// using shared_ptr_type = std::pmr::vector<ElementType>;
//// template<typename ElementType>
//// using vector = std::shared_ptr<shared_ptr_type<ElementType>>;
//
//template<std::size_t stack_size>
//class ModuleThreadMemoryManager
//{
//    friend class moduleThread;
//
//    using mbr = std::pmr::monotonic_buffer_resource;
//
//public:
//    ModuleThreadMemoryManager() : m_bufferResource(m_buffer, stack_size) {}
//
//    [[nodiscard]] constexpr std::uint8_t* RawBuffer() { return m_buffer; }
//    [[nodiscard]] constexpr std::size_t   StackSize() { return stack_size; }
//    [[nodiscard]] mbr&                    GetBufferResource() { return m_bufferResource; }
//
//private:
//    std::uint8_t m_buffer[stack_size];
//    mbr          m_bufferResource;
//};
//}    // namespace cep
