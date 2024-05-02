/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_VERTEX_INDICES_H
#define VCL_MESH_COMPONENTS_VERTEX_INDICES_H

#include <vclib/concepts/mesh/components/vertex_pointers.h>
#include <vclib/iterators/mesh/components/pointer_from_index_iterator.h>
#include <vclib/views/view.h>

#include "bases/indices_container_component.h"

namespace vcl::comp {

/**
 * @brief The VertexIndices class represents a component that stores a
 * container of pointers to vertices that will be part of an Element (e.g.
 * Face, Edge, Tetrahedron, etc.).
 *
 * The container of Vertex pointers is a random access container that could have
 * static or dynamic size, depending on the value of the template argument N
 * (a negative number means dynamic).
 *
 * For example, if a Face element should represent only triangles, N could be
 * 3. In this case, the container will have static size and it can't be resized
 * in any way. If N is -1, the container will have dynamic size and it will be
 * possible to resize it (e.g. if the Face element should represent a polygon).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the VertexIndices
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * uint n = f.vertexNumber();
 * const auto* v = f.vertex(0);
 * uint vi = f.vertexIndex(0);
 * @endcode
 *
 * @note This component is usually the main component of an Element, and
 * therefore it cannot be optional.
 *
 * @note Several components are *Tied To Vertex Number*: in other words, they
 * are composed by a container that has the same size of the VertexIndices
 * and, when the VertexIndices container is resized, also the container of
 * these components is resized automatically.
 *
 * @tparam Vertex The type of the vertices.
 * @tparam N: The size of the container, that will represent the number of
 * storable vertex pointers. If negative, the container is dynamic.
 * @tparam ElementType: This type is used to get access to the Element that has
 * the component (and, in case, to the Mesh that has the Element). If the
 * component doesn't need to access the Element, this type can be void. Note:
 * if the component is vertical (or optional), this type cannot be void.
 * @tparam VERT: If true, the component will be stored vertically. This argument
 * is considered only if the ElementType is not void.
 *
 * @ingroup components
 */
template<typename Vertex, int N, typename ElementType = void, bool VERT = false>
class VertexIndices :
        public IndicesContainerComponent<
            VertexIndices<Vertex, N, ElementType, VERT>,
            CompId::VERTEX_INDICES,
            Vertex,
            N,
            ElementType,
            VERT,
            false,
            false>
{
    using Base = IndicesContainerComponent<
        VertexIndices<Vertex, N, ElementType, VERT>,
        CompId::VERTEX_INDICES,
        Vertex,
        N,
        ElementType,
        VERT,
        false,
        false>;

public:
    /**
     * @brief Expose the type of the Vertex.
     */
    using VertexType = Vertex;

    static const bool STORES_VERTEX_POINTERS = false;

    static const int VERTEX_NUMBER = Base::SIZE;

    /* Iterator Types declaration */

    using ConstVertexIndexIterator = Base::ConstIterator;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the Vertex Pointers container size is static, initializes all the
     * Vertex Pointers to `nullptr`, otherwise the container will be empty.
     */
    VertexIndices() = default;

    /* Member functions */

    /**
     * @brief Returns the number of vertices of the element.
     * @return The number of vertices of the element.
     */
    uint vertexNumber() const { return Base::container().size(); }

    /**
     * @brief Returns the pointer to the i-th vertex of the element.
     *
     * @param[in] i: the position of the required vertex in this container.
     * @return The pointer i-th vertex of the element.
     */
    Vertex* vertex(uint i)
    {
        &Base::parentElement()->parentMesh()->vertex(Base::container().at(i));
    }

    /**
     * @brief Returns a const pointer to the i-th vertex of the element.
     * @param[in] i: the position of the required vertex in this container;
     * the value must be between 0 and the number of vertices.
     * @return The pointer to the i-th vertex of the element.
     */
    const Vertex* vertex(uint i) const
    {
        &Base::parentElement()->parentMesh()->vertex(Base::container().at(i));
    }

    /**
     * @brief Returns the index in the vertex container of the i-th vertex of
     * the element.
     * @param[in] i: the position of the required vertex in this container.
     * @return The index of the i-th vertex of the element.
     */
    uint vertexIndex(uint i) const
    {
        return Base::container().at(i);
    }

    /**
     * @brief Returns a reference of the pointer to the i-th vertex of the
     * element, but using as index the module between i and the number of
     * vertices. You can use this function if you need to get the "next vertex
     * after position k", without check if it is less than the number of
     * vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * auto* next = e.vertexMod(k+1); // the vertex next to k, that may also
     *                                // be at pos 0
     * auto* last = e.vertexMod(-1); // the vertex in position vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The pointer to the required vertex of the element.
     */
    Vertex* vertexMod(int i)
    {
        return &Base::parentElement()->parentMesh()->vertex(
            Base::container().atMod(i));
    }

    /**
     * @brief Same of vertexMod, but returns a const pointer to the vertex.
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The pointer to the required vertex of the element.
     */
    const Vertex* vertexMod(int i) const {
        return &Base::parentElement()->parentMesh()->vertex(
            Base::container().atMod(i));
    }

    /**
     * @brief Returns the index in the vertex container of the i-th vertex of
     * the element, but using as index the module between i and the number of
     * vertices. You can use this function if you need to get the "index of the
     * vertex next to position k", without check if it is less than the number
     * of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * auto idx = e.vertexIndexMod(k+1); // the index of the vertex next to k,
     *                                   // that may also be at pos 0
     * auto lastIdx = e.vertexIndexMod(-1); // the index of the vertex in
     *                                      // position vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The index of the required vertex of the element.
     */
    uint vertexIndexMod(int i) const
    {
        return Base::container().atMod(i);
    }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] i: the position in this container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertex(uint i, Vertex* v)
    {
        if (v == nullptr) [[unlikely]]
            Base::container().set(i, UINT_NULL);
        else
            Base::container().set(i, v->index());
    }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] i: the position in this container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertex(uint i, uint vi)
    {
        Base::container().set(i, vi);
    }

    /**
     * @brief Sets the vertex pointed by the iterator.
     * @param[in] it: the position of the iterator in this container on which
     * set the vertex; the value must be between begin() and end().
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertex(ConstVertexIndexIterator it, Vertex* v)
    {
        if (v == nullptr) [[unlikely]]
            Base::container().set(it, UINT_NULL);
        else
            Base::container().set(it, v->index());
    }

    /**
     * @brief Sets the vertex pointed by the iterator.
     * @param[in] it: the position of the iterator in this container on which
     * set the vertex; the value must be between begin() and end().
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertex(ConstVertexIndexIterator it, uint vi)
    {
        Base::container().set(it, vi);
    }

    /**
     * @brief Sets the i-th vertex of the element, but using as index the module
     * between i and the number of vertices. You can use this function if you
     * need to set the "next vertex after position k", without check if it is
     * less than the number of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * e.setVertexMod(k+1, aVertex); // set the vertex next to k, that may also
     *                               // be at pos 0
     * e.setVertexMod(-1, aVertex); // set the vertex in position
     *                              // vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the vertex; value is modularized on vertexNumber().
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertexMod(int i, Vertex* v)
    {
        uint vi = UINT_NULL;
        if (v != nullptr) [[likely]]
            vi = v->index();
        Base::container().atMod(i) = vi;
    }

    /**
     * @brief Sets the i-th vertex of the element, but using as index the module
     * between i and the number of vertices. You can use this function if you
     * need to set the "next vertex after position k", without check if it is
     * less than the number of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * e.setVertexMod(k+1, aVertInd); // set the vertex next to k, that may also
     *                               // be at pos 0
     * e.setVertexMod(-1, aVertInd); // set the vertex in position
     *                              // vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the vertex; value is modularized on vertexNumber().
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertexMod(int i, uint vi)
    {
        Base::container().atMod(i) = vi;
    }

    /**
     * @brief Sets all the vertex pointers of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of vertex pointers to set. The value
     * type of the range must be convertible to a pointer to a Vertex.
     *
     * @param[in] r: range of vertex pointers to set.
     */
    template<Range Rng>
    void setVertices(Rng&& r) requires RangeOfConvertibleTo<Rng, Vertex*>
    {
        auto conv = [&](auto v) {
            if (v == nullptr) [[unlikely]]
                return UINT_NULL;
            else
                return v->index();
        };

        Base::container().set(r | std::views::transform(conv));
    }

    /**
     * @brief Sets all the vertex pointers of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of vertex indices to set. The value
     * type of the range must be convertible to unsigned integer.
     *
     * @param[in] r: range of vertex indices to set.
     */
    template<Range Rng>
    void setVertices(Rng&& r) requires RangeOfConvertibleTo<Rng, uint>
    {
        Base::container().set(r);
    }

    /**
     * @brief Returns `true` if the container of vertices contains the given
     * vertex, `false` otherwise.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return `true` if the container of vertices contains the given vertex,
     * `false` otherwise.
     */
    bool containsVertex(const Vertex* v) const
    {
        if (v == nullptr) [[unlikely]]
            return Base::container().contains(UINT_NULL);
        else
            return Base::container().contains(v->index());
    }

    /**
     * @brief Returns `true` if the container of vertices contains the vertex
     * with the given index, `false` otherwise.
     *
     * @param[in] vi: the index of the vertex to search.
     * @return `true` if the container of vertices contains the vertex with the
     * given index, `false` otherwise.
     */
    bool containsVertex(uint vi) const
    {
        return Base::container().contains(vi);
    }

    /**
     * @brief Returns the index of the given vertex in the container of
     * the element. If the given vertex is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return the index of the given vertex, or UINT_NULL if it is not
     * found.
     */
    uint indexOfVertex(const Vertex* v) const
    {
        if (v == nullptr) [[unlikely]]
            return Base::container().indexOf(UINT_NULL);
        else
            return Base::container().indexOf(v->index());
    }

    /**
     * @brief Returns the index of the vertex with the given index in the
     * container of the element. If the vertex with the given index is not in
     * the container, returns UINT_NULL.
     *
     * @param[in] vi: the index of the vertex to search.
     * @return the index of the vertex with the given index, or UINT_NULL if it
     * is not found.
     */
    uint indexOfVertex(uint vi) const
    {
        return Base::container().indexOf(vi);
    }

    /**
     * @brief Returns the index of the given edge composed of the two vertices
     * v1 and v2 in the container of the element.
     *
     * The order of the two vertices is not important. The index of the edge
     * corresponds to the index of the first vertex found in the container. If
     * the edge formed by the two vertices is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] v1: the pointer to the first vertex that compose the edge to
     * search.
     * @param[in] v2: the pointer to the second vertex that compose the edge to
     * search.
     * @return the index of the edge composed of v1 and v2, or UINT_NULL if it
     * is not found.
     */
    uint indexOfEdge(const Vertex* v1, const Vertex* v2) const
    {
        uint vi1 = v1 == nullptr ? UINT_NULL : v1->index();
        uint vi2 = v2 == nullptr ? UINT_NULL : v2->index();
        return indexOfEdge(vi1, vi2);
    }

    /**
     * @brief Returns the index of the edge composed of the two vertices with
     * the given indices in the container of the element.
     *
     * The order of the two vertices is not important. The index of the edge
     * corresponds to the index of the first vertex found in the container. If
     * the edge formed by the two vertices is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] vi1: the index of the first vertex that compose the edge to
     * search.
     * @param[in] vi2: the index of the second vertex that compose the edge to
     * search.
     * @return the index of the edge composed of the vertices with the given
     * indices, or UINT_NULL if it is not found.
     */
    uint indexOfEdge(uint vi1, uint vi2) const
    {
        uint vid = indexOfVertex(vi1);
        if (vid == UINT_NULL) {
            return UINT_NULL;
        }
        else if (vertexMod(vid + 1) == vi2) {
            return vid;
        }
        else if (vertexMod((int) vid - 1) == vi2) {
            int n = vertexNumber(); // n must be int to avoid unwanted casts
            return (((int) vid - 1) % n + n) % n;
        }
        else {
            return UINT_NULL;
        }
    }

    /* Member functions specific for vector of pointers */

    /**
     * @brief Resize the container of the vertices to the given size.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] n: The new size of the vertices container.
     */
    void resizeVertices(uint n) requires (N < 0)
    {
        Base::container().resize(n);
    }

    /**
     * @brief Pushes in the back of the container the given vertex.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] v: The pointer to the vertex to push in the back of the
     * container.
     */
    void pushVertex(Vertex* v) requires (N < 0)
    {
        if (v != nullptr) [[likely]]
            Base::container().pushBack(v->index());
        else
            Base::container().pushBack(UINT_NULL);
    }

    /**
     * @brief Pushes in the back of the container the given vertex.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] vi: The index to the vertex to push in the back of the
     * container.
     */
    void pushVertex(uint vi) requires (N < 0)
    {
        Base::container().pushBack(vi);
    }

    /**
     * @brief Inserts the given vertex in the container at the given position.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position in this container where to insert the vertex.
     * @param[in] v: The pointer to the vertex to insert in the container.
     */
    void insertVertex(uint i, Vertex* v) requires (N < 0)
    {
        if (v != nullptr) [[likely]]
            Base::container().insert(i, v->index());
        else
            Base::container().insert(i, UINT_NULL);
    }

    /**
     * @brief Inserts the vertex with the given index in the container at the
     * given position.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position in this container where to insert the vertex.
     * @param[in] vi: The index of the vertex to insert in the container.
     */
    void insertVertex(uint i, uint vi) requires (N < 0)
    {
        Base::container().insert(i, vi);
    }

    /**
     * @brief Removes the vertex at the given position from the container.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position of the vertex to remove from this container.
     */
    void eraseVertex(uint i) requires (N < 0) { Base::container().erase(i); }

    /**
     * @brief Clears the container of vertices, making it empty.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     */
    void clearVertices() requires (N < 0) { Base::container().clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns a const iterator to the first vertex in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstVertexIndexIterator vertexBegin() const
    {
        return Base::container().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstVertexIndexIterator vertexEnd() const
    {
        return Base::container().end();
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of vertices of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto* vertex : el.vertices()) {
     *     // Do something read-only with vertex...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over vertices.
     */
    View<ConstVertexIndexIterator> vertices() const
    {
        return View(vertexBegin(), vertexEnd());
    }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasVertexPointers<Element>) {
            if constexpr (N > 0) {
                // same size non-polygonal faces
                if constexpr (N == Element::VERTEX_NUMBER) {
                    importIndicesFrom(e);
                }
                // from polygonal to fixed size, but the polygon size == the
                // fixed face size
                else if constexpr (Element::VERTEX_NUMBER < 0) {
                    if (e.vertexNumber() == N) {
                        importIndicesFrom(e);
                    }
                }
                else {
                    // do not import in this case: cannot import from a face of
                    // different size
                }
            }
            else {
                // from fixed to polygonal size: need to resize first, then
                // import
                resizeVertices(e.vertexNumber());
                importIndicesFrom(e);
            }
        }
    }

    // PointersComponent interface functions
    template<typename Element, typename ElVType>
    void importPointersFrom(
        const Element&,
        Vertex*,
        const ElVType*)
    {
    }

private:
    template<typename Element>
    void importIndicesFrom(const Element& e)
    {
        for (uint i = 0; i < e.vertexNumber(); ++i) {
            setVertex(i, e.vertexIndex(i));
        }
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTEX_INDICES_H
