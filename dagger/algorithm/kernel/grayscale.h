/*
 * Copyright (c) 2013 Hrvoje Zeba <zeba.hrvoje@gmail.com>
 *
 *    This file is part of dagger.
 *
 *    dagger is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once


#include <dagger/data/grayscale.h>
#include <dagger/algorithm/kernel/base.h>


namespace dagger {
namespace algorithm {
namespace kernel {

struct grayscale
{
    matrix g_kernel;

    bool normalize;

    grayscale()
      : g_kernel(1, 1)
      , normalize(false)
    {
        g_kernel.set_value(0, 0, 1);
    }

    data::grayscale operator()(const data::grayscale& s)
    {
        data::grayscale d;

        d.g = calculate(s.g, g_kernel, normalize);

        return d;
    }
};

}}}
