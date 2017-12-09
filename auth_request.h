#pragma once

#include <string>
#include <condition_variable>

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

class HttpAuthDecoderFilter : public StreamDecoderFilter, public AsyncClient::StreamCallbacks {
public:
  HttpAuthDecoderFilter(Envoy::Server::Configuration::FactoryContext& context, std::string& upstream);
  ~HttpAuthDecoderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap& headers, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  FilterTrailersStatus decodeTrailers(HeaderMap&) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override;

  // Http::AsyncClient::StreamCallbacks
  void onHeaders(Http::HeaderMapPtr&& headers, bool end_stream) override;
  void onData(Buffer::Instance& data, bool end_stream) override;
  void onTrailers(Http::HeaderMapPtr&& trailers) override;
  void onReset() override;

private:
  StreamDecoderFilterCallbacks* decoder_callbacks_;
  Upstream::ClusterManager* cm_;
  HeaderMap* headers_;
  HeaderMapPtr* response_headers_;
  Buffer::Instance* data_;
  std::string* upstream_name_;
  std::list<LowerCaseString*> temp_keys_;
  std::list<std::string*> temp_values_;
  uint64_t auth_status_ = 0;
};

} // Http
} // Envoy