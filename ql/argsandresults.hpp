
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

/*! \file argsandresults.hpp
    \brief Base classes for generic arguments and results
*/

#ifndef quantlib_args_and_results_h
#define quantlib_args_and_results_h

#include <ql/null.hpp>

namespace QuantLib {

    //! base class for generic argument groups
    class Arguments {
      public:
        virtual ~Arguments() {}
        virtual void validate() const = 0;
    };

    //! base class for generic result groups
    class Results {
      public:
        virtual ~Results() {}
        virtual void reset() = 0;
    };


    //! %option pricing results
    /*! It must be noted that there's no result data specifying
        whether the option is expired. The expiry condition should be
        checked before calling the engine.
    */
    class OptionValue : public virtual Results {
      public:
        OptionValue() { reset(); }
        void reset() {
            value = errorEstimate = Null<double>();
        }
        double value;
        double errorEstimate;
    };

    //! %option pricing results
    class OptionGreeks : public virtual Results {
      public:
        OptionGreeks() { reset(); }
        void reset() {
            delta =  gamma = theta = vega =
                rho = dividendRho = strikeSensitivity = Null<double>();
        }
        double delta, gamma;
        double theta;
        double vega;
        double rho, dividendRho;
        double strikeSensitivity;
    };

}


#endif
