#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix();
  explicit Matrix(const T& elem);
  explicit Matrix(const std::vector<std::vector<T>>& data);

  T& operator()(size_t first, size_t second);
  const T& operator()(size_t first, size_t second) const;

  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& other);
  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& other);
  Matrix<N, M, T>& operator*=(const T& scalar);

  Matrix<M, N, T> Transposed() const;
  T Trace() const;

  bool operator==(const Matrix<N, M, T>& other) const;

 private:
  std::vector<std::vector<T>> data_;
};

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix() : data_(N, std::vector<T>(M, T())) {}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const T& elem) : data_(N, std::vector<T>(M, elem)) {}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const std::vector<std::vector<T>>& data) : data_(data) {
  assert(data.size() == N);
  for (const auto& row : data) {
    assert(row.size() == M);
  }
}

template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(size_t first, size_t second) {
  return data_[first][second];
}

template <size_t N, size_t M, typename T>
const T& Matrix<N, M, T>::operator()(size_t first, size_t second) const {
  return data_[first][second];
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& other) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      data_[i][j] += other.data_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& other) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      data_[i][j] -= other.data_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const T& scalar) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      data_[i][j] *= scalar;
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T>& matrix,
                          const Matrix<N, M, T>& other) {
  Matrix<N, M, T> result(matrix);
  result += other;
  return result;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T>& matrix,
                          const Matrix<N, M, T>& other) {
  Matrix<N, M, T> result(matrix);
  result -= other;
  return result;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const Matrix<N, M, T>& matrix, const T& scalar) {
  Matrix<N, M, T> result(matrix);
  result *= scalar;
  return result;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const T& scalar, const Matrix<N, M, T>& matrix) {
  return matrix * scalar;
}

template <size_t N, size_t M, size_t P, typename T>
Matrix<N, P, T> operator*(const Matrix<N, M, T>& matrix,
                          const Matrix<M, P, T>& other) {
  Matrix<N, P, T> result;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < P; ++j) {
      for (size_t k = 0; k < M; ++k) {
        result(i, j) += matrix(i, k) * other(k, j);
      }
    }
  }
  return result;
}

template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() const {
  Matrix<M, N, T> result;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      result(j, i) = data_[i][j];
    }
  }
  return result;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::Trace() const {
  static_assert(N == M, "Trace is only defined for square matrices.");
  T trace = T();
  for (size_t i = 0; i < N; ++i) {
    trace += data_[i][i];
  }
  return trace;
}

template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& other) const {
  return data_ == other.data_;
}

template <size_t X, size_t Y, typename U>
std::ostream& operator<<(std::ostream& os, const Matrix<X, Y, U>& matrix) {
  for (size_t i = 0; i < X; ++i) {
    for (size_t j = 0; j < Y; ++j) {
      os << matrix(i, j);
      if (j < Y - 1) {
        os << " ";
      }
    }
    os << "\n";
  }
  return os;
}
