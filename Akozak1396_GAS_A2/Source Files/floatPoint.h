
struct floatPOINT
{
	float x;
	float y;

	floatPOINT operator +(const floatPOINT& a) {
		floatPOINT temp;
		temp.x = x + a.x;
		temp.y = y + a.y;
		return temp;
	}
};