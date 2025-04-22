#include "field.h"

Vector::Vector(size_t initial_size) : length(0), size(initial_size) {
    data = new uint64_t[size]();
}

Vector::Vector(const Vector& other) : length(other.length), size(other.size) {
    data = new uint64_t[size]();
    std::memcpy(data, other.data, length * sizeof(uint64_t));
}

Vector::~Vector() {
    delete[] data;
}

void Vector::push_back(uint64_t value) {
    if (length == size) {
        resize(size * 2);
    }
    data[length++] = value;
}

void Vector::push_front(uint64_t value) {
    if (length == size) {
        resize(size * 2);
    }
    for (size_t i = length; i > 0; --i) {
        data[i] = data[i - 1];
    }
    data[0] = value;
    ++length;
}

uint64_t& Vector::operator[](size_t index) {
    if (index >= length) {
        std::cout << "Index out of range(Vector)";
        exit(1);
    }
    return data[index];
}

size_t Vector::get_length() const {
    return length;
}

void Vector::resize(size_t new_size) {
    uint64_t* new_data = new uint64_t[new_size]();
    std::memcpy(new_data, data, length * sizeof(uint64_t));
    delete[] data;
    data = new_data;
    size = new_size;
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        delete[] data;
        length = other.length;
        size = other.size;
        data = new uint64_t[size]();
        std::memcpy(data, other.data, length * sizeof(uint64_t));
    }
    return *this;
}

Field::Field(size_t initial_size) : length(0), size(initial_size) {
    vectors = new Vector[size];
}

Field::~Field() {
    delete[] vectors;
}

void Field::push_back(const Vector& vector) {
    if (length == size) {
        resize(size * 2);
    }
    vectors[length++] = vector;
}

void Field::push_front(const Vector& vector) {
    if (length == size) {
        resize(size * 2);
    }
    for (size_t i = length; i > 0; --i) {
        vectors[i] = vectors[i - 1];
    }
    vectors[0] = vector;
    ++length;
}

Field& Field::operator=(const Field& other) {
    if (this == &other) {
        return *this;
    }

    delete[] vectors;

    size = other.size;
    length = other.length;

    vectors = new Vector[size];
    for (size_t i = 0; i < length; ++i) {
        vectors[i] = other.vectors[i];
    }

    return *this;
}

Field::Field(const Field& other) : length(other.length), size(other.size) {
    vectors = new Vector[size];
    for (size_t i = 0; i < length; ++i) {
        vectors[i] = other.vectors[i];
    }
}

Vector& Field::operator[](size_t index) const {
    if (index >= length) {
        std::cout << "Index out of range(Field)";
        exit(1);
    }
    return vectors[index];
}

size_t Field::get_length() const {
    return length;
}

void Field::resize(size_t new_size) {
    Vector* new_vectors = new Vector[new_size];
    for (size_t i = 0; i < length; ++i) {
        new_vectors[i] = vectors[i];
    }
    delete[] vectors;
    vectors = new_vectors;
    size = new_size;
}

void Field::expand() {
    size_t new_length = length + 2;
    size_t new_width = vectors[0].get_length() + 2;

    Vector new_column(new_width);
    
    for (size_t i = 0; i < new_width; ++i) {
        new_column.push_back(0);
    }

    for (size_t i = 0; i < length; ++i) {
        vectors[i].push_front(0);
        vectors[i].push_back(0);
    }

    push_front(new_column);
    push_back(new_column);

    length = new_length;
}
