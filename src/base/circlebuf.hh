/*
 * Copyright (c) 2015,2017 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Andreas Sandberg
 */

#ifndef __BASE_CIRCLEBUF_HH__
#define __BASE_CIRCLEBUF_HH__

#include <algorithm>
#include <cassert>
#include <vector>

#include "base/circularQueue.hh"
#include "base/misc.hh"
#include "sim/serialize.hh"

/**
 * Circular buffer backed by a vector though a circularQueue.
 *
 * The data in the cricular buffer is stored in a standard
 * vector.
 *
 */
template<typename T>
class CircleBuf : public circularQueue<T>
{
  protected:
    using circularQueue<T>::_head;
    using circularQueue<T>::_tail;
    using circularQueue<T>::_size;
    using circularQueue<T>::_empty;
  public:
    explicit CircleBuf(size_t size)
        : circularQueue<T>(size) {}
    using circularQueue<T>::empty;
    using circularQueue<T>::begin;
    using circularQueue<T>::end;
    using circularQueue<T>::pop_front;
    using circularQueue<T>::advance_tail;
    /**
     * Return the maximum number of elements that can be stored in
     * the buffer at any one time.
     */
    size_t capacity() const { return _size; }
    /** Return the number of elements stored in the buffer. */
    size_t size() const {
        if (_head > _tail)
            return _head - _tail + 1;
        else if (_empty)
            return 0;
        else
            return _head + _size - _tail + 1;
    }
    /**
     * Remove all the elements in the buffer.
     *
     * Note: This does not actually remove elements from the backing
     * store.
     */
    void flush() {
        _head = _size - 1;
        _tail = 0;
        _empty = true;
    }

    /**
     * Copy buffer contents without advancing the read pointer
     *
     * @param out Output iterator/pointer
     * @param len Number of elements to copy
     */
    template <class OutputIterator>
    void peek(OutputIterator out, size_t len) const {
        peek(out, 0, len);
    }

    /**
     * Copy buffer contents without advancing the read pointer
     *
     * @param out Output iterator/pointer
     * @param offset Offset into the ring buffer
     * @param len Number of elements to copy
     */
    template <class OutputIterator>
    void peek(OutputIterator out, off_t offset, size_t len) const {
        panic_if(offset + len > size(),
                 "Trying to read past end of circular buffer.\n");

        std::copy(begin() + offset, begin() + offset + len, out);
    }

    /**
     * Copy buffer contents and advance the read pointer
     *
     * @param out Output iterator/pointer
     * @param len Number of elements to read
     */
    template <class OutputIterator>
    void read(OutputIterator out, size_t len) {
        peek(out, len);
        pop_front(len);
    }

    /**
     * Add elements to the end of the ring buffers and advance.
     *
     * @param in Input iterator/pointer
     * @param len Number of elements to read
     */
    template <class InputIterator>
    void write(InputIterator in, size_t len) {
        // Writes that are larger than the backing store are allowed,
        // but only the last part of the buffer will be written.
        if (len > _size) {
            in += len - _size;
            len = _size;
        }
        advance_tail(len);
        std::copy(in, in + len, begin());
    }
};

/**
 * Simple FIFO implementation backed by a circular buffer.
 *
 * This class provides the same basic functionallity as the circular
 * buffer with the folling differences:
 * <ul>
 *    <li>Writes are checked to ensure that overflows can't happen.
 *    <li>Unserialization ensures that the data in the checkpoint fits
 *        in the buffer.
 * </ul>
 */
template<typename T>
class Fifo
{
  public:
    typedef T value_type;

  public:
    Fifo(size_t size)
        : buf(size) {}

    bool empty() const { return buf.empty(); }
    size_t size() const { return buf.size(); }
    size_t capacity() const { return buf.capacity(); }

    void flush() { buf.flush(); }

    template <class OutputIterator>
    void peek(OutputIterator out, size_t len) const { buf.peek(out, len); }
    template <class OutputIterator>
    void read(OutputIterator out, size_t len) { buf.read(out, len); }

    template <class InputIterator>
    void write(InputIterator in, size_t len) {
        panic_if(size() + len > capacity(),
                 "Trying to overfill FIFO buffer.\n");
        buf.write(in, len);
    }

  private:
    CircleBuf<value_type> buf;
};


template <typename T>
static void
arrayParamOut(CheckpointOut &cp, const std::string &name,
              const CircleBuf<T> &param)
{
    std::vector<T> temp(param.size());
    param.peek(temp.begin(), temp.size());
    arrayParamOut(cp, name, temp);
}

template <typename T>
static void
arrayParamIn(CheckpointIn &cp, const std::string &name,
             CircleBuf<T> &param)
{
    std::vector<T> temp;
    arrayParamIn(cp, name, temp);

    param.flush();
    param.write(temp.cbegin(), temp.size());
}

template <typename T>
static void
arrayParamOut(CheckpointOut &cp, const std::string &name,
              const Fifo<T> &param)
{
    std::vector<T> temp(param.size());
    param.peek(temp.begin(), temp.size());
    arrayParamOut(cp, name, temp);
}

template <typename T>
static void
arrayParamIn(CheckpointIn &cp, const std::string &name,
             Fifo<T> &param)
{
    std::vector<T> temp;
    arrayParamIn(cp, name, temp);

    fatal_if(param.capacity() < temp.size(),
             "Trying to unserialize data into too small FIFO\n");

    param.flush();
    param.write(temp.cbegin(), temp.size());
}

#endif // __BASE_CIRCLEBUF_HH__
