#pragma once
#include <memory>

/// Represents an immutable value container
/// for types that should be made nullable.
///
/// \ingroup utilities
template<class T>
class Nullable {
	std::tr1::shared_ptr<T> value_;

public:
	/// Initializes an empty container.
	Nullable()
	{
	}

	Nullable(const T& value)
		: value_(new T(value))
	{
	}

	/// Returns the attached value.
	///
	/// \return Attached value.
	T& GetValue()
	{
		if (!HasValue())
			throw std::runtime_error("No value set");

		return *value_;
	}

	/// Returns the attached value.
	///
	/// \return Attached value.
	const T& GetValue() const
	{
		if (!HasValue())
			throw std::runtime_error("No value set");

		return *value_;
	}

	/// Returns the attached value or if it's not available \a dflt.
	///
	/// \return Attached value or \a dflt.
	const T& GetValueOrDefault(const T& dflt = T()) const
	{
		return value_.get() ? *value_ : dflt;
	}

	/// Gets a value indicating whether a value has been attached.
	///
	/// \return \c true if a value is available, \c false otherwise.
	bool HasValue() const
	{
		return !!value_.get();
	}

	static const Nullable<int> Empty()
	{
		return Nullable<int>();
	}
};

template<class T, class U>
inline bool operator==(const Nullable<T>& left, const Nullable<U>& right)
{
	return left.HasValue() && right.HasValue() && left.GetValue() == right.GetValue();
}

template<class T, class U>
inline bool operator!=(const Nullable<T>& left, const Nullable<U>& right)
{
	return !(left == right);
}

template<class T, class U>
inline bool operator==(const Nullable<T>& left, const U& right)
{
	return left.HasValue() && left.GetValue() == right;
}

template<class T, class U>
inline bool operator==(const U& left, const Nullable<T>& right)
{
	return right == left;
}

template<class T, class U>
inline bool operator!=(const Nullable<T>& left, const U& right)
{
	return !(left == right);
}

template<class T, class U>
inline bool operator!=(const U& left, const Nullable<T>& right)
{
	return !(left == right);
}
