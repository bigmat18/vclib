/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_SPACE_COLOR_H
#define VCL_SPACE_COLOR_H

#include <vector>

#include "point.h"

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The Color class represents an RGBA color.
 *
 * The class is a specialization of the Point4 class, where each component is an unsigned char.
 * The class provides some useful methods to convert the color from/to different formats.
 *
 * @ingroup space
 */
class Color : public Point4<uint8_t>
{
public:
	/// RGBA enum with some standard colors.
	/// Can be used to initialize a color with an RGBA integer
	enum ColorRGBA {
		Black     = 0x000000ff,
		DarkGray  = 0x404040ff,
		Gray      = 0x808080ff,
		LightGray = 0xc0c0c0ff,
		White     = 0xffffffff,

		Red   = 0xff0000ff,
		Green = 0x00ff00ff,
		Blue  = 0x0000ffff,

		Cyan    = 0x00ffffff,
		Yellow  = 0xffff00ff,
		Magenta = 0xff00ffff,

		LightRed   = 0xff8080ff,
		LightGreen = 0x80ff80ff,
		LightBlue  = 0x8080ffff,

		LightCyan    = 0x80ffffff,
		LightYellow  = 0xffff80ff,
		LightMagenta = 0xff80ffff,

		DarkRed   = 0x400000ff,
		DarkGreen = 0x004000ff,
		DarkBlue  = 0x000040ff,

		DarkCyan    = 0x004040ff,
		DarkYellow  = 0x404000ff,
		DarkMagenta = 0x400040ff
	};

	/// List of Color Maps
	enum ColorMap {
		RedBlue,
		Parula,
		GreyShade
	};

	Color();
	Color(ColorRGBA cc);
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	Color(const Point4<uint8_t>& p);

	uint8_t red() const;
	uint8_t green() const;
	uint8_t blue() const;
	uint8_t alpha() const;

	uint8_t& red();
	uint8_t& green();
	uint8_t& blue();
	uint8_t& alpha();

	float redF() const;
	float greenF() const;
	float blueF() const;
	float alphaF() const;

	uint8_t hsvHue() const;
	uint8_t hsvSaturation() const;

	float hsvHueF() const;
	float hsvSaturationF() const;
	
	unsigned short toUnsignedR5G5B5() const;
	unsigned short toUnsignedB5G5R5() const;

	void setAlpha(uint8_t alpha);
	void setRed(uint8_t red);
	void setGreen(uint8_t green);
	void setBlue(uint8_t blue);

	void setRgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	void setHsv(uint h, uint8_t s, uint8_t v, uint8_t alpha = 255);

	void setAlphaF(float alpha);
	void setRedF(float red);
	void setGreenF(float green);
	void setBlueF(float blue);

	void setRgbF(float red, float green, float blue, float alpha = 1.0);
	void setHsvF(float hf, float sf, float vf, float alpha = 1.0);

	void setFromUnsignedR5G5B5(unsigned short val);
	void setFromUnsignedB5G5R5(unsigned short val);

	bool operator==(const Color& otherColor) const;
	bool operator!=(const Color& otherColor) const;
	bool operator<(const Color& otherColor) const;

	/// @private
	friend std::ostream& operator<<(std::ostream& out, const Color& p);
};

std::ostream& operator<<(std::ostream& out, const Color& p);

Color colorLerp(const Color& c0, const Color& c1, float value);

Color colorFromInterval(float value, Color::ColorMap cm);
Color colorFromInterval(float min, float max, float value, Color::ColorMap cm);

Color colorFromIntervalRedBlue(float value);
Color colorFromIntervalRedBlue(float min, float max, float value);

Color colorFromIntervalParula(float value);
Color colorFromIntervalParula(float min, float max, float value);

Color colorFromIntervalGreyShade(float value);
Color colorFromIntervalGreyShade(float min, float max, float value);

std::vector<Color> colorScattering(uint n, float sat = 0.3, float val = 0.9);

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Default constructor.
 * Initializes a black color (with alpha 255).
 */
