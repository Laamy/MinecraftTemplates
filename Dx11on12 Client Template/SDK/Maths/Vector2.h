#pragma once

template <typename T>
struct Vector2
{
	union
	{
		struct
		{
			T x, y;
		};
		T arr[2];
	};

	/**
	 * @brief Default constructor
	 */
	Vector2(T x = 0, T y = 0)
	{
		this->x = x; this->y = y;
	};

	/**
	 * @brief Lerp between two vectors
	 * @param target The target vector
	 * @param pct The percentage to lerp
	 * @return The lerped vector
	 */
	__forceinline Vector2<T> lerp(Vector2<T> target, float pct) {
		return Vector2<T>((this->x + (target.x - this->x) * pct), (this->y + (target.y - this->y) * pct));
	}

	Vector2<T> Add(Vector2<T> other)
	{
		return Vector2<T>(x + other.x, y + other.y);
	}

	/**
	 * @brief Cast to T
	 * @return The casted vector
	 */
	Vector2<T> toType()
	{
		return Vector2<T>(x, y);
	}

	/**
	 * @brief Create Vector3<T> using current Vector2<T>
	 * @return The new vector
	 */
	//Vector3<T> Vec3()
	//{
	//	return Vector3<T>(x, y, x);
	//}

	/**
	 * @brief Get the magnitude of the vector
	 * @return The magnitude of the vector
	 */
	float magnitude()
	{
		return std::sqrt(x * x + y * y);
	}

	/**
	 * @brief Normalize the vector
	 * @return The normalized vector
	 */
	Vector2<T> normalize()
	{
		T magnitude = std::sqrt(this->x * this->x + this->y * this->y);
		return (magnitude != 0) ? Vector2<T>(this->x / magnitude, this->y / magnitude) : Vector2<T>(0, 0);
	}

#pragma region Operators

	Vector2 operator* (T scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 operator/ (T scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	bool operator == (Vector2 v)
	{
		return v.x == x && v.y == y;
	};

	Vector2 operator + (Vector2 v) const
	{
		return { x + v.x, y + v.y };
	}

	Vector2 operator - (Vector2 v) const
	{
		return { x - v.x, y - v.y };
	}

	bool operator != (Vector2 v)
	{
		return v.x != x || v.y != y;
	};

	Vector2 operator *= (Vector2 v)
	{
		return Vector2(v.x * x, v.y * y);
	};

	Vector2 operator /= (Vector2 v)
	{
		return Vector2(v.x / x, v.y / y);
	};

#pragma endregion
};