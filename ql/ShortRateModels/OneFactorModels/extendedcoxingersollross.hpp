
/*
 Copyright (C) 2001, 2002, 2003 Sadruddin Rejeb

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

/*! \file extendedcoxingersollross.hpp
    \brief Extended Cox-Ingersoll-Ross model
*/

#ifndef quantlib_one_factor_models_extended_cox_ingersoll_ross_h
#define quantlib_one_factor_models_extended_cox_ingersoll_ross_h

#include <ql/ShortRateModels/OneFactorModels/coxingersollross.hpp>

namespace QuantLib {

    namespace ShortRateModels {

        //! Extended Cox-Ingersoll-Ross model class.
        /*! This class implements the extended Cox-Ingersoll-Ross model
            defined by 
            \f[ 
                dr_t = (\theta(t) - \alpha r_t)dt + \sqrt{r_t}\sigma dW_t .
            \f]
        */
        class ExtendedCoxIngersollRoss : public CoxIngersollRoss,
                                         public TermStructureConsistentModel {
          public:
            ExtendedCoxIngersollRoss(
                const RelinkableHandle<TermStructure>& termStructure,
                double theta = 0.1,
                double k = 0.1,
                double sigma = 0.1,
                double x0 = 0.05);

            Handle<Lattices::Lattice> tree(const TimeGrid& grid) const;

            Handle<ShortRateDynamics> dynamics() const;

            double discountBondOption(Option::Type type,
                                      double strike,
                                      Time maturity,
                                      Time bondMaturity) const;

          protected:
            void generateArguments();
            double A(Time t, Time T) const;

          private:
            class Dynamics;
            class FittingParameter;

            Parameter phi_;
        };

        //! Short-rate dynamics in the extended Cox-Ingersoll-Ross model
        /*! The short-rate is here
            \f[
                r_t = \varphi(t) + y_t^2
             \f]
            where \f$ \varphi(t) \f$ is the deterministic time-dependent 
            parameter used for term-structure fitting and \f$ y_t \f$ is the 
            state variable, the square-root of a standard CIR process.
        */
        class ExtendedCoxIngersollRoss::Dynamics
        : public CoxIngersollRoss::Dynamics {
          public:
            Dynamics(const Parameter& phi,
                     double theta,
                     double k,
                     double sigma,
                     double x0)
            : CoxIngersollRoss::Dynamics(theta, k, sigma, x0), phi_(phi) {}

            virtual double variable(Time t, Rate r) const {
                return QL_SQRT(r - phi_(t));
            }
            virtual double shortRate(Time t, double y) const {
                return y*y + phi_(t);
            }
          private:
            Parameter phi_;
        };

        //! Analytical term-structure fitting parameter \f$ \varphi(t) \f$.
        /*! \f$ \varphi(t) \f$ is analytically defined by
            \f[
                \varphi(t) = f(t) - 
                             \frac{2k\theta(e^{th}-1)}{2h+(k+h)(e^{th}-1)} -
                             \frac{4 x_0 h^2 e^{th}}{(2h+(k+h)(e^{th}-1))^1},
            \f]
            where \f$ f(t) \f$ is the instantaneous forward rate at \f$ t \f$ 
            and \f$ h = \sqrt{k^2 + 2\sigma^2} \f$.
        */
        class ExtendedCoxIngersollRoss::FittingParameter 
        : public TermStructureFittingParameter {
          private:
            class Impl : public Parameter::Impl {
              public:
                Impl(const RelinkableHandle<TermStructure>& termStructure,
                     double theta, double k, double sigma, double x0) 
                : termStructure_(termStructure), 
                  theta_(theta), k_(k), sigma_(sigma), x0_(x0) {}

                double value(const Array& params, Time t) const {
                    double forwardRate = 
                        termStructure_->instantaneousForward(t);
                    double h = QL_SQRT(k_*k_ + 2.0*sigma_*sigma_);
                    double expth = QL_EXP(t*h);
                    double temp = 2.0*h + (k_+h)*(expth-1.0);
                    double phi = forwardRate -
                                 2.0*k_*theta_*(expth - 1.0)/temp -
                                 x0_*4.0*h*h*expth/(temp*temp);
                    return phi;
                }
              private:
                RelinkableHandle<TermStructure> termStructure_;
                double theta_, k_, sigma_, x0_;
            };
          public:
            FittingParameter(
                const RelinkableHandle<TermStructure>& termStructure,
                double theta, double k, double sigma, double x0) 
            : TermStructureFittingParameter(Handle<Parameter::Impl>(
                new FittingParameter::Impl(
                    termStructure, theta, k, sigma, x0))) {}
        };

        // inline definitions

        inline Handle<OneFactorModel::ShortRateDynamics> 
        ExtendedCoxIngersollRoss::dynamics() const {
            return Handle<ShortRateDynamics>(
                new Dynamics(phi_, theta(), k() , sigma(), x0()));
        }

        inline void ExtendedCoxIngersollRoss::generateArguments() {
            phi_ = FittingParameter(termStructure(), theta(), k(), sigma(), 
                                    x0());
        }

    }

}

#endif
