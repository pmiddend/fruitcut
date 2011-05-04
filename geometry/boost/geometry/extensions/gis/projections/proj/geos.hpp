#ifndef BOOST_GEOMETRY_PROJECTIONS_GEOS_HPP
#define BOOST_GEOMETRY_PROJECTIONS_GEOS_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2011 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels (Geodan, Amsterdam)

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projection
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace geos{

            struct par_geos
            {
                double    h;
                double  radius_p;
                double  radius_p2;
                double  radius_p_inv2;
                double  radius_g;
                double  radius_g_1;
                double  C;
            };



            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_geos_ellipsoid : public base_t_fi<base_geos_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_geos m_proj_parm;

                inline base_geos_ellipsoid(const Parameters& par)
                    : base_t_fi<base_geos_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double r, Vx, Vy, Vz, tmp;

                /* Calculation of geocentric latitude. */
                    lp_lat = atan (this->m_proj_parm.radius_p2 * tan (lp_lat));
                /* Calculation of the three components of the vector from satellite to
                ** position on earth surface (lon,lat).*/
                    r = (this->m_proj_parm.radius_p) / boost::math::hypot(this->m_proj_parm.radius_p * cos (lp_lat), sin (lp_lat));
                    Vx = r * cos (lp_lon) * cos (lp_lat);
                    Vy = r * sin (lp_lon) * cos (lp_lat);
                    Vz = r * sin (lp_lat);
                /* Check visibility. */
                    if (((this->m_proj_parm.radius_g - Vx) * Vx - Vy * Vy - Vz * Vz * this->m_proj_parm.radius_p_inv2) < 0.)
                        throw proj_exception();;
                /* Calculation based on view angles from satellite. */
                    tmp = this->m_proj_parm.radius_g - Vx;
                    xy_x = this->m_proj_parm.radius_g_1 * atan (Vy / tmp);
                    xy_y = this->m_proj_parm.radius_g_1 * atan (Vz / boost::math::hypot (Vy, tmp));
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double Vx, Vy, Vz, a, b, det, k;

                /* Setting three components of vector from satellite to position.*/
                    Vx = -1.0;
                    Vy = tan (xy_x / this->m_proj_parm.radius_g_1);
                    Vz = tan (xy_y / this->m_proj_parm.radius_g_1) * boost::math::hypot(1.0, Vy);
                /* Calculation of terms in cubic equation and determinant.*/
                    a = Vz / this->m_proj_parm.radius_p;
                    a   = Vy * Vy + a * a + Vx * Vx;
                    b   = 2 * this->m_proj_parm.radius_g * Vx;
                    if ((det = (b * b) - 4 * a * this->m_proj_parm.C) < 0.) throw proj_exception();;
                /* Calculation of three components of vector from satellite to position.*/
                    k  = (-b - sqrt(det)) / (2. * a);
                    Vx = this->m_proj_parm.radius_g + k * Vx;
                    Vy *= k;
                    Vz *= k;
                /* Calculation of longitude and latitude.*/
                    lp_lon  = atan2 (Vy, Vx);
                    lp_lat = atan (Vz * cos (lp_lon) / Vx);
                    lp_lat = atan (this->m_proj_parm.radius_p_inv2 * tan (lp_lat));
                }
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_geos_spheroid : public base_t_fi<base_geos_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_geos m_proj_parm;

                inline base_geos_spheroid(const Parameters& par)
                    : base_t_fi<base_geos_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double Vx, Vy, Vz, tmp;

                /* Calculation of the three components of the vector from satellite to
                ** position on earth surface (lon,lat).*/
                    tmp = cos(lp_lat);
                    Vx = cos (lp_lon) * tmp;
                    Vy = sin (lp_lon) * tmp;
                    Vz = sin (lp_lat);
                /* Check visibility.*/
                    if (((this->m_proj_parm.radius_g - Vx) * Vx - Vy * Vy - Vz * Vz) < 0.) throw proj_exception();;
                /* Calculation based on view angles from satellite.*/
                    tmp = this->m_proj_parm.radius_g - Vx;
                    xy_x = this->m_proj_parm.radius_g_1 * atan(Vy / tmp);
                    xy_y = this->m_proj_parm.radius_g_1 * atan(Vz / boost::math::hypot(Vy, tmp));
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double Vx, Vy, Vz, a, b, det, k;

                /* Setting three components of vector from satellite to position.*/
                    Vx = -1.0;
                    Vy = tan (xy_x / (this->m_proj_parm.radius_g - 1.0));
                    Vz = tan (xy_y / (this->m_proj_parm.radius_g - 1.0)) * sqrt (1.0 + Vy * Vy);
                /* Calculation of terms in cubic equation and determinant.*/
                    a   = Vy * Vy + Vz * Vz + Vx * Vx;
                    b   = 2 * this->m_proj_parm.radius_g * Vx;
                    if ((det = (b * b) - 4 * a * this->m_proj_parm.C) < 0.) throw proj_exception();;
                /* Calculation of three components of vector from satellite to position.*/
                    k  = (-b - sqrt(det)) / (2 * a);
                    Vx = this->m_proj_parm.radius_g + k * Vx;
                    Vy *= k;
                    Vz *= k;
                /* Calculation of longitude and latitude.*/
                    lp_lon = atan2 (Vy, Vx);
                    lp_lat = atan (Vz * cos (lp_lon) / Vx);
                }
            };

            // Geostationary Satellite View
            template <typename Parameters>
            void setup_geos(Parameters& par, par_geos& proj_parm)
            {
                if ((proj_parm.h = pj_param(par.params, "dh").f) <= 0.) throw proj_exception(-30);
                if (par.phi0) throw proj_exception(-46);
                proj_parm.radius_g = 1. + (proj_parm.radius_g_1 = proj_parm.h / par.a);
                proj_parm.C  = proj_parm.radius_g * proj_parm.radius_g - 1.0;
                if (par.es) {
                    proj_parm.radius_p      = sqrt (par.one_es);
                    proj_parm.radius_p2     = par.one_es;
                    proj_parm.radius_p_inv2 = par.rone_es;
                // par.inv = e_inverse;
                // par.fwd = e_forward;
                } else {
                    proj_parm.radius_p = proj_parm.radius_p2 = proj_parm.radius_p_inv2 = 1.0;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
                }
            }

        }} // namespace detail::geos
    #endif // doxygen

    /*!
        \brief Geostationary Satellite View projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Azimuthal
         - Spheroid
         - Ellipsoid
         - h=
        \par Example
        \image html ex_geos.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct geos_ellipsoid : public detail::geos::base_geos_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline geos_ellipsoid(const Parameters& par) : detail::geos::base_geos_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::geos::setup_geos(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Geostationary Satellite View projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Azimuthal
         - Spheroid
         - Ellipsoid
         - h=
        \par Example
        \image html ex_geos.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct geos_spheroid : public detail::geos::base_geos_spheroid<Geographic, Cartesian, Parameters>
    {
        inline geos_spheroid(const Parameters& par) : detail::geos::base_geos_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::geos::setup_geos(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class geos_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<geos_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                    else
                        return new base_v_fi<geos_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void geos_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("geos", new geos_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projection

#endif // BOOST_GEOMETRY_PROJECTIONS_GEOS_HPP

