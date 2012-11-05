/**
 * Skills segmentation and learning for Robot Learning by Demonstration
 * Copyright (C) 2012  Pierre-Luc Bacon <pierre-luc.bacon@mail.mcgill.ca> 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */
#include <iostream>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

using namespace boost::numeric::odeint;

const double sigma = 10.0;
const double R = 28.0;
const double b = 8.0 / 3.0;

typedef boost::array< double , 3 > state_type;

void lorenz( const state_type &x , state_type &dxdt , double t )
{
  dxdt[0] = sigma * ( x[1] - x[0] );
  dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
  dxdt[2] = -b * x[2] + x[0] * x[1];
}

void write_lorenz( const state_type &x , const double t )
{
  std::cout << t << '\t' << x[0] << '\t' << x[1] << '\t' << x[2] << std::endl;
}

int main(int argc, char **argv)
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " [Delta t] [Number of points]" << std::endl << std::endl;
    std::cout << "Generate points from the Lorenz attractor." << std::endl;
    std::cout << "Lorenz, Edward N., 1963: Deterministic Nonperiodic Flow. J. Atmos. Sci., 20, 130–141." << std::endl;
    std::cout << "doi: http://dx.doi.org/10.1175/1520-0469(1963)020<0130:DNF>2.0.CO;2" << std::endl;
    std::cout << "Parameters: sigma = 10, rho = 28, beta = 8/3" << std::endl;
    return -1;
  }
  double dt = std::stod(argv[1]);
  int npoints = std::stoi(argv[2])-1;
  state_type x0 = { 1.0/10.0 , -1.0/5.0 , 3.0/10.0 }; // initial conditions
  runge_kutta4< state_type > stepper;
  integrate_n_steps(stepper, lorenz, x0, /*t0*/ 0.0, dt, npoints, write_lorenz);
  return 0;
}

