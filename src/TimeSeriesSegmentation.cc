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
#include <rlfd/DHMMSegmenter.hh>
#include <rlfd/DelayEmbedding.hh>
#include <rlfd/GammaTest.hh>
#include <rlfd/utils/Gnuplot.hh>
#include <rlfd/utils/Autocorrelation.hh>

#include <random>

int main(void)
{
  rlfd::DHMMSegmenter<rlfd::DelayEmbedding> segmenter();
  // Synthetic example 
  // Generate 500 uniformly distributed points x in the range 
  // [0, 2pi] and add uniformly distributed
  // noise component with a variance of 0.075
  std::random_device rd; 
  std::mt19937 gen(rd());

  const double pi = 3.14159265358979;
  const int npoints = 500;
  std::uniform_real_distribution<double> uniform(0.0, 2.0*pi);
  std::normal_distribution<double> noise(0.0, std::sqrt(0.075));

  std::vector<double> input(npoints);
  std::vector<double> output(npoints);
  for (int i = 0; i < npoints; i++) {
      input[i] = uniform(gen);
      output[i] = std::sin(input[i]) + noise(gen);   
  }

//  rlfd::utils::Gnuplot gnuplot;
//  gnuplot(input, output); 

  // Run the gamma test
  //rlfd::GammaTest gamma;
  //auto gamma_statistic = gamma(Eigen::VectorXd::Map(&input[0], input.size()), Eigen::VectorXd::Map(&output[0], output.size()));
  //std::cout << gamma_statistic << std::endl;

  // Compute the autocorrelation
  Eigen::VectorXd acoeffs;
  rlfd::utils::Autocorrelation(Eigen::VectorXd::Map(&output[0], output.size()), acoeffs);
  std::cout << acoeffs << std::endl;

  std::cout << "Input to FFT" << std::endl;
  std::cout << Eigen::VectorXd::Map(&output[0], output.size()) << std::endl; 

  return 0;
}