inline Color::Color() : Point(0, 0, 0, 255)
{
}

inline Color::Color(ColorRGBA cc)
{
	w() = cc % 256;
	z() = (cc >> 8) % 256;
	y() = (cc >> 16) % 256;
	x() = (cc >> 24) % 256;
}

/**
 * @brief Color constructor.
 * @param[in] red: red component
 * @param[in] green: green component
 * @param[in] blue: blue component
 * @param[in] alpha: alpha component (default 255)
 */
inline Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
		Point(red, green, blue, alpha)
{
}

inline Color::Color(const Point4<uint8_t>& p) :
		Point4<uint8_t>(p)
{
}

/**
 * @brief Returns the red component of this color [0-255]
 * @return red component of this color
 */
inline uint8_t Color::red() const
{
	return x();
}

/**
 * @brief Returns the green component of this color [0-255]
 * @return green component of this color
 */
inline uint8_t Color::green() const
{
	return y();
}

/**
 * @brief Returns the blue component of this color [0-255]
 * @return blue component of this color
 */
inline uint8_t Color::blue() const
{
	return z();
}

/**
 * @brief Returns the alpha component of this color [0-255]
 * @return alpha component of this color
 */
inline uint8_t Color::alpha() const
{
	return w();
}

/**
 * @brief Returns the red component of this color [0-255]
 * @return red component of this color
 */
inline uint8_t& Color::red()
{
	return x();
}

/**
 * @brief Returns the green component of this color [0-255]
 * @return green component of this color
 */
inline uint8_t& Color::green()
{
	return y();
}

/**
 * @brief Returns the blue component of this color [0-255]
 * @return blue component of this color
 */
inline uint8_t& Color::blue()
{
	return z();
}

/**
 * @brief Returns the alpha component of this color [0-255]
 * @return alpha component of this color
 */
inline uint8_t& Color::alpha()
{
	return w();
}

/**
 * @brief Returns the float red component of this color [0-1]
 * @return float red component of this color
 */
inline float Color::redF() const
{
	return (float) x() / 255;
}

/**
 * @brief Returns the float green component of this color [0-1]
 * @return float green component of this color
 */
inline float Color::greenF() const
{
	return (float) y() / 255;
}

/**
 * @brief Returns the float blue component of this color [0-1]
 * @return float blue component of this color
 */
inline float Color::blueF() const
{
	return (float) z() / 255;
}

/**
 * @brief Returns the float alpha component of this color [0-1]
 * @return float alpha component of this color
 */
inline float Color::alphaF() const
{
	return (float) w() / 255;
}

/**
 * @brief Returns the hue color component of this color [0-359]
 * @return hue color component of this color
 */
inline uint8_t Color::hsvHue() const
{
	uint8_t rgbMin, rgbMax;
	uint8_t h;

	rgbMin = x() < y() ? (x() < z() ? x() : z()) : (y() < z() ? y() : z());
	rgbMax = x() > y() ? (x() > z() ? x() : z()) : (y() > z() ? y() : z());

	if (rgbMax == 0) {
		return 0;
	}

	if (255 * long(rgbMax - rgbMin) / rgbMax == 0) {
		return 0;
	}

	if (rgbMax == x())
		h = 0 + 43 * (y() - z()) / (rgbMax - rgbMin);
	else if (rgbMax == y())
		h = 85 + 43 * (z() - x()) / (rgbMax - rgbMin);
	else
		h = 171 + 43 * (x() - y()) / (rgbMax - rgbMin);

	return h;
}

/**
 * @brief Returns the saturation color component of this color [0-255]
 * @return saturation color component of this color
 */
inline uint8_t Color::hsvSaturation() const
{
	uint8_t rgbMin, rgbMax;

	rgbMin = x() < y() ? (x() < z() ? x() : z()) : (y() < z() ? y() : z());
	rgbMax = x() > y() ? (x() > z() ? x() : z()) : (y() > z() ? y() : z());

	if (rgbMax == 0) {
		return 0;
	}

	return 255 * long(rgbMax - rgbMin) / rgbMax;
}

