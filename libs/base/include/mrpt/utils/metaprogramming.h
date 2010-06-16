/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2010  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */
#ifndef metaprogramming_H
#define metaprogramming_H

#include <mrpt/utils/CSerializable.h>

namespace mrpt
{
	namespace utils
	{
		/** A set of utility objects for metaprogramming with STL algorithms.
		  */
		namespace metaprogramming
		{
			/** An object for deleting pointers (intended for STL algorithms) */
			struct ObjectDelete {
				template<typename T>
				inline void operator()(const T *ptr) {
					delete ptr;
				}
			};

			/** A function which deletes a container of pointers. */
			template<typename T> inline void DeleteContainer(T &container)	{
				for_each(container.begin(),container.end(),ObjectDelete());
			}

			//NOTE: when using this algorithm with for_each, replace the whole line with:
			//	for_each(bypassPointer(<beginning iterator>),bypassPointer(<ending iterator>),mem_fun_ref(&<NonPointerBaseClass>::clear)).
			/** An object for clearing an object (invokes its method "->clear()") given a pointer or smart-pointer, intended for being used in STL algorithms. */
			struct ObjectClear {
				template<typename T>
				inline void operator()(T &ptr) {
					ptr->clear();
				}
			};

			//NOTE: replace this with mem_fun_ref(&<BaseClass>::clear).
			/** An object for clearing an object (invokes its method ".clear()") given a pointer or smart-pointer, intended for being used in STL algorithms. */
			struct ObjectClear2 {
				template<typename T>
				inline void operator()(T &ptr){
					ptr.clear();
				}
			};

			/** An object for clearing an object->second (invokes its method "clear()") given a pointer or smart-pointer, intended for being used in STL algorithms. */
			struct ObjectClearSecond {
				template<typename T>
				inline void operator()(T obj) {
					obj.second.clear();
				}
			};

			/** An object for transforming between types/classes, intended for being used in STL algorithms.
			  *  Example of usage:
			  *   \code
			  *     vector_int      v1(10);  // Input
			  *     vector_double   v2(10);  // Output
			  *     std::transform(v1.begin(),v1.end(), v2.begin(), ObjectConvert<double> );
			  *   \endcode
			  */
			template <typename TARGET_TYPE>
			struct ObjectConvert {
				template<typename T>
				inline TARGET_TYPE operator()(const T &val) {
					return TARGET_TYPE(val);
				}
			};

			//NOTE: replace with mem_fun_ref(&CSerializable::make_unique)
			/** An object for making smart pointers unique (ie, making copies if necessary), intended for being used in STL algorithms. */
			struct ObjectMakeUnique {
				typedef mrpt::utils::CObjectPtr argument_type;
				typedef void result_type;
				inline void operator()(mrpt::utils::CObjectPtr &ptr) {
					ptr.make_unique();
				}
			};

			/** An object for making smart pointers unique (ie, making copies if necessary), intended for being used in STL algorithms. */
			struct ObjectPairMakeUnique {
				template <typename T>
				inline void operator()(T &ptr) {
					ptr.first.make_unique();
					ptr.second.make_unique();
				}
			};

			//NOTE: replace with mem_fun_ref(&CSerializable::clear_unique)
			/** An object for making smart pointers unique (ie, making copies if necessary), intended for being used in STL algorithms. */
			struct ObjectClearUnique {
				typedef mrpt::utils::CObjectPtr argument_type;
				typedef void result_type;
				inline void operator()(mrpt::utils::CObjectPtr &ptr)
				{
					ptr.clear_unique();
				}
			};

			/** An object for reading objects from a stream, intended for being used in STL algorithms. */
			struct ObjectReadFromStream
			{
			private:
				CStream		*m_stream;
			public:
				inline ObjectReadFromStream(CStream *stream) : m_stream(stream) {  }

				// T can be CSerializablePtr, CSerializable, or any other class implementing ">>"
				template <typename T>
				inline void operator()(T &obj)
				{
					(*m_stream) >> obj;
				}
			};

			/** An object for writing objects to a stream, intended for being used in STL algorithms. */
			struct ObjectWriteToStream
			{
			private:
				CStream		*m_stream;
			public:
				inline ObjectWriteToStream(CStream *stream) : m_stream(stream) {  }

				// T can be CSerializablePtr, CSerializable, or any other class implementing "<<"
				template <typename T>
				inline void operator()(const T &ptr)
				{
					(*m_stream) << ptr;
				}
			};

