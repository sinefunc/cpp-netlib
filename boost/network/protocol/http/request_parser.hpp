//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Dean Michael Berris (mikhailberis at gmail dot com)
// Copyright (c) 2009 Tarro, Inc.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modifications by Dean Michael Berris <mikhailberis@gmail.com>
//

#ifndef HTTP_SERVER3_REQUEST_PARSER_HPP
#define HTTP_SERVER3_REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/network/protocol/http/request.hpp>

namespace boost { namespace network { namespace http {

namespace tag {
    struct default_;
}

/// Parser for incoming requests.
template <class Tag>
class basic_request_parser
{
public:
  /// Construct ready to parse the request method.
  basic_request_parser() : state_(method_start) {}

  /// Reset to initial parser state.
  void reset() { state_ = method_start; }

  /// Parse some data. The tribool return value is true when a complete request
  /// has been parsed, false if the data is invalid, indeterminate when more
  /// data is required. The InputIterator return value indicates how much of the
  /// input has been consumed.
  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> parse_headers(basic_request<Tag> & req,
      InputIterator begin, InputIterator end)
  {
    while (begin != end)
    {
      boost::tribool result = consume(req, *begin++);
      if (result || !result)
        return boost::make_tuple(result, begin);
    }
    boost::tribool result = boost::indeterminate;
    return boost::make_tuple(result, begin);
  }

private:
  /// Handle the next character of input.
  boost::tribool consume(basic_request<Tag> & req, char input);

  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  /// Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);

  /// The current state of the parser.
  enum state
  {
    method_start,
    method,
    uri_start,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3
  } state_;
};

typedef basic_request_parser<tags::http_server> request_parser;

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/impl/request_parser.ipp>

#endif // HTTP_SERVER3_REQUEST_PARSER_HPP
