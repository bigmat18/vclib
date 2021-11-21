/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "timer.h"

namespace vcl {

inline Timer::Timer(bool _start) : caption("Timer"), isStopped(false)
{
	if (_start)
		start();
}

/**
 * @brief Timer::Timer
 * @param caption
 * @param _start
 */
inline Timer::Timer(const char* caption, bool _start) : caption(caption), isStopped(false)
{
	if (_start)
		start();
}

/**
 * @brief Timer::Timer
 * @param caption
 * @param _start
 */
inline Timer::Timer(const std::string& caption, bool _start) : caption(caption), isStopped(false)
{
	if (_start)
		start();
}

/**
 * @brief Timer::start
 */
inline void Timer::start()
{
	begin = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Timer::stopAndPrint
 */
inline void Timer::stopAndPrint()
{
	stop();
	print();
}

/**
 * @brief Timer::stop
 */
inline void Timer::stop()
{
	end       = std::chrono::high_resolution_clock::now();
	isStopped = true;
}

/**
 * @brief Timer::print
 */
inline void Timer::print() const
{
	double secs = delay();
	int    mins = (int) secs / 60;
	if (mins == 0)
		std::cout << "[" << secs << " secs]\t" << caption << std::endl;
	else {
		secs      = secs - mins * 60;
		int hours = mins / 60;
		if (hours == 0)
			std::cout << "[" << mins << " mins; " << secs << " secs]\t" << caption << std::endl;
		else {
			mins = mins - hours * 60;
			std::cout << "[" << hours << " hours; " << mins << " mins; " << secs << " secs]\t"
					  << caption << std::endl;
		}
	}
}

/**
 * @brief Timer::delay
 * @return
 */
inline double Timer::delay() const
{
	double secs;
	if (isStopped) {
		secs =
			(double) (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /
			1000000;
	}
	else {
		std::chrono::high_resolution_clock::time_point s =
			std::chrono::high_resolution_clock::now();
		secs = (double) (std::chrono::duration_cast<std::chrono::microseconds>(s - begin).count()) /
			   1000000;
	}
	return secs;
}

} // namespace vcl