			/** Behaves like std::copy but allows the source and target iterators to be of different types through static typecasting.
			  * \note As in std::copy, the target iterator must point to the first "slot" where to put the first transformed element, and sufficient space must be allocated in advance.
			  * \sa copy_container_typecasting
			  */
			template<typename it_src, typename it_dst>
			inline void copy_typecasting(it_src  first, it_src last,it_dst  target)
			{
				for (it_src i=first; i!=last ; ++i,++target)
					*target = static_cast<typename it_dst::value_type>(*i);
			}

			/** Copy all the elements in a container (vector, deque, list) into a different one performing the appropriate typecasting.
			  *  The target container is automatically resized to the appropriate size, and previous contents are lost.
			  *  This can be used to assign std::vector's of different types:
			  * \code
			  *   std::vector<int>    vi(10);
			  *   std::vector<float>  vf;
			  *   vf = vi;   // Compiler error
			  *   mrpt::utils::metaprogramming::copy_container_typecasting(v1,vf);  // Ok
			  * \endcode
			  */
			template<typename src_container, typename dst_container>
			inline void copy_container_typecasting(const src_container &src, dst_container &trg)
			{
				trg.resize( src.size() );
				typename src_container::const_iterator i = src.begin();
				typename src_container::const_iterator last = src.end();
				typename dst_container::iterator target = trg.begin();
				for ( ; i!=last ; ++i,++target)
					*target = static_cast<typename dst_container::value_type>(*i);
			}

			/** This class bypasses pointer access in iterators to pointers, thus allowing
			  * the use of algorithms that expect an object of class T with containers of T*.
			  * Although it may be used directly, use the bypassPointer function for better
			  * results and readability (since it most probably won't require template
			  * arguments).
			  */
			template<typename T,typename U> class MemoryBypasserIterator	{
			private:
				T baseIterator;
			public:
				typedef typename T::iterator_category iterator_category;
				typedef U value_type;
				typedef typename T::difference_type difference_type;
				typedef U *pointer;
				typedef U &reference;
				inline MemoryBypasserIterator(const T &bi):baseIterator(bi)	{}
				inline reference operator*()	{
					return *(*baseIterator);
				}
				inline MemoryBypasserIterator<T,U> &operator++()	{
					++baseIterator;
					return *this;
				}
				inline MemoryBypasserIterator<T,U> operator++(int)	{
					MemoryBypasserIterator it=*this;
					++baseIterator;
					return it;
				}
				inline MemoryBypasserIterator<T,U> &operator--()	{
					--baseIterator;
					return *this;
				}
				inline MemoryBypasserIterator<T,U> operator--(int)	{
					MemoryBypasserIterator it=*this;
					--baseIterator;
					return *this;
				}
				inline MemoryBypasserIterator<T,U> &operator+=(difference_type off)	{
					baseIterator+=off;
					return *this;
				}
				inline MemoryBypasserIterator<T,U> operator+(difference_type off) const	{
					return (MemoryBypasserIterator<T,U>(*this))+=off;
				}
				inline MemoryBypasserIterator<T,U> &operator-=(difference_type off)	{
					baseIterator-=off;
					return *this;
				}
				inline MemoryBypasserIterator<T,U> operator-(difference_type off) const	{
					return (MemoryBypasserIterator<T,U>(*this))-=off;
				}
				inline difference_type operator-(const MemoryBypasserIterator<T,U> &it) const	{
					return baseIterator-it.baseIterator;
				}
				inline reference operator[](difference_type off) const	{
					return *(this+off);
				}
				inline bool operator==(const MemoryBypasserIterator<T,U> &i) const	{
					return baseIterator==i.baseIterator;
				}
				inline bool operator!=(const MemoryBypasserIterator<T,U> &i) const	{
					return baseIterator!=i.baseIterator;
				}
				inline bool operator<(const MemoryBypasserIterator<T,U> &i) const	{
					return baseIterator<i.baseIterator;
				}
			};

			/** Sintactic sugar for MemoryBypasserIterator.
			  * For example, having the following declarations:
			  *		vector<double *> vec;
			  *		void modifyVal(double &v);
			  * The following sentence is not legal:
			  *		for_each(vec.begin(),vec.end(),&modifyVal)
			  * But this one is:
			  *		for_each(bypassPointer(vec.begin()),bypassPointer(vec.end()),&modifyVal)
			  */
			template<typename U,typename T> inline MemoryBypasserIterator<T,U> bypassPointer(const T &baseIterator)	{
				return MemoryBypasserIterator<T,U>(baseIterator);
			}

