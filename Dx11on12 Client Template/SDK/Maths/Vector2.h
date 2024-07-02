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

	Vector2(T x = 0, T y = 0)
	{
		this->x = x; this->y = y;
	};

	__forceinline Vector2<T> invert() {
		return Vector2<T>(this->y, this->x);
	}

	__forceinline Vector2<T> lerp(Vector2<T> target, float pct) {
		return Vector2<T>((this->x + (target.x - this->x) * pct), (this->y + (target.y - this->y) * pct));
	}

	Vector2<int> ToInt()
	{
		return Vector2<int>(x, y);
	}

	Vector2<float> toFloat()
	{
		return Vector2<float>(x, y);
	}

	Vector2<T> toType()
	{
		return Vector2<T>(x, y);
	}

	float magnitude()
	{
		return std::sqrt(x * x + y * y);
	}

	Vector2<short> toShort()
	{
		return Vector2<short>(x, y);
	}

	Vector2<T> normalize()
	{
		T magnitude = std::sqrt(this->x * this->x + this->y * this->y);
		return (magnitude != 0) ? Vector2<T>(this->x / magnitude, this->y / magnitude) : Vector2<T>(0, 0);
	}

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
};