#include <iostream>
#include <string>
#include <format>
#include "nlohmann/json.h"
#include "libexecstream/exec-stream.h"

#ifdef _WIN32

#include <io.h>
#include <fcntl.h>

#endif

inline void init() {
#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
#endif
}

class NativeMessage {
private:
    std::uint32_t length{};
    std::string data{};
public:
    explicit NativeMessage(std::istream &is) {
        is >> *this;
    }

    explicit NativeMessage(const std::string &data) : length(data.size()), data(data) {
    }

    [[nodiscard]] uint32_t getLength() const {
        return length;
    }

    [[nodiscard]] const std::string &getData() const {
        return data;
    }

    friend std::istream &operator>>(std::istream &is, NativeMessage &message) {
        is.read(reinterpret_cast<char *>(&message.length), sizeof(message.length));

        message.data.resize(message.length);
        is.read(&message.data[0], message.length);

        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const NativeMessage &message) {
        os.write(reinterpret_cast<const char *>(&message.length), sizeof(message.length));
        os.write(message.data.data(), message.length);
        return os;
    }
};

int main() {
    init();

    auto response = NativeMessage(R"({"error": "Unexpected Error"})");

    try {
        // Read Message
        auto message = NativeMessage(std::cin);

        auto json = nlohmann::json::parse(message.getData());
        auto doc = json["doc"].template get<std::string>();

        // Call typstfmt Executable
        auto es = exec_stream_t("typstfmt", "--output -");

        // Write to typstfmt
        es.in() << doc;
        es.close_in();

        // Read from typstfmt stdout
        auto fmt_doc_buf = std::stringstream{};
        es.out() >> fmt_doc_buf.rdbuf();
        auto fmt_doc = fmt_doc_buf.str();

        // Read from typstfmt stderr
        auto fmt_err_buf = std::stringstream{};
        es.err() >> fmt_err_buf.rdbuf();
        auto fmt_err = fmt_err_buf.str();

        es.close();

        // Check Exception
        if (!fmt_err.empty()) {
            throw std::runtime_error(fmt_err);
        }

        // Send Response
        auto fmt_doc_json = nlohmann::json{{"doc", fmt_doc}};
        response = NativeMessage(fmt_doc_json.dump());
    } catch (std::exception &e) {
        response = NativeMessage(std::format(R"({{"error": "{}"}})", e.what()));
    } catch (...) {
        response = NativeMessage(R"({"error": "Unknown Error"})");
    }

    std::cout << response;

    return 0;
}
