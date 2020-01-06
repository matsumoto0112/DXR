#include "ByteReader.h"
#include "Utility/Debug.h"

namespace Framework::Utility {
    //ÉfÅ[É^ÇÃì«Ç›çûÇ›
    std::vector<BYTE> ByteReader::read(const std::filesystem::path& path) {
        std::ifstream ifs(path.string().c_str(), std::ios::binary | std::ios::in);
        MY_THROW_IF_FALSE(!!ifs);

        ifs.seekg(0, std::ios::end);
        std::vector<BYTE> res(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        ifs.read((char*)res.data(), res.size());

        return res;
    }
} // namespace Framework::Utility