/**
 * @brief Returns the float saturation color component of this color [0-1]
 * @return float saturation color component of this color
 */
inline float Color::hsvHueF() const
{
	return (float) hsvHue() / 255;
}

/**
 * @brief Returns the float saturation color component of this color [0-1]
 * @return float saturation color component of this color
 */
inline float Color::hsvSaturationF() const
{
	return (float) hsvSaturation() / 255;
}

/**
 * @brief Converts the color to an unsigned short in R5G5B5 format.
 *
 * @return an unsigned short containing the converted color.
 */
inline unsigned short Color::toUnsignedR5G5B5() const
{
	unsigned short r = x()/8;
	unsigned short g = y()/8;
	unsigned short b = z()/8;
	unsigned short res = r + g*32 + b*1024;
	return res;
}

/**
 * @brief Converts the color to an unsigned short in B5G5R5 format.
 *
 * @return an unsigned short containing the converted color.
 */
inline unsigned short Color::toUnsignedB5G5R5() const
{
	unsigned short r = x()/8;
	unsigned short g = y()/8;
	unsigned short b = z()/8;
	unsigned short res = b + g*32 + r*1024;
	return res;
}

/**
 * @brief Sets the alpha of this color [0-255]
 * @param[in] alpha
 */
inline void Color::setAlpha(uint8_t alpha)
{
	w() = alpha;
}

/**
 * @brief Sets the red of this color [0-255]
 * @param[in] red
 */
inline void Color::setRed(uint8_t red)
{
	x() = red;
}

/**
 * @brief Sets the green of this color [0-255]
 * @param[in] green
 */
inline void Color::setGreen(uint8_t green)
{
	y() = green;
}

/**
 * @brief Sets the blue of this color [0-255]
 * @param[in] blue
 */
inline void Color::setBlue(uint8_t blue)
{
	z() = blue;
}

/**
 * @brief Sets the RGB values of this color.
 *
 * All the values must be in the range 0-255.
 *
 * @param[in] red
 * @param[in] green
 * @param[in] blue
 * @param[in] alpha: default 255
 */
inline void Color::setRgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	x() = red;
	y() = green;
	z() = blue;
	w() = alpha;
}

/**
 * @brief Sets the HSV values of this color.
 *
 * All the values must be in the range 0-255.
 *
 * @param[in] h
 * @param[in] s
 * @param[in] v
 * @param[in] alpha: default 255
 */
inline void Color::setHsv(uint h, uint8_t s, uint8_t v, uint8_t alpha)
{
	w() = alpha;
	if (s == 0) {
		x() = v;
		y() = v;
		z() = v;
	}
	else {
		h = (h / 360.0) * 255;
		uint8_t region, remainder, p, q, t;
		region    = h / 43;
		remainder = (h - (region * 43)) * 6;

		p = (v * (255 - s)) >> 8;
		q = (v * (255 - ((s * remainder) >> 8))) >> 8;
		t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

		switch (region) {
		case 0:
			x() = v;
			y() = t;
			z() = p;
			break;
		case 1:
			x() = q;
			y() = v;
			z() = p;
			break;
		case 2:
			x() = p;
			y() = v;
			z() = t;
			break;
		case 3:
			x() = p;
			y() = q;
			z() = v;
			break;
		case 4:
			x() = t;
			y() = p;
			z() = v;
			break;
		default:
			x() = v;
			y() = p;
			z() = q;
			break;
		}
	}
}

/**
 * @brief Sets the alpha of this color [0-1]
 * @param[in] alpha
 */
inline void Color::setAlphaF(float alpha)
{
	w() = (uint8_t) (alpha * 255);
}

/**
 * @brief Sets the red of this color [0-1]
 * @param[in] red
 */
inline void Color::setRedF(float red)
{
	x() = (uint8_t) (red * 255);
}

/**
 * @brief Sets the green of this color [0-1]
 * @param[in] green
 */
inline void Color::setGreenF(float green)
{
	y() = (uint8_t) (green * 255);
}

