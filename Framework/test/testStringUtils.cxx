// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file    testStringUtils.cxx
/// \author  Barthelemy von Haller
///

#include "QualityControl/stringUtils.h"

#define BOOST_TEST_MODULE Triggers test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

using namespace o2::quality_control::core;

BOOST_AUTO_TEST_CASE(test_is_number)
{
  BOOST_CHECK_EQUAL(isUnsignedInteger("1"), true);
  BOOST_CHECK_EQUAL(isUnsignedInteger("-1"), false);
  BOOST_CHECK_EQUAL(isUnsignedInteger("1000000"), true);
  BOOST_CHECK_EQUAL(isUnsignedInteger("0.1"), false);
  BOOST_CHECK_EQUAL(isUnsignedInteger(".2"), false);
  BOOST_CHECK_EQUAL(isUnsignedInteger("x"), false);
  BOOST_CHECK_EQUAL(isUnsignedInteger("1x"), false);
  BOOST_CHECK_EQUAL(isUnsignedInteger("......"), false);
}