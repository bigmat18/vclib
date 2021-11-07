/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_BIT_FLAGS_H
#define MGP_MESH_PROPERTIES_BIT_FLAGS_H

#include <assert.h>
#include <type_traits>

namespace mgp::prop {

/**
 * @brief The BitFlags property class represents a collection of 32 bits that will be part of an
 * Element (e.g. Vertex, Face, ...).
 *
 * This Property (or a specialization) is mandatory into every Element of the mesh.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Element has been deleted
 * - 1: selected: if the current Element has been selected
 * - 2: border: if the current Element is on border
 * - other: user bits that can have custom meanings to the user
 *
 * The access to the user bits is generally allowed by the userBit member functions, that accept
 * a position as parameter that is a number between 0 and the total number of resereved user bits,
 * which in this class is 29, but may be less if using a derived of this class.
 */
class BitFlags
{
public:
	bool isDeleted() const;
	bool isSelected() const;
	bool isOnBorder() const;
	bool userBitFlag(unsigned int bit) const;

	void setSelected();
	void setOnBorder();
	void setUserBit(unsigned int bit);

	void clearAllFlags();
	void clearSelected();
	void clearOnBorder();
	void clearUserBit(unsigned int bit);

protected:
	void setDeleted();
	void clearDeleted();

	bool flagValue(unsigned int flag) const;
	void setFlag(unsigned int flag);
	void clearFlag(unsigned int flag);

	bool userBitFlag(unsigned int bit, unsigned int firstBit) const;
	void setUserBit(unsigned int bit, unsigned int firstBit);
	void clearUserBit(unsigned int bit, unsigned int firstBit);

	int flags = 0;

	static const unsigned int FIRST_USER_BIT = 3;

	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		DELETED  = 1 << 0, // bit 0
		SELECTED = 1 << 1, // bit 1
		BORDER   = 1 << 2  // bit 2
	};
};

/**
 * Detector to check if a class has (inherits) BitFlags
 */

template<typename T>
using hasBitFlagsT = std::is_base_of<BitFlags, T>;

template<typename T>
bool constexpr hasBitFlags()
{
	return hasBitFlagsT<T>::value;
}

} // namespace mgp::prop

#include "bit_flags.cpp"

#endif // MGP_MESH_PROPERTIES_BIT_FLAGS_H