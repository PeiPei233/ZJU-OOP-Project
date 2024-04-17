#pragma once

template<class T>
class Vector {
public:
    Vector();                       // creates an empty vector
    Vector(size_t size);            // creates a vector for holding 'size' elements
    Vector(const Vector& r);        // the copy ctor
    ~Vector();                      // destructs the vector 
    T& operator[](size_t index);    // accesses the specified element without bounds checking
    T& at(size_t index);            // accesses the specified element, throws an exception of
                                    // type 'std::out_of_range' when index <0 or >=m_nSize
    size_t size() const;            // return the size of the container
    size_t capacity() const;        // return the capacity of the container
    void push_back(const T& x);     // adds an element to the end 
    void clear();                   // clears the contents
    bool empty() const;             // checks whether the container is empty 
private:
    void inflate();                 // expand the storage of the container to a new capacity,
                                    // e.g. 2*m_nCapacity
    T *m_pElements;                 // pointer to the dynamically allocated storage
    size_t m_nSize;                 // the number of elements in the container
    size_t m_nCapacity;             // the number of elements that can be held in currently
                                    // allocated storage
};

template<class T>
Vector<T>::Vector() : m_pElements(nullptr), m_nSize(0), m_nCapacity(0) {}

template<class T>
Vector<T>::Vector(size_t size) : m_pElements(new T[size]()), m_nSize(size), m_nCapacity(size) {}

template<class T>
Vector<T>::Vector(const Vector& r) : m_pElements(new T[r.m_nSize]()), m_nSize(r.m_nSize), m_nCapacity(r.m_nSize) {
    for (size_t i = 0; i < m_nSize; i++) {
        m_pElements[i] = r.m_pElements[i];
    }
}

template<class T>
Vector<T>::~Vector() {
    delete[] m_pElements;
}

template<class T>
T& Vector<T>::operator[](size_t index) {
    return m_pElements[index];
}

template<class T>
T& Vector<T>::at(size_t index) {
    if (index >= m_nSize) {
        throw std::out_of_range("index out of range");
    }
    return m_pElements[index];
}

template<class T>
size_t Vector<T>::size() const {
    return m_nSize;
}

template<class T>
size_t Vector<T>::capacity() const {
    return m_nCapacity;
}

template<class T>
void Vector<T>::push_back(const T& x) {
    if (m_nSize == m_nCapacity) {
        inflate();
    }
    m_pElements[m_nSize++] = x;
}

template<class T>
void Vector<T>::clear() {
    m_nSize = 0;
}

template<class T>
bool Vector<T>::empty() const {
    return m_nSize == 0;
}

template<class T>
void Vector<T>::inflate() {
    size_t newCapacity = m_nCapacity == 0 ? 1 : 2 * m_nCapacity;
    T *pNewElements = new T[newCapacity];
    for (size_t i = 0; i < m_nSize; i++) {
        pNewElements[i] = m_pElements[i];
    }
    delete[] m_pElements;
    m_pElements = pNewElements;
    m_nCapacity = newCapacity;
}