#pragma once


#include "cgp/02_numarray/numarray.hpp"

#include <string>
#include <sstream>
#include <fstream>

namespace cgp
{
	/** Ensure that a file already exists and is accessible
	 * Display an error message with some reminder on path setting in case the file cannot be accessed */
	void assert_file_exist(std::string const& filename);

	/** Return true if the file can be accessed, false otherwise */
	bool check_file_exist(std::string const& filename);

	/** Return true if a path (file or directory) exists, false otherwise */
	bool check_path_exist(std::string const& pathname);

	/** Return the size in octets of a file*/
	size_t file_get_size(std::string const& filename);

	/** Read the entire content of a file as binary vector of octets*/
	std::vector <char> read_from_file_binary(std::string const& filename);

	std::string read_text_file(std::string const& filename);
	template <typename T> void read_from_file(std::string const& filename, T& data);
	template <typename T> void read_from_file(std::string const& filename, numarray<numarray<T>>& data);
	template <typename T> void read_from_file(std::string const& filename, std::vector<std::vector<T>>& data);

	template <typename T> std::istream& read_from_stream(std::istream& stream, T& data);
	template <typename T, int N> std::istream& read_from_stream(std::istream& stream, numarray_stack<T,N>& data);
	template <typename T, int N> std::istream& read_from_stream(std::istream& stream, std::array<T,N>& data);
	template <typename T> std::istream& read_line_from_stream(std::istream& stream, T& data);
	template <typename T> std::istream& read_from_stream(std::istream& stream, numarray<T>& data);
	template <typename T> std::istream& read_from_stream(std::istream& stream, std::vector<T>& data);
	template <typename T> std::istream& read_from_stream_per_line(std::istream& stream, numarray<T>& data);
	template <typename T> std::istream& read_from_stream_per_line(std::istream& stream, std::vector<T>& data);
}


namespace cgp
{
	template <typename T>
	std::istream& read_from_stream(std::istream& stream, T& data)
	{
		stream >> data;
		return stream;
	}

	template <typename T, int N>
	std::istream& read_from_stream(std::istream& stream, numarray_stack<T, N>& data)
	{
		for (int k = 0; k < N && stream.good(); ++k)
			read_from_stream(stream, data[k]);
		return stream;
	}
	template <typename T, int N>
	std::istream& read_from_stream(std::istream& stream, std::array<T, N>& data)
	{
		for (int k = 0; k < N && stream.good(); ++k)
			read_from_stream(stream, data[k]);
		return stream;
	}

	template <typename T>
	std::istream& read_line_from_stream(std::istream& stream, T& data)
	{
		if(stream.good()) {
			std::string line;
			std::getline(stream, line);
			if(stream.good())
			{
				std::istringstream stream_line(line);
				read_from_stream(stream_line, data);
			}
		}

		return stream;
	}

	template <typename T>
	std::istream& read_from_stream(std::istream& stream, numarray<T>& data)
	{
		while(stream.good()) {
			T temp;
			read_from_stream(stream, temp);
			if(stream)
				data.push_back(temp);
		}
		
		return stream;
	}
	template <typename T>
	std::istream& read_from_stream(std::istream& stream, std::vector<T>& data)
	{
		while(stream.good()) {
			T temp;
			read_from_stream(stream, temp);
			if(stream)
				data.push_back(temp);
		}
		
		return stream;
	}

	template <typename T>
	std::istream& read_from_stream_per_line(std::istream& stream, numarray<T>& data)
	{
		while(stream.good()) {
			T temp;
			read_line_from_stream(stream, temp);
			if(stream.good())
				data.push_back(temp);
		}

		return stream;
	}
	template <typename T>
	std::istream& read_from_stream_per_line(std::istream& stream, std::vector<T>& data)
	{
		while(stream.good()) {
			T temp;
			read_line_from_stream(stream, temp);
			if(stream.good())
				data.push_back(temp);
		}

		return stream;
	}

	template <typename T>
	void read_from_file(std::string const& filename, T& data)
	{
		assert_file_exist(filename);

		// Open file with pointer at last position
		std::ifstream stream(filename);
		assert_cgp_no_msg(stream.is_open());

		read_from_stream(stream, data);

		stream.close();
		assert_cgp_no_msg(!stream.is_open());
	}

	template <typename T>
	void read_from_file(std::string const& filename, numarray<numarray<T>>& data)
	{
		assert_file_exist(filename);

		// Open file with pointer at last position
		std::ifstream stream(filename);
		assert_cgp_no_msg(stream.is_open());

		read_from_stream_per_line(stream, data);

		stream.close();
		assert_cgp_no_msg(!stream.is_open());
	}
	template <typename T>
	void read_from_file(std::string const& filename, std::vector<std::vector<T>>& data)
	{
		assert_file_exist(filename);

		// Open file with pointer at last position
		std::ifstream stream(filename);
		assert_cgp_no_msg(stream.is_open());

		read_from_stream_per_line(stream, data);

		stream.close();
		assert_cgp_no_msg(!stream.is_open());
	}


}