
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

/*! \file stockholm.hpp
    \brief Stockholm calendar
*/

#ifndef quantlib_stockholm_calendar_h
#define quantlib_stockholm_calendar_h

#include <ql/calendar.hpp>

namespace QuantLib {

    namespace Calendars {

        //! %Stockholm calendar
        /*! Holidays:
            <ul>
            <li>Saturdays</li>
            <li>Sundays</li>
            <li>Good Friday</li>
            <li>Easter Monday</li>
            <li>Ascension</li>
            <li>Whit(Pentecost) Monday </li>
            <li>Midsummer Eve (Friday between June 18-24)</li>            
            <li>New Year's Day, January 1st</li>
            <li>Epiphany, January 6th</li>
            <li>May Day, May 1st</li>
            <li>National Day, June 6th</li>
            <li>Christmas Eve, December 24th</li>
            <li>Christmas Day, December 25th</li>
            <li>Boxing Day, December 26th</li>
            <li>New Year's Eve, December 31th</li>
            </ul>
        */
        class Stockholm : public Calendar {
          private:
            class Impl : public Calendar::WesternImpl {
              public:
                std::string name() const { return "Stockholm"; }
                bool isBusinessDay(const Date&) const;
            };
          public:
            Stockholm()
            : Calendar(Handle<Calendar::Impl>(new Stockholm::Impl)) {}
        };

    }

}

#endif