			/** This template encapsulates a binary member function and a single object into a
			  * function expecting the two parameters of the member function.
			  * Don't use directly. Use the wrapMember function instead to avoid explicit
			  * template instantiation.
			  */
			template<typename T,typename U1,typename U2,typename V> class BinaryMemberFunctionWrapper	{
			private:
				typedef T (V::*MemberFunction)(U1,U2);
				V &obj;
				MemberFunction func;
			public:
				typedef U1 first_argument_type;
				typedef U2 second_argument_type;
				typedef T result_type;
				inline BinaryMemberFunctionWrapper(V &o,MemberFunction f):obj(o),func(f)	{}
				inline T operator()(U1 p1,U2 p2)	{
					return (obj.*func)(p1,p2);
				}
			};
			/** This template encapsulates an unary member function and a single object into a
			  * function expecting the parameter of the member function.
			  * Don't use directly. Use the wrapMember function instead.
			  */
			template<typename T,typename U,typename V> class UnaryMemberFunctionWrapper	{
			private:
				typedef T (V::*MemberFunction)(U);
				V &obj;
				MemberFunction func;
			public:
				typedef U argument_type;
				typedef T result_type;
				inline UnaryMemberFunctionWrapper(V &o,MemberFunction f):obj(o),func(f)	{}
				inline T operator()(U p)	{
					return (obj.*func)(p);
				}
			};
			/** This template encapsulates a member function without arguments and a single
			  * object into a function.
			  * Don't use directly. Use the wrapMember function instead.
			  */
			template<typename T,typename V> class MemberFunctionWrapper	{
			private:
				typedef T (V::*MemberFunction)(void);
				V &obj;
				MemberFunction func;
			public:
				inline MemberFunctionWrapper(V &o,MemberFunction f):obj(o),func(f)	{}
				inline T operator()()	{
					return (obj.*func)();
				}
			};
			/** This function creates a function from an object and a member function.
			  * It has three overloads, for zero, one and two parameters in the function.
			  */
			template<typename T,typename U1,typename U2,typename V> inline BinaryMemberFunctionWrapper<T,U1,U2,V> wrapMember(V &obj,T (V::*fun)(U1,U2))	{
				return BinaryMemberFunctionWrapper<T,U1,U2,V>(obj,fun);
			}
			template<typename T,typename U,typename V> inline UnaryMemberFunctionWrapper<T,U,V> wrapMember(V &obj,T (V::*fun)(U))	{
				return UnaryMemberFunctionWrapper<T,U,V>(obj,fun);
			}
			template<typename T,typename V> inline MemberFunctionWrapper<T,V> wrapMember(V &obj,T (V::*fun)(void))	{
				return MemberFunctionWrapper<T,V>(obj,fun);
			}

			/** Equivalent of std::bind1st for functions with non-const arguments.
			  */
			template<typename Op> class NonConstBind1st	{
			private:
				Op &op;
				typename Op::first_argument_type &val;
			public:
				typedef typename Op::second_argument_type argument_type;
				typedef typename Op::result_type result_type;
				NonConstBind1st(Op &o,typename Op::first_argument_type &t):op(o),val(t)	{}
				inline result_type operator()(argument_type &s)	{
					return op(val,s);
				}
			};
			/** Use this function instead of directly calling NonConstBind1st.
			  */
			template<typename Op> inline NonConstBind1st<Op> nonConstBind1st(Op &o,typename Op::first_argument_type &t)	{
				return NonConstBind1st<Op>(o,t);
			}
			/** Equivalent of std::bind2nd for functions with non-const arguments.
			  */
			template<typename Op> class NonConstBind2nd	{
			private:
				Op &op;
				typename Op::second_argument_type &val;
			public:
				typedef typename Op::first_argument_type argument_type;
				typedef typename Op::result_type result_type;
				NonConstBind2nd(Op &o,typename Op::second_argument_type &t):op(o),val(t)	{}
				inline result_type operator()(argument_type &f)	{
					return op(f,val);
				}
			};
			/** Do not directly use the NonConstBind2nd class directly. Use this function.
			  */
			template<typename Op> inline NonConstBind2nd<Op> nonConstBind2nd(Op &o,typename Op::second_argument_type &t)	{
				return NonConstBind2nd<Op>(o,t);
			}

		} // end metaprogramming
	} // End of namespace
} // end of namespace
#endif