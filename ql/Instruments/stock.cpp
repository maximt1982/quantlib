

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/
/*! \file stock.cpp
    \brief concrete stock class

    \fullpath
    ql/Instruments/%stock.cpp
*/

// $Id$

#include <ql/Instruments/stock.hpp>

namespace QuantLib {

    namespace Instruments {

        Stock::Stock(const RelinkableHandle<MarketElement>& quote,
            const std::string& isinCode, const std::string& description)
        : Instrument(isinCode,description), quote_(quote) {
            registerWith(quote_);
        }

        void Stock::performCalculations() const {
            QL_REQUIRE(!quote_.isNull(),
                "null quote set for "+isinCode()+" stock");
            isExpired_ = false;
            NPV_ = quote_->value();
        }

    }

}