/**
 * @brief Sets the blue of this color [0-1]
 * @param[in] blue
 */
inline void Color::setBlueF(float blue)
{
	z() = (uint8_t) (blue * 255);
}

/**
 * @brief Sets the RGB values of this color.
 *
 * All the values must be in the range 0-1.
 *
 * @param[in] red: red component of the color in as a float value between 0 and 1.
 * @param[in] green: green component of the color in as a float value between 0 and 1.
 * @param[in] blue: blue component of the color in as a float value between 0 and 1.
 * @param[in] alpha: : alpha component of the color in as a float value between 0 and 1, default
 * value is 1.
 */
inline void Color::setRgbF(float red, float green, float blue, float alpha)
{
	w() = (uint8_t) (alpha * 255);
	x() = (uint8_t) (red * 255);
	y() = (uint8_t) (green * 255);
	z() = (uint8_t) (blue * 255);
}

/**
 * @brief Sets the HSV values of this color.
 *
 * All the values must be in the range 0-1.
 * The color will be converted in RGBA and then stored in this color.
 *
 * @param[in] hf
 * @param[in] sf
 * @param[in] vf
 * @param[in] alpha
 */
inline void Color::setHsvF(float hf, float sf, float vf, float alpha)
{
	setHsv(hf * 255, sf * 255, vf * 255, alpha * 255);
}

/**
 * Set the color values from an unsigned 5-5-5 RGB value.
 *
 * The input value is interpreted as follows:
 * - The 5 least significant bits represent the red component.
 * - The next 5 bits represent the green component.
 * - The 5 most significant bits represent the blue component.
 *
 * Each color component is scaled from 0 to 255 by multiplying the value by 8.
 *
 * @param[in] val: The unsigned 5-5-5 RGB value to set.
 */
inline void Color::setFromUnsignedR5G5B5(unsigned short val)
{
	x() = val % 32 * 8;
	y() = ((val / 32) % 32) * 8;
	z() = ((val / 1024) % 32) * 8;
	w() = 255;
}

/**
 * Set the color values from an unsigned 5-5-5 BGR value.
 *
 * The input value is interpreted as follows:
 * - The 5 least significant bits represent the blue component.
 * - The next 5 bits represent the green component.
 * - The 5 most significant bits represent the red component.
 *
 * Each color component is scaled from 0 to 255 by multiplying the value by 8.
 *
 * @param[in] val: The unsigned 5-5-5 BGR value to set.
 */
inline void Color::setFromUnsignedB5G5R5(unsigned short val)
{
	z() = val % 32 * 8;
	y() = ((val / 32) % 32) * 8;
	x() = ((val / 1024) % 32) * 8;
	w() = 255;
}

/**
 * @brief Returns true if this color has the same RGB and alpha values
 * as otherColor; otherwise returns false.
 *
 * @param otherColor
 * @return
 */
inline bool Color::operator==(const Color& otherColor) const
{
	return (
		x() == otherColor.x() && y() == otherColor.y() && z() == otherColor.z() &&
		w() == otherColor.w());
}

/**
 * @brief Returns false if this color has the same RGB and alpha values as otherColor; otherwise
 * returns true.
 *
 * @param otherColor
 * @return
 */
inline bool Color::operator!=(const Color& otherColor) const
{
	return !(*this == otherColor);
}

/**
 * @brief Returns true if this color is less than otherColor follwing the RGBA order; otherwise
 * returns false.
 *
 * @param otherColor
 * @return
 */
inline bool Color::operator<(const Color& otherColor) const
{
	if (x() < otherColor.x())
		return true;
	if (x() > otherColor.x())
		return false;
	if (y() < otherColor.y())
		return true;
	if (y() > otherColor.y())
		return false;
	if (z() < otherColor.z())
		return true;
	if (z() > otherColor.z())
		return false;
	if (w() < otherColor.w())
		return true;
	return false;
}

/**
 * @brief Overload of stream operator to allow a pretty print of a vcl::Color.
 */
