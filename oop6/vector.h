#pragma once


#include <bits/allocator.h>
#include <memory>
#include <limits>
#include <cstring>


namespace andrey {

    template<class Type, class Allocator = std::allocator<Type>>
    class Vector {

    public:
        using value_type = Type;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const reference;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

        template<class T>
        class VectorIterator : public std::iterator<std::random_access_iterator_tag, value_type> {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using reference = T &;
            using pointer = T *;
            using iterator_category = std::random_access_iterator_tag;

            VectorIterator &operator=(value_type *rhs) {
                ptr_ = rhs;
                return *this;
            }

            VectorIterator &operator+=(const int &rhs) {
                ptr_ += rhs;
                return *this;
            }

            inline VectorIterator &operator-=(const int &rhs) {
                ptr_ -= rhs;
                return *this;
            }

            reference operator*() const { return *ptr_; }

            pointer operator->() const { return ptr_; }

            reference operator[](const int &index) const { return ptr_[index]; }

            VectorIterator &operator++() {
                ++ptr_;
                return *this;
            }

            VectorIterator operator++(int) {
                return VectorIterator(ptr_++);
            }

            VectorIterator &operator--() {
                --ptr_;
                return *this;
            }

            VectorIterator operator--(int) {
                return ptr_--;
            }

            VectorIterator operator+(difference_type n) const {
                return {ptr_ + n};
            }

            VectorIterator operator-(difference_type n) const {
                return {ptr_ - n};
            }

            difference_type operator-(const VectorIterator &r) const {
                return ptr_ - r.ptr_;
            }

            VectorIterator &operator+=(difference_type n) {
                ptr_ += n;
                return *this;
            }

            VectorIterator &operator-=(difference_type n) {
                ptr_ -= n;
                return *this;
            }

            bool operator<(const VectorIterator &rhs) const {
                return ptr_ < rhs.ptr_;
            }

            bool operator>(const VectorIterator &rhs) const {
                return ptr_ > rhs.ptr_;
            }

            bool operator<=(const VectorIterator &rhs) const {
                return ptr_ <= rhs.ptr_;
            }

            bool operator>=(const VectorIterator &rhs) const {
                return ptr_ >= rhs.ptr_;
            }

            bool operator==(const VectorIterator &rhs) const {
                return ptr_ == rhs.ptr_;
            }

            bool operator!=(const VectorIterator &rhs) const {
                return ptr_ != rhs.ptr_;
            }

        public:

            VectorIterator(const VectorIterator &other) : ptr_(other.ptr_) {}

            VectorIterator(value_type *other) : ptr_(other) {}

        private:
            value_type *ptr_;

        };

        using iterator = VectorIterator<Type>;
        using reverse_iterator = std::reverse_iterator<iterator>;

        using const_iterator = VectorIterator<const Type>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        explicit Vector() : size_(0), capacity_(defaultCapacity) {
            data_ = alloc_.allocate(defaultCapacity);
        }

        Vector(size_type initialSize, const value_type &value) : size_(0), capacity_(initialSize) {
            data_ = alloc_.allocate(capacity_);
            resize(initialSize, value);
        }

        explicit Vector(size_type initialSize) : size_(initialSize), capacity_(initialSize) {
            data_ = alloc_.allocate(initialSize);
        }

        template<class InputIterator,
                class SFINAE =
                typename std::enable_if<!std::is_integral<InputIterator>::value
                                        &&
                                        std::is_base_of<
                                                std::input_iterator_tag,
                                                typename std::iterator_traits<InputIterator>::iterator_category>
                                        ::value
                >::type
        >
        Vector(InputIterator first, InputIterator last) : size_(
                static_cast<size_type>(std::distance(first, last))), capacity_(size_) {
            data_ = alloc_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i) {
                alloc_.construct(data_ + i, *first++);
            }
        }

