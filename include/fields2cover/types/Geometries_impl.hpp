//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                           MIT License
//=============================================================================

#pragma once
#ifndef FIELDS2COVER_TYPES_GEOMETRIES_IMPL_HPP_
#define FIELDS2COVER_TYPES_GEOMETRIES_IMPL_HPP_

namespace f2c::types {

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
double Geometries<SAMETYPE, T, R, CHILDRENTYPE>::getArea() const {
  return (this->data?this->data->get_Area():0.0);
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
SAMETYPE Geometries<SAMETYPE, T, R, CHILDRENTYPE>::clone() const {
  SAMETYPE cloned;
  cloned.importFromWkt(this->exportToWkt());
  return cloned;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::Private::Private()
  = default;

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::Private::~Private() {}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
void Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::update() {
  if (std::is_same<T, OGRMultiPoint>::value ||
    std::is_same<T, OGRLinearRing>::value ||
    std::is_same<T, OGRLineString>::value) {
    if (!m_poPrivate->m_bUpdateChecked) {
      CHILDRENTYPE oChildBefore;
      static_cast<SAMETYPE*>(m_poPrivate->m_poSelf)->getGeometry(
        m_poPrivate->m_nPos, oChildBefore);
      if (oChildBefore != m_poPrivate->m_oChild) {
          static_cast<SAMETYPE*>(m_poPrivate->m_poSelf)->setGeometry(
             m_poPrivate->m_nPos, m_poPrivate->m_oChild);
      }
      m_poPrivate->m_bUpdateChecked = true;
    }
  }
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::Iterator(
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>* poSelf, int nPos) :
      m_poPrivate(new Private()) {
  m_poPrivate->m_poSelf = poSelf;
  m_poPrivate->m_nPos = nPos;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::~Iterator() {
  update();
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
CHILDRENTYPE& Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::operator*() {
  update();
  static_cast<SAMETYPE*>(m_poPrivate->m_poSelf)->getGeometry(
      m_poPrivate->m_nPos, m_poPrivate->m_oChild);
  m_poPrivate->m_bUpdateChecked = false;
  return m_poPrivate->m_oChild;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator&
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::operator++() {
  update();
  ++m_poPrivate->m_nPos;
  return *this;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
bool Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator::operator!=(
    const Iterator& it) const {
  return m_poPrivate->m_nPos != it.m_poPrivate->m_nPos;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::Private::Private()
  = default;


template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::ConstIterator(
    ConstIterator& oOther) = default;

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::ConstIterator(
    const Geometries<SAMETYPE, T, R, CHILDRENTYPE>* poSelf,
        int nPos) : m_poPrivate(new Private()) {
  m_poPrivate->m_poSelf = poSelf;
  m_poPrivate->m_nPos = nPos;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::~ConstIterator()
  = default;

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
const CHILDRENTYPE& Geometries<SAMETYPE, T, R, CHILDRENTYPE>
        ::ConstIterator::operator*() const {
  static_cast<const SAMETYPE*>(
    m_poPrivate->m_poSelf)->getGeometry(m_poPrivate->m_nPos,
      m_poPrivate->m_oChild);
  return m_poPrivate->m_oChild;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator&
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::operator++() {
  ++m_poPrivate->m_nPos;
  return *this;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
bool Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::operator!=(
    const ConstIterator& it) const {
  return m_poPrivate->m_nPos != it.m_poPrivate->m_nPos;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
bool Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator::operator==(
    const Iterator& it) const {
  return m_poPrivate->m_nPos == it.m_poPrivate->m_nPos;
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::begin() {
  return {this, 0};
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::Iterator
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::end() { return {this,
  static_cast<int>(static_cast<SAMETYPE*>(this)->size())};
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::begin() const {
  return {this, 0};
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::end() const {
  return {this, static_cast<int>(static_cast<SAMETYPE const*>(this)->size())};
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::begin(const SAMETYPE* poSelf) {
  return poSelf->begin();
}

template <class SAMETYPE, class T, OGRwkbGeometryType R, class CHILDRENTYPE>
typename Geometries<SAMETYPE, T, R, CHILDRENTYPE>::ConstIterator
    Geometries<SAMETYPE, T, R, CHILDRENTYPE>::end(const SAMETYPE* poSelf) {
  return poSelf->end();
}

}  // namespace f2c::types

#endif  // FIELDS2COVER_TYPES_GEOMETRIES_IMPL_HPP_
