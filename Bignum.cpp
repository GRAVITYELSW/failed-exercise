#include <iostream>
#include <cstring>
using namespace std;
class Bignum
{
private:
	int len;
	int arr[100];
public:
	friend istream& operator >> (istream& is, Bignum& a)
	{
		string s;
		is >> s;
		int i;
		a.len = s.length();
		for (i = 0; i < 100; i++)
		{
			a.arr[i] = 0;
		}
		for (i = 0; i < a.len; i++)
		{
			a.arr[i] = s[a.len - 1 - i] - '0';
		}
		return is;
	}
	friend ostream& operator << (ostream& os, const Bignum& a)
	{
		int i;
		for (i = a.len - 1; i >= 0; i--)
		{
			os << a.arr[i];
		}
		return os;
	}
	Bignum operator+(const Bignum& b)
	{
		Bignum result;
		
		int i, plus = 0, sum;
		for (i = 0; (i < len || i < b.len); i++)
		{
			sum = arr[i] + b.arr[i] + plus;
			result.arr[i] = sum % 10;
			plus = sum / 10;
		}
		if (plus != 0)
		{
			result.arr[i] = plus;
			result.len = i + 1;
		}
		else
			result.len = i;
		return result;
	}
};
int main()
{
	cout << "------------大数加法------------" << endl;
	cout << "请输入两个数，中间用空格隔开：" << endl;
	Bignum a;
	Bignum b;
	cin >> a >> b;
	cout << a + b << endl;
	return 0;
}