        Vector(const Vector &right) : size_(right.size_), capacity_(right.capacity_) {
            data_ = alloc_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i) {
                alloc_.construct(data_ + i, right.data_[i]);
            }
        }

        Vector(Vector &&other) noexcept : data_(other.data_),
                                          size_(other.size_),
                                          capacity_(other.capacity_),
                                          alloc_(other.alloc_) {
            other.capacity_ = 0;
            other.size_ = 0;
            other.data_ = nullptr;
        }

        Vector(std::initializer_list<value_type> initializerList) : Vector(initializerList.begin(),
                                                                           initializerList.end()) {}

        Vector &operator=(const Vector &rhs) {
            if (this != &rhs) {
                deleter(data_, data_ + size_);
                alloc_.deallocate(data_, size_);
                size_ = rhs.size_;
                capacity_ = rhs.capacity_;
                data_ = alloc_.allocate(capacity_);
                for (size_type i = 0; i < size_; ++i) {
                    alloc_.construct(data_ + i, rhs.data_[i]);
                }
            }
            return *this;
        }

        Vector &operator=(Vector &&rhs) noexcept {
            if (this != &rhs) {

                deleter(data_, data_ + size_);
                alloc_.deallocate(data_, size_);
                data_ = rhs.data_;
                size_ = rhs.size_;
                capacity_ = rhs.capacity_;
                rhs.data_ = nullptr;
                rhs.size_ = 0;
                rhs.capacity_ = 0;
            }
            return *this;
        }


        ~Vector() {
            deleter(data_, data_ + size_);
            alloc_.deallocate(data_, size_);
        }

        bool empty() const noexcept {
            return size_ == 0;
        }

        size_type size() const noexcept {
            return size_;
        }

        size_type capacity() const noexcept {
            return capacity_;
        }

        size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max();
        }

        void push_back(const value_type &value) {
            expandIfNecessary();
            alloc_.construct(data_ + size_, value);
            size_++;
        }

        void push_back(value_type &&value) {
            emplace_back(std::move(value));
        }

        template<class... Types>
        void emplace_back(Types &&...args) {
            expandIfNecessary();
            alloc_.construct(data_ + size_, std::forward<Types>(args)...);
            size_++;
        }


        void shrink_to_fit() {
            if (capacity_ == size_) {
                return;
            }
            deleter(data_ + size_, data_ + (capacity_ - size_));
            alloc_.deallocate(data_ + size_, capacity_ - size_);
            capacity_ = size_;

        }

        void reserve(size_type n) {
            if (capacity_ > n) {
                return;
            }
            const size_type amountOfReserve = n - size_;
            value_type *buffer = alloc_.allocate(capacity_ + n);
            memcpy(buffer, data_, size_ * sizeof(value_type));
            alloc_.deallocate(data_, size_);
            data_ = buffer;
            capacity_ += amountOfReserve;
        }

        reference at(size_type position) {
            return const_cast<reference>(static_cast<const Vector *>(this)->at(position));
        }

        const_reference at(size_type position) const {
            if (position >= size_) {
                throw std::out_of_range(
                        "size is " + std::to_string(size_) + ", when position is " + std::to_string(position));
            }
            return data_[position];
        }

        value_type *data() noexcept {
            return data_;
        }

        const value_type *data() const noexcept {
            return data_;
        }

        reference back() noexcept {
            return data_[size_ - 1];
        }

        const_reference back() const noexcept {
            return data_[size_ - 1];
        }

        reference front() noexcept {
            return data_[0];
        }

        const_reference front() const noexcept {
            return data_[0];
        }

        reference operator[](size_type position) {
            return data_[size_ - 1];
        }

        const_reference operator[](size_type position) const {
            return data_[size_ - 1];
        }

        void clear() noexcept {
            deleter(data_, data_ + size_);
            alloc_.deallocate(data_, size_);
            size_ = 0;
        }

        void resize(size_type count, const value_type &value) {
            if (count > size_) {
                for (size_type i = 0; i < count; ++i) {
                    push_back(value);
                }
                size_ = count;
                return;
            }
            if (count < size_) {
                deleter(data_ + count, data_ + size_);
                size_ = count;
                return;
            }
        }

        void resize(size_type count) {
            resize(count, value_type());
        }

        void pop_back() {
            resize(size_ - 1);
        }

        allocator_type get_allocator() const {
            return alloc_;
        }

        void assign(size_type count, const value_type &value) {
            for (size_type i = 0; i < count; ++i) {
                push_back(value);
            }
        }

        void assign(std::initializer_list<value_type> initializerList) {
            assign(initializerList.begin(), initializerList.end());
        }

        template<class Iterator>
        void assign(Iterator first, Iterator last) {
            while (first++ != last) {
                push_back(*first);
            }
        }

        iterator insert(const_iterator pos, const value_type &value) {
            auto distance = std::distance(cbegin(), pos);
            expandIfNecessary();
            shiftArray(distance, 1);
            alloc_.construct(data_ + distance, value);
            size_++;
            return {data_ + distance};
        }

        iterator insert(const_iterator pos, value_type &&value) {
            auto distance = std::distance(cbegin(), pos);
            expandIfNecessary();
            shiftArray(distance, 1);
            alloc_.construct(data_ + distance, std::move(value));
            size_++;
            return {data_ + distance};
        }

        iterator insert(const_iterator pos, size_type count, const value_type &value) {
            auto distance = std::distance(cbegin(), pos);
            reserve(size_ + count);
            shiftArray(distance, count);
            size_ += count;
            auto offset = distance + count;
            for (size_type i = distance; i < offset; ++i) {
                alloc_.construct(data_ + i, value);
            }
            return {data_ + distance};
        }

        template<class InputIterator,
                class SFINAE = typename
                std::enable_if<!std::is_integral<InputIterator>::value
                               &&
                               std::is_base_of<
                                       std::input_iterator_tag,
                                       typename std::iterator_traits<InputIterator>::iterator_category>
                               ::value
                >::type
        >
        iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
            auto distance = std::distance(cbegin(), pos);
            auto count = std::distance(first, last);;
            reserve(size_ + count);
            shiftArray(distance, count);
            size_ += count;
            auto offset = distance + count;
            for (size_type i = distance; i < offset; ++i) {
                alloc_.construct(data_ + i, *first);
                first++;
            }
            return {data_ + distance};
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
            insert(pos, ilist.begin(), ilist.end());
        }

        template<class... Args>
        iterator emplace(const_iterator pos, Args &&... args) {
            auto distance = std::distance(cbegin(), pos);
            expandIfNecessary();
            shiftArray(distance, 1);
            alloc_.construct(data_ + distance, std::forward<Args>(args)...);
            size_++;
            return {data_ + distance};
        }

        iterator end() noexcept {
            return iterator(data_ + size_);
        }

        const_iterator end() const noexcept {
            return const_iterator(data_ + size_);
        }

        const_iterator cend() const noexcept {
            return end();
        }

        iterator begin() noexcept {
            return iterator(data_);
        }

        const_iterator begin() const noexcept {
            return const_iterator(data_);
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator(data_ + size_);
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(data_ + size_);
        }

        const_reverse_iterator crend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(data_);
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(data_);
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        void swap(Vector &other) {
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            std::swap(data_, other.data_);
        }

    private:

        void deleter(value_type *from, value_type *to) {
            while (from != to) {
                alloc_.destroy(from);
                from++;
            }
        }

        void expandIfNecessary() {
            if (size_ == capacity_) {
                capacity_ *= sizeMultiplier_;
                value_type *buffer = alloc_.allocate(capacity_);
                memcpy(buffer, data_, size_ * sizeof(value_type));
                alloc_.deallocate(data_, size_);
                data_ = buffer;
            }
        }

        void shiftArray(size_type begIndex, size_type n) {
            for (size_type i = begIndex; i <= size_; ++i) {
                memcpy(data_ + i + n, data_ + i, sizeof(value_type));
            }
        }

        Allocator alloc_{};
        size_type size_;
        const size_type sizeMultiplier_ = 2;
        const size_type defaultCapacity = 2;
        size_type capacity_;
        value_type *data_;

    }; // end Vector

    using std::swap;

    template<class Type, class Allocator>
    void swap(Vector<Type, Allocator> &lhs, Vector<Type, Allocator> &rhs) {
        lhs.swap(rhs);
    }
} // end andrey