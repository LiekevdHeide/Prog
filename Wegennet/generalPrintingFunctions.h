#pragma once
#include <vector>
#include <fstream>

void print2Dim(std::vector<std::vector<size_t>> toPrint, size_t dim1, size_t dim2);
void print2Dim(std::vector<std::vector<size_t>> toPrint, size_t dim1, size_t dim2, std::ofstream&);

void print2Dim(std::vector<std::vector<double>> toPrint, size_t dim1, size_t dim2);
void print2Dim(std::vector<std::vector<double>> toPrint, size_t dim1, size_t dim2, std::ofstream&);

void print2Dim(std::vector<std::vector<size_t>> toPrint, size_t dim1);
void print2Dim(std::vector<std::vector<size_t>> toPrint, size_t dim1, std::ofstream&);

void print2Dim(std::vector<std::vector<double>> toPrint, size_t dim1);
void print2Dim(std::vector<std::vector<double>> toPrint, size_t dim1, std::ofstream&);
