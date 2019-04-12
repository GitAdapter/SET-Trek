/*
*  FILE          : FloatPoint.h
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file create a floatPOINT class which contains two floats (x and y)
*	 which can be added together and compared. 
*/

#define F_POINT

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

	bool operator !=(const floatPOINT& a) {
		return !(this->x == a.x && this->y == a.y);
	}
};