#include <string>

#include "auth_request.h"

#include "envoy/registry/registry.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class HttpAuthDecoderFilterConfig : public NamedHttpFilterConfigFactory {
public:
  HttpFilterFactoryCb createFilterFactory(const Json::Object&, const std::string&,
                                          FactoryContext& context) override {
    return [&context](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(
          Http::StreamDecoderFilterSharedPtr{new Http::HttpAuthDecoderFilter(context)});
    };
  }
  std::string name() override { return "auth_request"; }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<HttpAuthDecoderFilterConfig, NamedHttpFilterConfigFactory> register_;

} // Configuration
} // Server
} // Envoy
