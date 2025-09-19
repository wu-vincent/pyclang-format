#include <clang/Format/Format.h>
#include <format>
#include <pybind11/pybind11.h>

namespace py = pybind11;

static std::string format(const std::string &code,
                          const std::string &style = clang::format::DefaultFormatStyle,
                          const std::string &fallback_style = clang::format::DefaultFallbackStyle,
                          const std::optional<std::string> &assume_filename = std::nullopt,
                          const bool error_on_incomplete_format = false) {
    using namespace llvm;
    using namespace clang;

    std::unique_ptr<MemoryBuffer> buf
        = MemoryBuffer::getMemBufferCopy(StringRef(code.data(), code.size()));
    if (buf->getBufferSize() == 0) {
        return code;
    }

    std::vector<tooling::Range> Ranges;
    Ranges.emplace_back(0, buf->getBufferSize());

    StringRef assumed_filename = assume_filename.value_or("<stdin>");
    auto format_style = format::getStyle(style,
                                         assumed_filename,
                                         fallback_style,
                                         buf->getBuffer(),
                                         nullptr,
                                         /*AllowUnknownOptions*/ false);
    if (!format_style) {
        throw std::runtime_error(toString(format_style.takeError()));
    }

    bool incomplete_format;
    auto replacements
        = reformat(*format_style, buf->getBuffer(), Ranges, assumed_filename, &incomplete_format);

    if (incomplete_format && error_on_incomplete_format) {
        throw std::runtime_error("Incomplete format due to a non-recoverable syntax error");
    }

    auto result = applyAllReplacements(buf->getBuffer(), replacements);
    if (!result) {
        throw std::runtime_error(toString(result.takeError()));
    }

    return std::move(result.get());
}

PYBIND11_MODULE(example, m, py::mod_gil_not_used()) {
    m.def("format",
          &format,
          py::arg("code"),
          py::arg("style") = clang::format::DefaultFormatStyle,
          py::arg("fallback_style") = clang::format::DefaultFallbackStyle,
          py::arg("assume_filename") = std::nullopt,
          py::arg("error_on_incomplete_format") = false);
}
