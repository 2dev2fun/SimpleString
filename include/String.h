// Copyright (C) 2020 Maxim, 2dev2fun@gmail.com. All rights reserved.

#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace simple {
namespace string {

size_t power_of_two(size_t n) {
	size_t power = 0;
	while (n > 0) { n >>= 1; ++power; }
	return power;
}

size_t next_capacity(size_t capacity) {
	return 1 << (power_of_two(capacity) + 1);
}

} // namespace string


class String {
public:
	String() : sso(true) {
		base.fake.size   =   0;
		base.fake.str[0] = '\0';
	}

	String(char const* text) : String(text, std::strlen(text)) {}

	String(String const& string) : String(string.data(), string.size()) {}

	String(String&& other) : String() { swap(other); }

	~String() {
		if (!sso && base.real.ptr) {
			std::free(base.real.ptr);
		}
	}

	String& operator=(String other) {
		swap(other);
		return *this;
	}

	String& operator=(char const* text) {
		this->~String();
		new (this) String(text);
		return *this;
	}

	//String& operator=(String const& other) {
		//String string(other);
		//swap(string);
		//return *this;
	//}

	//String& operator=(String&& other) {
		//swap(other);
		//return *this;
	//}

	char& operator[](uint32_t index) {
		return *(this->data() + index);
	}

	char const& operator[](uint32_t index) const {
		return *(this->data() + index);
	}

	char* data() {
		if (sso) {
			return base.fake.str;
		} else {
			return base.real.ptr;
		}
	}

	char const* data() const {
		if (sso) {
			return base.fake.str;
		} else {
			return base.real.ptr;
		}
	}

	size_t size() const {
		if (sso) {
			return base.fake.size;
		} else {
			return base.real.size;
		}
	}

	size_t capacity() const {
		if (sso) {
			return sso_capacity;
		} else {
			return base.real.capacity;
		}
	}

	void reserve(size_t new_capacity) {
		if (new_capacity == this->capacity()) { return; }

		if (sso) {
			if (new_capacity > this->capacity()) {
				auto size = base.fake.size;

				char* ptr = static_cast<char*>(std::malloc(new_capacity));

				if (ptr == nullptr) {
					throw std::runtime_error("std::malloc returned nullptr!");
				}

				std::memcpy(ptr, base.fake.str, size + 1);

				base.real.capacity = new_capacity;
				base.real.ptr      = ptr;
				base.real.size     = size;

				sso = false;
			} else {
				// When using SSO, the minimum capacity is always sso_capacity.
			}
		} else {
			if (new_capacity > this->capacity()) {
				char* ptr = static_cast<char*>(std::realloc(base.real.ptr, new_capacity));

				if (ptr == nullptr) {
					throw std::runtime_error("std::realloc returned nullptr!");
				}

				base.real.capacity = new_capacity;
				base.real.ptr      = ptr;
			} else {
				char* ptr = static_cast<char*>(std::realloc(base.real.ptr, new_capacity));

				if (ptr == nullptr) {
					throw std::runtime_error("std::realloc returned nullptr!");
				}

				base.real.capacity = new_capacity;
				base.real.ptr      = ptr;

				if (new_capacity <= this->size()) {
					base.real.ptr[new_capacity - 1] = '\0';
					base.real.size = new_capacity - 1;
				}
			}
		}
	}

	void resize(size_t new_size) {
		if (new_size == this->size()) { return; }

		if (sso && new_size < sso_capacity) {
			base.fake.size = new_size;
			base.fake.str[new_size] = '\0';

			return;
		}

		if (sso && new_size >= sso_capacity) {
			size_t capacity = new_size + 1;

			char* ptr = static_cast<char*>(std::malloc(capacity));

			if (ptr == nullptr) {
				throw std::runtime_error("std::malloc returned nullptr!");
			}

			std::memcpy(ptr, base.fake.str, base.fake.size + 1);

			base.real.capacity = capacity;
			base.real.ptr      = ptr;
			base.real.size     = new_size;

			sso = false;

			return;
		}

		if (new_size < base.real.capacity) {
			base.real.size = new_size;
			base.real.ptr[new_size] = '\0';

			return;
		}

		if (new_size >= base.real.capacity) {
			size_t capacity = string::next_capacity(new_size + 1);

			char* ptr = static_cast<char*>(std::realloc(base.real.ptr, capacity));

			if (ptr == nullptr) {
				throw std::runtime_error("std::realloc returned nullptr!");
			}

			base.real.ptr           = ptr;
			base.real.ptr[new_size] = '\0';
			base.real.capacity      = capacity;
			base.real.size          = new_size;

			return;
		}

		throw std::runtime_error("Undefined case!");
	}

