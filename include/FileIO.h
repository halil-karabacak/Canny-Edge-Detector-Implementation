#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstddef>


namespace CV {
	namespace Utils {
		struct RGB {
			unsigned char r, g, b;
		};

		class Image {
		public:
			static std::shared_ptr<Image> loadPNG(std::string file_path);

			int width, height;
			std::vector<RGB> rgbData;
		};
	}
}
