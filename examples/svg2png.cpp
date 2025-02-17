#include <lunasvg.h>

#include <iostream>
#include <sstream>

using namespace lunasvg;

int help()
{
    std::cout << "Usage: \n"
                 "   svg2png [filename] [resolution] [bgColor]\n\n"
                 "Examples: \n"
                 "    $ svg2png input.svg\n"
                 "    $ svg2png input.svg 512x512\n"
                 "    $ svg2png input.svg 512x512 0xff00ffff\n\n";
    return 1;
}

bool setup(int argc, char** argv, std::string& filename, std::uint32_t& width, std::uint32_t& height, std::uint32_t& bgColor)
{
    if(argc > 1) filename.assign(argv[1]);
    if(argc > 2) {
        std::stringstream ss;
        ss << argv[2];
        ss >> width;

        if(ss.fail() || ss.get() != 'x')
            return false;
        ss >> height;
    }

    if(argc > 3) {
        std::stringstream ss;
        ss << std::hex << argv[3];
        ss >> std::hex >> bgColor;
    }

    return argc > 1;
}

int main(int argc, char** argv)
{
    std::string filename;
    std::uint32_t width = 0, height = 0;
    std::uint32_t bgColor = 0x00000000;
    if(!setup(argc, argv, filename, width, height, bgColor)) {
        return help();
    }

    auto document = Document::loadFromFile(filename);
    if(document == nullptr) {
        return help();
    }

    auto bitmap = document->renderToBitmap(width, height, bgColor);
    if(bitmap.isNull()) {
        return help();
    }

    auto lastSlashIndex = filename.find_last_of("/\\");
    auto basename = lastSlashIndex == std::string::npos ? filename : filename.substr(lastSlashIndex + 1);
    basename.append(".png");

    bitmap.writeToPng(basename);
    std::cout << "Generated PNG file : " << basename << std::endl;
    return 0;
}