	void append(char const* text, size_t size) {
		size_t new_size = this->size() + size;

		if (new_size <= this->size()) {
			throw std::runtime_error("new_size must be bigger then size!");
		}

		if (sso) {
			if (new_size < this->capacity()) {
				char* ptr = base.fake.str + base.fake.size;
				std::memcpy(ptr, text, size + 1);
				base.fake.size = new_size;
			} else {
				size_t capacity = string::next_capacity(new_size + 1);

				char* ptr = static_cast<char*>(std::malloc(capacity));

				if (ptr == nullptr) {
					throw std::runtime_error("std::malloc returned nullptr!");
				}

				std::memcpy(ptr, base.fake.str, base.fake.size);
				std::memcpy(ptr + base.fake.size, text, size + 1);

				base.real.capacity = capacity;
				base.real.ptr      = ptr;
				base.real.size     = new_size;

				sso = false;
			}
		} else {
			if (new_size < this->capacity()) {
				char* ptr = base.real.ptr + base.real.size;
				std::memcpy(ptr, text, size + 1);
				base.real.size += size;
			} else {
				size_t capacity = string::next_capacity(new_size + 1);

				char* ptr = static_cast<char*>(std::realloc(base.real.ptr, capacity));

				if (ptr == nullptr) {
					throw std::runtime_error("std::malloc returned nullptr!");
				}

				std::memcpy(ptr, base.fake.str, base.fake.size + 1);

				base.real.capacity = capacity;
				base.real.ptr      = ptr;
				base.real.size     = new_size;
			}
		}
	}

	void append(char symbol) {
		append(&symbol, 1);
	}

	void append(char const* text) {
		append(text, std::strlen(text));
	}

	void append(String const& string) {
		append(string.data(), string.size());
	}

	String& to_lower() {
		char diff = 'a' - 'A';

		char min = 'A';
		char max = 'Z';

		char* data = this->data();

		for (uint32_t i = 0; i < size(); ++i) {
			if (min <= data[i] && data[i] <= max) {
				data[i] += diff;
			}
		}

		return *this;
	}

	String& to_upper() {
		auto diff = 'a' - 'A';

		auto min = 'a';
		auto max = 'z';

		char* data = this->data();

		for (uint32_t i = 0; i < size(); ++i) {
			if (min <= data[i] && data[i] >= max) {
				data[i] -= diff;
			}
		}

		return *this;
	}
private:
	String(char const* text, std::size_t size) : sso(size < sso_capacity ? true : false) {
		if(sso) {
			std::memcpy(base.fake.str, text, size + 1);
			base.fake.size = size;
		} else {
			size_t capacity = string::next_capacity(size + 1);

			char* ptr = static_cast<char*>(std::malloc(capacity));

			if (ptr == nullptr) {
				throw std::runtime_error("std::malloc returned nullptr!");
			}

			std::memcpy(ptr, text, size + 1);

			base.real.ptr       = ptr;
			base.real.size      = size;
			base.real.capacity  = capacity;
		}
	}

	void swap(String& string) {
		std::swap(base, string.base);
		std::swap(sso,  string.sso);
	}

	union BaseString {
		struct RealString {
			char* ptr;
			uint32_t size;
			uint32_t capacity;
		} real;
		struct FakeString {
			char str[sizeof(RealString) - 1];
			uint8_t size;
		} fake;
	};

	BaseString base;
	bool       sso;

	// TODO In the future sso flag should be embedded in BaseString.

	static const size_t sso_capacity = sizeof(BaseString) - 1;
};

String operator+(String const& left, String const& right) {
	String string;

	string.append(left);
	string.append(right);

	return string;
}

bool operator==(String const& left, char const* right) {
	return !std::strcmp(left.data(), right);
}

bool operator==(char const* left, String const& right) {
	return right == left;
}

bool operator==(String const& left, String const& right) {
	if (left.size() != right.size()) { return false; }
	return !std::strcmp(left.data(), right.data());
}

bool operator!=(String const& left, char const* right) {
	return !(left == right);
}

bool operator!=(char const* left, String const& right) {
	return !(left == right);
}

bool operator!=(String const& left, String const& right) {
	return !(left == right);
}

bool operator<(String const& left, String const& right) {
	return std::strcmp(left.data(), right.data()) < 0;
}

bool operator<=(String const& left, String const& right) {
	return std::strcmp(left.data(), right.data()) <= 0;
}

bool operator>(String const& left, String const& right) {
	return std::strcmp(left.data(), right.data()) > 0;
}

bool operator>=(String const& left, String const& right) {
	return std::strcmp(left.data(), right.data()) >= 0;
}

std::ostream& operator<<(std::ostream& out, String const& string) {
	return out << string.data();
}

} // namespace simple
