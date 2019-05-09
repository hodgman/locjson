 This software is licensed under either the terms of the MIT license
  or the DWTFYWPL, reproduced below.

------------------------------------------------------------------------------
MIT License
------------------------------------------------------------------------------

Copyright (c) 2019 Brooke Hodgman
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------
 DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
------------------------------------------------------------------------------

  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

 0. You just DO WHAT THE FUCK YOU WANT TO.

------------------------------------------------------------------------------
 Purpose
------------------------------------------------------------------------------
 I needed a simple, bare-bones, C++ JSON parser with no dependencies.
 This parser is written to be small and simple (LOC-JSON stands for lines of 
 code), so it's likely slower than other parsers and has a coarser API.

 This library is also designed to be replacable. The `migration` directory
 contains:
   `locjson_to_cpprest.h`
   `locjson_to_rapidjson.h`
 Which implement the same API as this file, but simply convert your calls to
 the cpprest or japidjson libraries.
 If you write code that uses this locjson, you can easily swap this file out
 for one of the above files later to switch to a better JSON parser.

 Why another no-dependencies single-file C++ JSON parser?
 I looked at some others and they were up to 20k LOC... This file is:
 ~140 lines of this comment block
 ~130 lines of API declaration + config block
 ~200 lines of implementation

------------------------------------------------------------------------------
 Limitations
------------------------------------------------------------------------------
 The parser should handle any valid JSON document. However, there is currently
 no API to *retrieve* fractional/floating point numbers, booleans, or null.

 Error reporting will return a flag (or throw an exception if you opt-in) when
 invalid JSON is detected, but there's no line number reporting or anything 
 helpful like that.

 The JSON builder is VERY simple. It only supports a single, non-nested object
 containing string keys, string values, or array values containing strings or 
 numbers.

------------------------------------------------------------------------------
 API configuration
------------------------------------------------------------------------------
 As different C++ projects may use different primitive types, you can use the 
  following `#define`s (BEFORE including locjson.h) to override the default types.

```
 #define               | typedef               | Default           | Notes
 LOCJSON_INT32         | locjson::Int32        | int32_t           |
 LOCJSON_UINT32        | locjson::UInt32       | uint32_t          |
 LOCJSON_CHAR          | locjson::Char         | char              | See Unicode section
 LOCJSON_STRING        | locjson::String       | std::string       | Must support construction from `const LOCJSON_CHAR*`
 LOCJSON_STRING_VIEW   | locjson::StringView   | std::string_view* | *If C++17 is available, else std::string
 LOCJSON_STRING_STREAM | locjson::StringStream | std::stringstream | 
```

 The behavior of this header can further be modified by using the following 
  `#define`s (BEFORE including locjson.h)
```
 #define                    | Notes                   
 LOCJSON_EXCEPTION(message) | See Exceptions section
 LOCJSON_LITERAL(x)         | See Unicode section
 LOCJSON_FUNCTION           | See Integration section         
 LOCJSON_IMPLEMENTATION     | See Integration section
```
------------------------------------------------------------------------------
 Integration
------------------------------------------------------------------------------
 This API is provided in the "single header" style to support simple and flexible
  integration into your project (see https://github.com/nothings/single_file_libs).
  The implementation of function bodies will be excluded unless you define
  `LOCJSON_IMPLEMENTATION` before including locjson.h.

 Typical linking:
  In one CPP file, before including locjson.h:
   `#define LOCJSON_IMPLEMENTATION`

 Inline linking:
  In every CPP file that uses the API, before including locjson.h:
   `#define LOCJSON_IMPLEMENTATION`
   `#define LOCJSON_FUNCTION inline`

 Aside from `LOCJSON_IMPLEMENTATION` / `LOCJSON_FUNCTION`, you should take care
  to ensure that every other `LOCJSON_*` macro is defined to the same value in
  all of your CPP files that use the locjson API.

------------------------------------------------------------------------------
 Exceptions
------------------------------------------------------------------------------
 Support for C++ exceptions is opt-in. Define the following (or an alternative
  based on your own exception classes) before including locjson.h:
```
 #define LOCJSON_EXCEPTION(message) throw std::runtime_error(message)
```
------------------------------------------------------------------------------
 Unicode
------------------------------------------------------------------------------
 To build a wide-character version of the API, define the following before
  including locjson.h:
```
 #define LOCJSON_WIDE
```
------------------------------------------------------------------------------
 C++17
------------------------------------------------------------------------------
 When built on a C++17 compiler, the parser will not perform any memory 
 allocations. On earlier compilers, the parser will perform a lot of temporary
 memory allocations...

 Microsoft Visual Studio reports the wrong value for `__cplusplus` by default
 for reasons. To enable C++17 suppport in MSVC, you need to add these compiler
 switches:
  `/Zc:__cplusplus /std:c++17`
 OR:
  `/Zc:__cplusplus /std:c++latest`

------------------------------------------------------------------------------