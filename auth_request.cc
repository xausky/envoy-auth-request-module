#include <string>
#include <thread>

#include "auth_request.h"

#include "server/config/network/http_connection_manager.h"

#include "common/http/async_client_impl.h"

namespace Envoy {
namespace Http {

HttpAuthDecoderFilter::HttpAuthDecoderFilter(Envoy::Server::Configuration::FactoryContext& context,std::string& upstream) {
  cm_ = &context.clusterManager();
  data_ = new Buffer::OwnedImpl();
  upstream_name_ = &upstream;
}

HttpAuthDecoderFilter::~HttpAuthDecoderFilter() {
  delete data_;
  std::list<LowerCaseString*>::iterator key;
  for (key = temp_keys_.begin(); key != temp_keys_.end(); ++key){
    delete *key;
  }
  std::list<std::string*>::iterator value;
  for (value = temp_values_.begin(); value != temp_values_.end(); ++value){
    delete *value;
  }
  delete response_headers_;
}

void HttpAuthDecoderFilter::onDestroy() {}

FilterHeadersStatus HttpAuthDecoderFilter::decodeHeaders(HeaderMap& headers, bool) {
  headers_ = &headers;
  AsyncClient& clent = cm_->httpAsyncClientForCluster(*upstream_name_);
  AsyncClient::Stream* stream = clent.start(*this, Optional<std::chrono::milliseconds>(std::chrono::milliseconds(1000)), false);
  stream->sendHeaders(headers, false);
  return FilterHeadersStatus::StopIteration;
}

FilterDataStatus HttpAuthDecoderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus HttpAuthDecoderFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void HttpAuthDecoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

void HttpAuthDecoderFilter::onHeaders(Http::HeaderMapPtr&& headers, bool){
  auth_status_ = Utility::getResponseStatus(*headers);
  response_headers_ = new HeaderMapPtr(new HeaderMapImpl(*headers));
}

void HttpAuthDecoderFilter::onData(Buffer::Instance& data, bool end_stream){
  data_->add(data);
  if(!end_stream){
    return;
  }
  if(auth_status_ / 100 == 2){
    std::string body;
    uint64_t num_slices = data_->getRawSlices(nullptr, 0);
    Buffer::RawSlice slices[num_slices];
    data_->getRawSlices(slices, num_slices);
    for (Buffer::RawSlice& slice : slices) {
      body.append(static_cast<const char*>(slice.mem_), slice.len_);
    }
    try {
      Json::ObjectSharedPtr json_body = Json::Factory::loadFromString(body);
      json_body->iterate(
        [=](const std::string& key, const Json::Object& value){
          LowerCaseString *keyString = new LowerCaseString(key);
          std::string *valueString = new std::string(value.asString());
          headers_->addReference(*keyString, *valueString);
          temp_keys_.push_back(keyString);
          temp_values_.push_back(valueString);
          return true;
        }
      );
    } catch (const Json::Exception& jsonEx) {
      ENVOY_LOG_MISC(info, "json exception: {}", body);
    }
    decoder_callbacks_->continueDecoding();
  }else{
    decoder_callbacks_->encodeHeaders(std::move(*response_headers_),false);
    decoder_callbacks_->encodeData(*data_, true);
  }
}
void HttpAuthDecoderFilter::onTrailers(Http::HeaderMapPtr&&){
}
void HttpAuthDecoderFilter::onReset(){
}

} // Http
} // Envoy
