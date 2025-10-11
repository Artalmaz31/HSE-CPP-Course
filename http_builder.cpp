#include "http_builder.h"

std::string LowerString(std::string_view str) {
    std::string result;
    for (char c : str) {
        result += std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}

std::string_view HttpHeaderCollection::GetValue(std::string_view name) const {
    std::string name_lowercase = LowerString(name);
    return storage_.at(name_lowercase).value;
}

void HttpHeaderCollection::Set(std::string_view name, std::string_view value) {
    std::string name_lowercase = LowerString(name);
    if (Contains(name_lowercase)) {
        storage_.at(name_lowercase).name = name;
        storage_.at(name_lowercase).value = value;
    } else {
        storage_[name_lowercase] = HttpHeader{std::string(name), std::string(value)};
    }
}

void HttpHeaderCollection::Remove(std::string_view name) {
    std::string name_lowercase = LowerString(name);
    auto it = storage_.find(name_lowercase);
    if (it != storage_.end()) {
        storage_.erase(it);
    }
}

std::size_t HttpHeaderCollection::Size() const {
    return storage_.size();
}

bool HttpHeaderCollection::Contains(std::string_view name) const {
    std::string name_lowercase = LowerString(name);
    auto it = storage_.find(name_lowercase);
    return it != storage_.end();
}

const HttpHeaderCollection::Storage &HttpHeaderCollection::Items() const {
    return storage_;
}

void HttpHeaderCollection::Clear() {
    storage_.clear();
}

HttpRequest::HttpRequest() {

}

std::string HttpRequest::ToString() const {
    std::string result;
    result += HttpMethodToString(method_) + " " + target_ + " HTTP/1.1\r\n";
    result += "Host: ";
    result += host_;
    if (port_.has_value()) {
        result += ":" + std::to_string(*port_);
    }
    result += "\r\n";
    for (const auto& [key, header] : headers_.Items()) {
        result += header.name + ": " + header.value + "\r\n";
    }
    result += "\r\n";
    if (has_body_) {
        result += body_;
    }
    return result;
}

HttpMethod HttpRequest::GetMethod() const {
    return method_;
}

std::string_view HttpRequest::GetTarget() const {
    return target_;
}

std::string_view HttpRequest::GetHost() const {
    return host_;
}

std::optional<std::uint16_t> HttpRequest::GetPort() const {
    return port_;
}

const HttpHeaderCollection &HttpRequest::GetHeaders() const {
    return headers_;
}

std::optional<std::string_view> HttpRequest::GetBody() const {
    return has_body_ ? std::optional<std::string_view>(body_) : std::nullopt;
}

HttpRequest::Builder::Builder() {
    method_ = HttpMethod::Get;
    target_ = "/";
    host_ = "";
    port_ = std::nullopt;
    headers_.Clear();
    body_ = "";
    query_params_.clear();
    has_body_ = false;
}

HttpRequest::Builder &HttpRequest::Builder::Reset() {
    this->method_ = HttpMethod::Get;
    this->target_ = "/";
    this->host_ = "";
    this->port_ = std::nullopt;
    this->headers_.Clear();
    this->body_ = "";
    this->query_params_.clear();
    this->has_body_ = false;
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetMethod(HttpMethod method) {
    this->method_ = method;
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetTarget(std::string_view target) {
    if (target.empty() || target[0] != '/') {
        this->target_ = '/' + std::string(target);
    } else {
        this->target_ = target;
    }
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::Get(std::string_view target) {
    SetMethod(HttpMethod::Get);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Head(std::string_view target) {
    SetMethod(HttpMethod::Head);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Post(std::string_view target) {
    SetMethod(HttpMethod::Post);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Put(std::string_view target) {
    SetMethod(HttpMethod::Put);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Delete(std::string_view target) {
    SetMethod(HttpMethod::Delete);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Patch(std::string_view target) {
    SetMethod(HttpMethod::Patch);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Options(std::string_view target) {
    SetMethod(HttpMethod::Options);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::SetHost(std::string_view host) {
    this->host_ = host;
    if (host_.empty()) {
        port_ = std::nullopt;
    }
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetPort(std::uint16_t port) {
    this->port_ = port;
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetQuery(std::string_view key, std::string_view value) {
    query_params_.emplace_back(std::string(key), std::string(value));
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::ClearQuery() {
    query_params_.clear();
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::RemoveQuery(std::string_view key) {
    for (auto it = query_params_.begin(); it != query_params_.end(); it++) {
        if (it->first == key) {
            query_params_.erase(it);
            break;
        }
    }
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetHeader(std::string_view name, std::string_view value) {
    if (LowerString(name) == "host") {
        host_ = value;
    } else {
        headers_.Set(name, value);
    }
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::RemoveHeader(std::string_view name) {
    headers_.Remove(name);
    if (LowerString(name) == "host") {
        host_ = "";
        port_ = std::nullopt;
    }
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetBody(std::string_view data) {
    this->body_ = data;
    this->has_body_ = true;
    headers_.Set("Content-Length", std::to_string(data.size()));
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetNoBody() {
    this->body_ = "";
    this->has_body_ = false;
    headers_.Remove("Content-Length");
    return *this;
}

std::string DecToHex(size_t dec) {
	size_t hex = dec;
    size_t shift;
	std::string result;
	while (dec > 0) {
		hex = dec % 16;
		if (hex < 10) {
			shift = static_cast<size_t>('0');
        } else {
			shift = static_cast<size_t>('A') - 10;
        }
        result = std::string(1, (hex + shift)) + result;
		dec /= 16;
	}
	return result.empty() ? "0" : result;
}

std::string ApplyPercentEncoding(std::string_view str) {
    std::string result;
    for (char c : str) {
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') ||
        c == '-' || c == '_' || c == '.' || c == '~') {
            result += c;
        } else {
            size_t char_idx = static_cast<unsigned char>(c);
            std::string hex = DecToHex(char_idx);
            if (hex.length() < 2) {
                hex = "0" + hex;
            }
            result += '%' + hex.substr(0, 2);
        }
    }
    return result;
}

HttpRequest HttpRequest::Builder::Build() const {
    HttpRequest request;
    request.method_ = method_;
    request.target_ = target_;
    request.host_ = host_;
    request.port_ = port_;
    request.body_ = body_;
    request.has_body_ = has_body_;
    request.headers_ = headers_;
    if (!query_params_.empty()) {
        std::string query_str;
        for (size_t i = 0; i < query_params_.size(); ++i) {
            query_str += ApplyPercentEncoding(query_params_[i].first) + '=' + ApplyPercentEncoding(query_params_[i].second);
            if (i < query_params_.size() - 1) {
                query_str += '&';
            }
        }
        request.target_ += '?' + query_str;
    }
    return request;
}
