/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CPX_HH_1278398428__H_
#define CPX_HH_1278398428__H_

#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace c {
struct cpx {
  double re;
  double im;
};
}
namespace internal_avro {
template <>
struct codec_traits<c::cpx> {
  static void encode(Encoder& e, const c::cpx& v) {
    internal_avro::encode(e, v.re);
    internal_avro::encode(e, v.im);
  }
  static void decode(Decoder& d, c::cpx& v) {
    internal_avro::decode(d, v.re);
    internal_avro::decode(d, v.im);
  }
};
}
#endif