#pragma once

#include <sigmafive/resource2/package.hpp>

namespace sigmafive {
    namespace resource2 {
        class filesystem_package : public package {
        public:
            filesystem_package() = default;

            filesystem_package(const filesystem_package &) = delete;

            filesystem_package &operator=(const filesystem_package &) = delete;

            virtual bool contains(identifier id) const override;

            virtual std::unique_ptr<std::istream> get_stream(identifier id) override;
        };
    }
}