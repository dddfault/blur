#pragma once

// NOLINTBEGIN(misc-include-cleaner)

// std
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <stack>
#include <filesystem>
#include <numeric>
#include <functional>
#include <codecvt>
#include <locale>
#include <regex>
#include <set>
#include <unordered_set>
#include <ranges>
#include <cfloat>

// libs
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <tl/expected.hpp>

#include <boost/process.hpp>
#include <boost/asio.hpp>
#ifdef _WIN32
#	include <boost/process/v1/windows.hpp>
#endif

// os
#ifdef _WIN32
#	include <conio.h>
#	include <shobjidl.h>
#	include <Windows.h>
#	include <tlhelp32.h>
#elif __APPLE__
#	include <mach-o/dyld.h>
#	include <CoreFoundation/CoreFoundation.h>
#endif

// blur
#include "blur.h"
#include "utils.h"

// NOLINTEND(misc-include-cleaner)
