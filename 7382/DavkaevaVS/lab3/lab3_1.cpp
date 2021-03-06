#include "pch.h"
#include <iostream>
#include <algorithm> // std::copy, std::rotate
#include <cstddef> // size_t
#include <initializer_list>
#include <stdexcept>
namespace stepik
{
	template <typename Type>
	class vector
	{
	public:
		typedef Type* iterator;
		typedef const Type* const_iterator;

		typedef Type value_type;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef std::ptrdiff_t difference_type;

		explicit vector(int count = 0)
		{
			m_first = new Type[count];
			m_last = m_first + count;
			std::cout << "конструктор от размера массва\n";
		}

		template <typename InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			Type *first_t = (Type*)first;
			Type *last_t = (Type*)last;

			m_first = new Type[last_t - first_t];
			m_last = m_first + (last_t - first_t);
			std::copy(first_t, last_t, m_first);
			std::cout << "конструктор от 2х итераторов \n";
		}

		vector(std::initializer_list<Type> init) :vector(init.begin(), init.end()) { std::cout << "конструктор списка инициализации\n"; }

		vector(const vector& other) :vector(other.m_first, other.m_last) { std::cout << "конструктор копирования \n"; }

		vector(vector&& other)
		{
			swap(*this, other);
			std::cout << "перемещение\n";
		}

		~vector()
		{
			delete[] m_first;
			m_first = nullptr;
			m_last = nullptr;
			std::cout << "деструктор\n";
		}

		vector& operator=(const vector& other)
		{
			if (((void*)this) == ((void*)(&other)))return *this;
			vector v(other);
			swap(*this, v);
			std::cout << "присваивание копированием\n";
			return *this;
		}

		vector& operator=(vector&& other)
		{
			if (((void*)this) == ((void*)(&other)))return *this;
			swap(*this, other);
			std::cout << "присваивание перемещением\n";
			return *this;
		}

		template <typename InputIterator>
		void assign(InputIterator first, InputIterator last)
		{
			vector v(first, last);
			swap(*this, v);
			std::cout << "assign\n";
		}


		void resize(size_t count)
		{
			size_t c = m_last - m_first;
			if (count == c)return;

			vector v(count);
			size_t ms = count < c ? count : c;
			std::copy(m_first, m_first + ms, v.m_first);
			swap(*this, v);
		}

		iterator erase(const_iterator pos)
		{
			size_t p = (Type*)pos - m_first;
			vector v = *this;
			std::rotate(v.m_first + p, v.m_first + p + 1, v.m_last);
			v.resize(v.size() - 1);
			*this = std::move(v);
			std::cout << "удаление 1го элемента\n";
			return m_first + p;
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			size_t f = (Type*)first - m_first;
			size_t l = (Type*)last - m_first;
			vector v = *this;
			std::rotate(v.m_first + f, v.m_first + l, v.m_last);
			v.resize(v.size() - l + f);
			*this = std::move(v);
			std::cout << "удаление диапозона элементов\n";
			return m_first + f;
		}

		//insert methods
		iterator insert(const_iterator pos, const Type& value)
		{
			size_t f = (Type*)pos - m_first;
			vector v(size() + 1);
			std::copy(m_first, m_first + f, v.m_first);
			*(v.m_first + f) = value;
			std::copy(m_first + f, m_last, v.m_first + f + 1);
			*this = std::move(v);
			std::cout << "вставка одного элемента\n";
			return m_first + f;
		}

		template <typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last)
		{
			size_t f = (Type*)pos - m_first;
			size_t l = f + (last - first);
			vector v(size() + last - first);
			std::copy(m_first, m_first + f, v.m_first);
			std::copy(first, last, v.m_first + f);
			std::copy(m_first + f, m_last, v.m_first + l);
			*this = std::move(v);
			std::cout << "вставка диапозона элементов\n";
			return m_first + f;
		}

		void push_back(const value_type& value)
		{
			resize(size() + 1);
			*(m_last - 1) = value;
			std::cout << "вставка элемента в конец\n";
		}

		reference at(size_t pos)
		{
			return checkIndexAndGet(pos);
		}

		const_reference at(size_t pos) const
		{
			return checkIndexAndGet(pos);
		}

		reference operator[](size_t pos)
		{
			return m_first[pos];
		}

		const_reference operator[](size_t pos) const
		{
			return m_first[pos];
		}

		iterator begin()
		{
			return m_first;
		}

		const_iterator begin() const
		{
			return m_first;
		}

		iterator end()
		{
			return m_last;
		}

		const_iterator end() const
		{
			return m_last;
		}

		size_t size() const
		{
			return m_last - m_first;
		}

		bool empty() const
		{
			return m_first == m_last;
		}

	private:
		reference checkIndexAndGet(size_t pos) const
		{
			if (pos >= size())
			{
				throw std::out_of_range("out of range");
			}

			return m_first[pos];
		}

		static void swap(vector& a, vector& b) {
			std::swap(a.m_first, b.m_first);
			std::swap(a.m_last, b.m_last);
		}

	private:
		iterator m_first = nullptr;
		iterator m_last = nullptr;
	};
}

int main()
{
	setlocale(LC_ALL, "Russian");
	stepik::vector<int> words1{ 1, 2, 3, 4, 5 };
	//for (int i = 0; i < words1.size(); i++)
		//std::cout << words1[i] << std::endl;
	stepik::vector<int> words2(words1.begin(), words1.end());
	//for (int i = 0; i < words2.size(); i++)
		//std::cout << words2[i] << std::endl;
	stepik::vector<int> words3(5);
	//for (int i = 0; i < words3.size(); i++)
		//std::cout << words3[i] << std::endl;
	stepik::vector<int> words4(words1);
	//for (int i = 0; i < words4.size(); i++)
		//std::cout << words4[i] << std::endl;

	stepik::vector<int> v(std::move(words1));
	//for (int i = 0; i < v.size(); i++)
		//std::cout << v[i] << std::endl;
	words1 = words3;
	//for (int i = 0; i < words1.size(); i++)
		//std::cout << words1[i] << std::endl;
	words1 = std::move(words2);
	//for (int i = 0; i < words1.size(); i++)
		//std::cout << words1[i] << std::endl;
	int myints[] = { 1776,7,4 ,78,5,134};
	words1.assign(myints, myints + 5);
	//for (int i = 0; i < words1.size(); i++)
		//std::cout << words1[i] << std::endl;
	words1.erase(words1.begin()+1);
	//for (int i = 0; i < words1.size(); i++)
		//std::cout << words1[i] << std::endl;
	words1.erase(words1.begin() + 1, words1.begin() + 3);
	//for (int i = 0; i < words1.size(); i++)
		//std::cout << words1[i] << std::endl;
	words1.push_back(23);
	words1.insert(words1.begin()+1, 345);
	words1.insert(words1.begin(), words3.begin(), words3.begin() + 2);
	for (int i = 0; i < words1.size(); i++)
		std::cout << words1[i] << std::endl;



}