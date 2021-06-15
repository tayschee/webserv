#include <algorithm>
#include <string>

int main()
{
	std::string test("abc\n");

	std::cout << std::find(test.begin(), test.end(), '3') << "\n";
	return 0;
}