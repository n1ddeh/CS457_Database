/**
 * File: column.h
 * Author: Mark Minkoff
 * Functionality: Function declarations for file column.cpp
 * 
 * */

#ifndef COLUMN_H_
#define COLUMN_H_

#include "include.h"

template <class T>
class Column
{
private:

    std::string column_name;        // Column Name
    unsigned int data_type;         // type of elements it stores
    std::vector<T> elements;        // Container for elements
    size_t CHAR_MAX;                // Used for VARCHAR types

public:
    // ---------------------------
    // ---- Constructors
    // ---------------------------

    // Constructor for Column<int>
    Column(std::string, std::vector<int>);

    // Constructor for Column<float>
    Column(std::string, std::vector<float>);

    // Constructor for Column<float>
    Column(std::string, std::vector<char>);

    // Constructor for Column<float>
    Column(std::string, std::vector<std::string>, size_t);

    // ---------------------------
    // ---- Mutator Functions
    // ---------------------------

    // Inserts an element
    bool insertElement(T);

    // Deletes an element at some specified row
    bool deleteElement(const size_t);

    // ---------------------------
    // ---- Getter Functions
    // ---------------------------

    std::string getName() {return this->column_name;}
    unsigned int getDataType() {return this->data_type;}
    std::vector<T> getElements() {return this->elements;}
    size_t getCharMax() {return this->CHAR_MAX;}

    // ---------------------------
    // ---- Helper Functions
    // ---------------------------

    std::unordered_set<size_t> filterElements(const std::string& op, T val);
    size_t updateElementsOnIndex(const std::unordered_set<size_t>&, const T&);
};

#endif // COLUMN_H_
