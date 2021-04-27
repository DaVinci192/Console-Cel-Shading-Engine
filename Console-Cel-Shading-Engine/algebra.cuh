#ifndef ALGEBRA
#define ALGEBRA

#include <iostream>
#include <vector>

namespace algebra
{
	template<typename T>
	class Matrix
	{
	private:
		std::vector<T> ptr;
		int rows;
		int cols;

	public:
		Matrix<T>(int rows0, int cols0)
		{
			rows = rows0;
			cols = cols0;
			ptr.resize(rows * cols);
		}

		int num_cols()
		{
			return cols;
		}

		int num_rows()
		{
			return rows;
		}

		T* data()
		{
			return ptr.data();
		}

		void populate(T val)
		{
			for (int i = 0; i < rows * cols; i++)
			{
				ptr[i] = val;
			}
		}

		T& operator()(int i, int j)
		{
			if (i > rows - 1 || j > cols - 1)
				throw std::invalid_argument("Index out of bounds!");
			
			return ptr[j + i * cols];
		}

		Matrix<T> operator+(Matrix<T>& matrix_2)
		{
			if (this->rows != matrix_2.num_rows() ||
				this->cols != matrix_2.num_cols())
			{
				throw std::invalid_argument("Arrays not the same dimension!");
			}

			Matrix<T> res = Matrix<T>(rows, cols);

			// CPU-bound adding using nested for-loops
			for (int j = 0; j < cols; j++)
			{
				for (int i = 0; i < rows; i++)
				{
					res(i, j) = (*this)(i, j) + matrix_2(i, j);
				}
			}
			
			return res;

		}

		Matrix<T> operator-(Matrix<T>& matrix_2)
		{
			if (this->rows != matrix_2.num_rows() ||
				this->cols != matrix_2.num_cols())
			{
				throw std::invalid_argument("Arrays not the same dimension!");
			}

			Matrix<T> res = Matrix<T>(rows, cols);

			// CPU-bound subtracting using nested for loops
			for (int j = 0; j < cols; j++)
			{
				for (int i = 0; i < rows; i++)
				{
					res(i, j) = (*this)(i, j) - matrix_2(i, j);
				}
			}

			return res;
		}

		Matrix<T> operator*(Matrix<T>& matrix_2)
		{
			if (this->cols != matrix_2.num_rows())
				throw std::invalid_argument("Invalid dimensions for matrix multiplication!");

			Matrix<T> res = Matrix<T>(this->rows, matrix_2.num_cols());

			// CPU-bound multiplication using nested for loops
			for (int j = 0; j < res.num_cols(); j++)
			{
				for (int i = 0; i < res.num_rows(); i++)
				{
					for (int k = 0; k < cols; k++)
					{
						res[i][j] += (*this)(i, j) * matrix_2(i, j);
					}
				}
			}
		}

		Matrix<T> transpose()
		{
			Matrix<T> res = Matrix<T>(this->rows, this->cols);

			for (int i = 0; i < cols; i++)
			{
				for (int j = 0; j < rows; j++)
				{
					res(j, i) = (*this)(i, j);
				}
			}
			return res;
		}

		void print()
		{
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					std::cout << ptr[i + j * cols] << " ";
				}
				std::cout << std::endl;
			}
		}

	};
}


#endif ALGEBRA