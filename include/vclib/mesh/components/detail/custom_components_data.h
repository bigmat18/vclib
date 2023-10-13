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

#ifndef VCL_MESH_COMPONENTS_INTERNAL_CUSTOM_COMPONENTS_DATA_H
#define VCL_MESH_COMPONENTS_INTERNAL_CUSTOM_COMPONENTS_DATA_H

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <vclib/types.h>

namespace vcl::comp::detail {

/*
 * The CustomComponentData is the data structure that manages the access to the
 * custom components from an Element. If the custom components are horizontal,
 * they need to be stored in the Element memory frame, if they are vertical they
 * need to be stored by the Container of elements.
 *
 * The CustomComponentData stores the data horizontally
 * The CustomComponentData<El, true> (specialized on the boolean template)
 * stores the data vertically.
 */

// horizontal, not specialized. Store the custom component in this struct
template<typename ElementType, bool VERTICAL>
struct CustomComponentsData
{
    bool componentExists(const std::string& compName, const ElementType*) const
    {
        return (map.find(compName) != map.end());
    }

    template<typename CompType>
    bool isComponentOfType(const std::string& compName, const ElementType*)
        const
    {
        std::type_index t(typeid(CompType));
        return t == compType.at(compName);
    }

    std::type_index componentType(
        const std::string& compName,
        const ElementType*) const
    {
        return compType.at(compName);
    }

    template<typename CompType>
    std::vector<std::string> componentNamesOfType(const ElementType*) const
    {
        std::vector<std::string> names;
        std::type_index          t(typeid(CompType));
        for (const auto& p : compType) {
            if (p.second == t)
                names.push_back(p.first);
        }
        return names;
    }

    template<typename CompType>
    const CompType& get(const std::string& compName, const ElementType*) const
    {
        return std::any_cast<const CompType&>(map.at(compName));
    }

    template<typename CompType>
    CompType& get(const std::string& compName, ElementType*)
    {
        return std::any_cast<CompType&>(map.at(compName));
    }

    template<typename CompType>
    void addCustomComponent(
        const std::string& compName,
        const CompType     c = CompType())
    {
        map[compName] = c;
        compType.emplace(compName, typeid(CompType));
    }

    void deleteCustomComponent(const std::string& compName)
    {
        map.erase(compName);
        compType.erase(compName);
    }

private:
    std::unordered_map<std::string, std::any>        map;
    std::unordered_map<std::string, std::type_index> compType;
};

// vertical, specialized on the boolean.
// Access to the data stored in the Container of elements, trough the
// member function ccVec(elem), where elem is the pointer to the Element
// that is asking for the data
template<typename ElementType>
struct CustomComponentsData<ElementType, true>
{
    bool componentExists(const std::string& compName, const ElementType* elem)
        const
    {
        return ccVec(elem).componentExists(compName);
    }

    template<typename CompType>
    bool isComponentOfType(const std::string& compName, const ElementType* elem)
        const
    {
        return ccVec(elem).template isComponentOfType<CompType>(compName);
    }

    std::type_index componentType(
        const std::string& compName,
        const ElementType* elem) const
    {
        return ccVec(elem).componentType(compName);
    }

    template<typename CompType>
    std::vector<std::string> componentNamesOfType(const ElementType* elem) const
    {
        return ccVec(elem).template allComponentNamesOfType<CompType>();
    }

    template<typename CompType>
    const CompType& get(const std::string& compName, const ElementType* elem)
        const
    {
        return std::any_cast<const CompType&>(
            ccVec(elem).template componentVector<CompType>(
                compName)[thisId(elem)]);
    }

    template<typename CompType>
    CompType& get(const std::string& compName, ElementType* elem)
    {
        return std::any_cast<CompType&>(
            ccVec(elem).template componentVector<CompType>(
                compName)[thisId(elem)]);
    }

private:
    uint thisId(const ElementType* elem) const
    {
        assert(elem->parentMesh());
        return elem->index();
    }

    auto& ccVec(ElementType* elem)
    {
        assert(elem->parentMesh());
        // get the vector of custom components
        return elem->parentMesh()->template customComponents<ElementType>();
    }

    const auto& ccVec(const ElementType* elem) const
    {
        assert(elem->parentMesh());
        // get the vector of custom components
        return elem->parentMesh()->template customComponents<ElementType>();
    }
};

} // namespace vcl::comp::detail

#endif // VCL_MESH_COMPONENTS_INTERNAL_CUSTOM_COMPONENTS_DATA_H
