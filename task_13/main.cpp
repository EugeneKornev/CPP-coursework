#include <cassert>
#include <type_traits>
#include <utility>


template <typename Checker, typename... Args> requires (std::is_invocable_r_v<bool, Checker, Args> && ...)
int getIndexOfFirstMatch(Checker&& check, Args... args) {
	int index = 0;
	int final_index = -1;
	bool found = false;
	auto lambda = [&](auto&& arg) -> void {
			if (found) {
				return;
			}
			if (check(std::forward<decltype(arg)>(arg))) {
				found = true;
				final_index = index;
			}
			index++;
	};
	(lambda(std::forward<Args>(args)), ...);
	return final_index;
}

struct Point {
	float x;
	float y;
};

int main() {
	auto checker = [](auto&& arg) -> bool {
		using T = std::remove_cvref_t<decltype(arg)>;
		if constexpr (std::is_integral_v<T>) {
			return arg % 2 == 0;
		} else {
			return false;
		}
	};
	assert(getIndexOfFirstMatch(checker, 12.654f, Point{1234.5678f, 8765.4321f}, "Point", 123, 124, 47.849) == 4);
}