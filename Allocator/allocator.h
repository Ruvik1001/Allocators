#pragma once

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <memory>

namespace MyAllocator {

	template <typename Type>
	class Allocator
	{
	public:
		typedef Type value_type;
		typedef Type* pointer_type;
		typedef Type& reference_type; 
		typedef const Type const_value_type;
		typedef const Type* const_pointer_type;
		typedef const Type& const_reference_type;
		typedef char* memory;

	private:
		Allocator() = delete;

		memory mem = nullptr;
		size_t count_obj = 0;
		size_t size_alloc = 0;
		bool* created;
	public:
		Allocator(size_t);
		~Allocator();

		template <typename ... Args>
		pointer_type construct(Args ...);
		
		void destroy(pointer_type);
		void deallocate(pointer_type, size_t);

		pointer_type at(size_t);
	};

	template<typename Type>
	inline Allocator<Type>::Allocator(size_t count) : size_alloc(count) {
		mem = (char*)malloc(sizeof(Type) * count);
		if (mem) {
			created = new bool[count];
			for (size_t i = 0; i < count; i++)
				created[i] = false;
		}
		else
			size_alloc = 0;
	}

	template<typename Type>
	inline Allocator<Type>::~Allocator() {
		for (size_t i = 0; i < size_alloc; i++)
			if (created[i])
				(((pointer_type)mem) + i)->~value_type();

		delete[] created;
		free((void*)mem);
	}

	template<typename Type>
	inline void MyAllocator::Allocator<Type>::destroy(pointer_type ptr) {
		size_t num = ptr - (pointer_type)mem;
		if (num < size_alloc && created[num]) {
			created[num] = false;
			count_obj--;
			ptr->~value_type();
		}
	}

	template<typename Type>
	inline void MyAllocator::Allocator<Type>::deallocate(pointer_type ptr, size_t n) {
		while (n--)
			destroy(ptr++);
	}

	template<typename Type>
	typename Allocator<Type>::pointer_type MyAllocator::Allocator<Type>::at(size_t n) {
		return (n < size_alloc && created[n]) ? (((pointer_type)mem) + n) : nullptr;
	}

	template<typename Type>
	template<typename ...Args>
	typename Allocator<Type>::pointer_type MyAllocator::Allocator<Type>::construct(Args ... arg) {
		pointer_type ptr = nullptr;
		for (size_t i = 0; i < size_alloc; i++) 
			if (!created[i]) {
				count_obj++;
				created[i] = true;
				ptr = new(((pointer_type)mem) + i) value_type(arg...);
				break;
			}
		return ptr;
	}
}

#endif // !__ALLOCATOR_H__
