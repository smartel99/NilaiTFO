#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <cstdlib>

template<typename T>
class CircularBuffer {
  public:
    CircularBuffer()
    : m_alloc(0) {
    }

    ~CircularBuffer() {
        free(m_c);
    }

    void init(size_t size) {
        if (m_alloc == 0) {
            m_alloc = size;
            m_c     = (T*)malloc(sizeof(T) * size);
        }
    }

    /**
     * Return the first element of the buffer
     */
    T& front() {
        return m_c[m_read];
    }

    /**
     * Increment the read pointer by one, effectively removing the first element of the buffer
     * If the buffer size is 0, does nothing
     */
    void pop() {
        if (size() >= 0) {
            m_read = next(m_read);
            --m_size;
        }
    }

    /**
     * Increment the read pointer by a number n
     * effectively removing the n first elements of the buffer
     */
    void pop(size_t n) {
        if (n >= size())    // if we want to remove more elements that we stored, clear will be enough
            clear();
        else {
            while (n >= m_alloc)
                n -= m_alloc;
            m_read += n;
            m_size -= n;
            while (m_read >= m_alloc)
                m_read -= m_alloc;
        }
    }

    /**
     * Insert an element at the end of the buffer
     * If the buffer is full, does nothing
     * @param t the element to insert
     */
    void push(const T t) {
        if (size() < m_alloc) {
            m_c[m_write] = t;
            m_write      = next(m_write);
            ++m_size;
        }
    }

    /**
     * Return the number of elements inserted in the buffer
     * Does not return the max size of the buffer
     * Elements inserted using operator [] are ignored
     */
    size_t size() const {
        return m_size;
    }

    /**
     * Reset the cursor and size of the buffer to 0
     * Effectively clearing the buffer
     */
    void clear(void) {
        m_size  = 0;
        m_read  = 0;
        m_write = 0;
    }

    /**
     * Access to the underlying array
     */
    T* data() {
        return m_c;
    }

    /**
     * array access operator for writing
     * Note: does not update cursors position nor array size counter;
     */
    T& operator[](int i) {
        return m_c[idx(i)];
    }

    /**
     * array access operation for reading
     */
    T operator[](int i) const {
        return m_c[idx(i)];
    }

  private:
    T*     m_c;
    size_t m_alloc;    // Cannot be const, because we want default operator=
                       // This is used so we have different sized buffer
    size_t m_read  = 0;
    size_t m_write = 0;
    size_t m_size  = 0;

    size_t next(size_t origin) const {
        size_t r = origin + 1;
        while (r >= m_alloc)
            r -= m_alloc;
        return r;
    }

    size_t idx(size_t idx) const {
        size_t r = m_read + idx;
        while (r >= m_alloc)
            r -= m_alloc;
        return r;
    }
};

#endif
