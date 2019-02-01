// -*- C++ -*-
/*!
 * @file  Allocator.h
 * @brief Memory allocator class
 * @date  $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: PublisherPeriodic.h 1225 2009-02-28 02:30:25Z n-ando $
 *
 */

#ifndef COIL_ALLOCATOR_H
#define COIL_ALLOCATOR_H

#include <coil/Singleton.h>

#include <stddef.h>
#include <new>

namespace coil
{
  /*!
   * @if jp
   *
   * @class Allocator
   * @brief Allocator クラス
   *
   * @else
   *
   * @class Allocator
   * @brief Allocator class
   *
   * @endif
   */
  class Allocator
    : public Singleton<Allocator>
  {
  public:
    /*!
     * @if jp
     *
     * @brief デストラクタ
     *
     * デストラクタ。
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~Allocator() {}

    /*!
     * @if jp
     *
     * @brief メモリ領域確保
     *
     * メモリ領域を確保する。
     *
     * @param t 割り当てサイズ
     *
     * @return メモリ領域へのポインタ
     *
     * @else
     *
     * @brief Create of memory allocation
     *
     * Create of memory allocation.
     *
     * @param t Allocation size.
     *
     * @return Pointer to a memory allocation.
     *
     * @endif
     */
#if __cplusplus < 201703L 
    virtual void* New(size_t t) throw (std::bad_alloc);
#else
    virtual void* New(size_t t);
#endif

    /*!
     * @if jp
     *
     * @brief メモリ領域解放
     *
     * メモリ領域を解放する。
     *
     * @param p メモリ領域へのポインタ
     *
     * @else
     *
     * @brief Delete of memory allocation
     *
     * Delete of memory allocation.
     *
     * @param p Pointer to a memory allocation.
     *
     * @endif
     */
#if __cplusplus < 201703L
    virtual void Delete(void* p) throw ();
#else
    virtual void Delete(void* p) noexcept;
#endif

    /*!
     * @if jp
     *
     * @brief 配列用メモリ領域確保
     *
     * 配列用メモリ領域を確保する。
     *
     * @param t 割り当てサイズ
     *
     * @return メモリ領域へのポインタ
     *
     * @else
     *
     * @brief Create of array memory allocation
     *
     * Create of array memory allocation.
     *
     * @param t Allocation size.
     *
     * @return Pointer to a memory allocation.
     *
     * @endif
     */
#if __cplusplus < 201703L
    virtual void* NewArray(size_t t) throw (std::bad_alloc);
#else
    virtual void* NewArray(size_t t);
#endif

    /*!
     * @if jp
     *
     * @brief 配列用メモリ領域解放
     *
     * 配列用メモリ領域を解放する。
     *
     * @param p メモリ領域へのポインタ
     *
     * @else
     *
     * @brief Delete of array memory allocation
     *
     * Delete of array memory allocation.
     *
     * @param p Pointer to a memory allocation.
     *
     * @endif
     */
#if __cplusplus < 201703L
    virtual void DeleteArray(void* p) throw ();
#else
    virtual void DeleteArray(void* p) noexcept;
#endif

  };
};  // namespace coil
#endif  // COIL_ALLOCATOR_H
