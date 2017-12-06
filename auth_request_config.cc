#include <string>

#include "auth_request.h"

#include "envoy/registry/registry.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class HttpAuthDecoderFilterConfig : public NamedHttpFilterConfigFactory, Logger::Loggable<Logger::Id::http> {
public:
  HttpFilterFactoryCb createFilterFactory(const Json::Object& config, const std::string&,
                                          FactoryContext& context) override {
    upstream_ = config.getString("upstream");
    ENVOY_LOG(info, "auth request module loaded upstream: {}", upstream_);
    return [&](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(
          Http::StreamDecoderFilterSharedPtr{new Http::HttpAuthDecoderFilter(context, upstream_)});
    };
  }
  std::string name() override { return "auth_request"; }
  private:
    std::string upstream_;
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<HttpAuthDecoderFilterConfig, NamedHttpFilterConfigFactory> register_;

} // Configuration
} // Server
} // Envoy
