#ifndef __HTTPIMAGERETRIEVER_HH
#define __HTTPIMAGERETRIEVER_HH

#include <iostream>
#include <memory>

#include "../net/curl.hh"
#include "imageretriever.hh"

namespace imageretriever {
class HttpImageRetriever : public ImageRetriever {
   public:
    using ImageRetriever::ImageRetriever;

    screens::Image retrieve() override;

    void set_url(const std::string& url) {
        http = std::make_unique<net::Http>(url);
    }

   protected:
    net::Data get_image() const;

   private:
    std::unique_ptr<net::Http> http{};
};
}  // namespace imageretriever

#endif