inline std::ostream& operator<<(std::ostream& out, const Color& c)
{
	out << c.p.cast<uint>();
	return out;
}

/**
 * @brief Given an interval of colors (from `c0` to `c1`) and a value in the interval [0, 1],
 * returns the linear interpolation color between `c0` and `c1` based on `value` in the [0, 1]
 * interval. If `value` is out of range, it will be set to the nearest extreme of the interval.
 *
 * Linear interpolation is computed in the RGBA values of the two colors.
 *
 * @param[in] c0: minimum in the input color interval.
 * @param[in] c1: minimum in the input color interval.
 * @param[in] value: a float value between 0 and 1.
 * @return The linear interpolation between `c0` and `c1`.
 */
inline Color colorLerp(const Color& c0, const Color& c1, float value)
{
	Color c;
	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;
	for (uint i = 0; i < 4; i++)
		c(i) = c1(i)*value + c0(i)*(1-value);
	return c;
}

/**
 * @brief Given a `value` in the interval [0, 1], it returns the color in the position in the given
 * ColorMap `cm` corresponding to the position of `value` in the interval [0, 1].
 *
 * @param[in] value: a float value between 0 and 1.
 * @param[cm] cm: the colormap used to compute the color.
 * @return The color in the position of `value` in the given colormap.
 */
inline Color colorFromInterval(float value, Color::ColorMap cm)
{
	switch (cm) {
	case Color::RedBlue:
		return colorFromIntervalRedBlue(value);
	case Color::Parula:
		return colorFromIntervalParula(value);
	case Color::GreyShade:
		return colorFromIntervalGreyShade(value);
	default:
		assert(0);
		return Color::Gray;
	}
}

/**
 * @brief Returns a color in the interval from the minimum color to the maximum color of the given
 * colormap `cm`, depending on the position of the given value in the given interval [min, max].
 *
 * If the given value is less than the minimum value of the interval, the minimum color will be
 * returned. If the given value is higher than the maximum value of the interval, the maximum color
 * will be returned. If min and max are equal, the Grey color will be returned.
 *
 * This function works also if the values of the interval are swapped.
 *
 * @param[in] min: minimum value of the interval
 * @param[in] max: maximum value of the interval
 * @param[in] value: the value in the interval to ramp.
 * @param[in] cm: the colormap on which compute the color.
 * @return A color in the given colormap representing the position of value in the interval [min,
 * max].
 */
inline Color colorFromInterval(float min, float max, float value, Color::ColorMap cm)
{
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromInterval(value, cm);
}

/**
 * @brief Returns a color in the interval from Red to Blue depending on the position of the given
 * value in the interval [0, 1].
 *
 * If the given value is less than 0, the Red color will be returned. If the given value is higher
 * than 1, the Blue color will be returned.
 *
 * @param[in] value: the value between 0 and 1 to ramp.
 * @return A color between Red and Blue representing the position of value in the interval [0, 1].
 */
inline Color colorFromIntervalRedBlue(float value)
{
	Color c;

	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;

	c.setHsv(value * 240, 255, 255);
	return c;
}

/**
 * @brief Returns a color in the interval from Red to Blue depending on the position of the given
 * value in the given interval [min, max].
 *
 * If the given value is less than the minimum value of the interval, the Red color will be
 * returned. If the given value is higher than the maximum value of the interval, the Blue color
 * will be returned. If min and max are equal, the Grey color will be returned.
 *
 * This function works also if the values of the interval are swapped.
 *
 * @param[in] min: minimum value of the interval
 * @param[in] max: maximum value of the interval
 * @param[in] value: the value in the interval to ramp.
 * @return A color between Red and Blue representing the position of value in the interval [min,
 * max].
 */
inline Color colorFromIntervalRedBlue(float min, float max, float value)
{
	Color c;
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromIntervalRedBlue(value);
}

/**
 * @brief Returns a color in the Paraula colormap depending on the position of the given
 * value in the interval [0, 1].
 *
 * If the given value is less than 0, the minimum color of the colormap will be returned. If the
 * given value is higher than 1, the maximum color in the colormap will be returned.
 *
 * @param[in] value: the value between 0 and 1 to ramp.
 * @return A color in the Parula colormap representing the position of value in the interval [0, 1].
 */
