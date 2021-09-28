#include "color.h"
#include "vec3.h"
#include <iostream>

int main()
{

    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // 행	: 위에서 아래로
    // 픽셀	: 왼쪽에서 오른쪽
    for (int j = image_height - 1; j >= 0; --j)
    {
        /*
    ** 	\r 캐리지 리턴 : 문자의 새줄을 시작(맨앞으로 이동)
    ** 	std::flush : os.flush() 호출한 것처럼 os 의 출력 시퀀스를 비운다.
    */
        std::cerr << "\rScanlines remaing: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
            write_color(std::cout, color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25));
    }

    std::cerr << "\ndone.\n";
}