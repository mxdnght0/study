#pragma once

#include <iostream>
#include <cstdint>
#include <filesystem>
#include <cstring>

const uint64_t NUM_OF_NEGATIVE = 32768;

struct Vector {
    uint64_t* data;
    size_t length; 
    size_t size;

    Vector(size_t initial_size = 4);
    Vector(const Vector& other);
    ~Vector();
    void push_back(uint64_t value);
    void push_front(uint64_t value);
    uint64_t& operator[](size_t index);
    Vector& operator=(const Vector& other);
    size_t get_length() const;
    void resize(size_t new_size);
    Vector& operator=(const uint8_t* arr);
};

struct Field {
    Vector* vectors;
    size_t length;
    size_t size;

    Field(size_t initial_size = 4);
    Field(const Field& other);
    ~Field();
    void push_back(const Vector& vector);
    void push_front(const Vector& vector);
    Vector& operator[](size_t index) const;
    Field& operator=(const Field& other);
    size_t get_length() const;
    void resize(size_t new_size);
    void expand();
};