inline Color colorFromIntervalParula(float value)
{
	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;

	static uint paruVal[9] = {
		0x271680ff,
		0x0363e1ff,
		0x1485d4ff,
		0x06a7c6ff,
		0x38b99eff,
		0x92bf73ff,
		0xd9ba56ff,
		0xfcce2eff,
		0xfffa0aff};

	int ind = int(floor(value * 8.0f));
	float div = (value * 8.0f - ind);

	if (div < 0)
		div = 0;
	else if (div > 1)
		div = 1;

	return colorLerp((Color::ColorRGBA) paruVal[ind], (Color::ColorRGBA) paruVal[ind + 1], div);
}

/**
 * @brief Returns a color in the Paraula colormap depending on the position of the given
 * value in the given interval [min, max].
 *
 * If the given value is less than the minimum value of the interval, the minimum color of the
 * colormap will be returned. If the given value is higher than the maximum value of the interval,
 * the maximum color in the colormap will be returned. If min and max are equal, the Grey color will
 * be returned.
 *
 * This function works also if the values of the interval are swapped.
 *
 * @param[in] min: minimum value of the interval
 * @param[in] max: maximum value of the interval
 * @param[in] value: the value in the interval to ramp.
 * @return A color in the Parula colormap representing the position of value in the interval [min,
 * max].
 */
inline Color colorFromIntervalParula(float min, float max, float value)
{
	Color c;
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromIntervalParula(value);
}

/**
 * @brief Returns a color in the grey shade colormap depending on the position of the given
 * value in the interval [0, 1].
 *
 * If the given value is less than 0, the minimum color of the colormap will be returned. If the
 * given value is higher than 1, the maximum color in the colormap will be returned.
 *
 * @param[in] value: the value between 0 and 1 to ramp.
 * @return A color in the grey shade colormap representing the position of value in the interval [0,
 * 1].
 */
inline Color colorFromIntervalGreyShade(float value)
{
	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;
	return Color(value * 255, value * 255, value * 255, 255);
}

/**
 * @brief Returns a color in the grey shade colormap depending on the position of the given
 * value in the given interval [min, max].
 *
 * If the given value is less than the minimum value of the interval, the minimum color of the
 * colormap will be returned. If the given value is higher than the maximum value of the interval,
 * the maximum color in the colormap will be returned. If min and max are equal, the Grey color will
 * be returned.
 *
 * This function works also if the values of the interval are swapped.
 *
 * @param[in] min: minimum value of the interval
 * @param[in] max: maximum value of the interval
 * @param[in] value: the value in the interval to ramp.
 * @return A color in the grey shade colormap representing the position of value in the interval
 * [min, max].
 */
inline Color colorFromIntervalGreyShade(float min, float max, float value)
{
	Color c;
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromIntervalGreyShade(value);
}

/**
 * @brief Given a number `n`, returns a vector of `n` colors so that each color differs as much as
 * possible from the previous one of the vector.
 *
 * @param n: the number of colors in the returned vector
 * @param sat: the saturation in the returned colors (default 0.3)
 * @param val: the value in the returned colors (default 0.9)
 * @return a vector of different colors.
 */
inline std::vector<Color> colorScattering(uint n, float sat, float val)
{
	std::vector<Color> scattering;
	scattering.reserve(n);

	for (uint v = 0; v < n; ++v) {
		uint value = v, m = n;
		uint b = 0;

		for (uint k = 1; k < n; k <<= 1) {
			if (value << 1 >= m) {
				b += k;
				value -= (m + 1) >> 1;
				m >>= 1;
			}
			else {
				m = (m + 1) >> 1;
			}
		}

		Color rc;
		rc.setHsvF(float(b) / float(n), sat, val);
		scattering.push_back(rc);
	}
	return scattering;
}

} // namespace vcl

#endif // VCL_SPACE_COLOR_H
