#pragma once
// Copyright 2017 - Andromeda Developers

#include <vector>
#include <memory>

namespace Andromeda {
	namespace Utils {
		class NibbleArray {
		private:
			char* _data;
		public:
			NibbleArray() {
			}

			NibbleArray(int length) {
				//data.reserve(length);
				//data.resize(length);
				_data = new char[length];
			}

			inline char* get() {
				return (char*)_data;
			}

			inline char get(int index) const {
				return this->_data[index / 2] >> ((index) % 2 * 4) & 0xF;
			}

			inline void set(int index, char value) {
				value &= 0xF;
				this->_data[index / 2] &= (0xf << (index + 1) % 2 * 4);
				this->_data[index / 2] |= value << (index % 2 * 4);
			}

			~NibbleArray() {
				delete _data;
			}
		};
	}
}