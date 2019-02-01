// -*- C++ -*-
/*!
 * @file  Allocator.cpp
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

#include <coil/Allocator.h>

namespace coil
{

  /*!
   * @if jp
   * @brief メモリ領域確保
   * @else
   * @brief Create of memory allocation
   * @endif
   */
#if __cplusplus < 201703L
  void* Allocator::New(size_t t) throw (std::bad_alloc)
#else
  void* Allocator::New(size_t t)
#endif
  {
    return operator new(t);
  }

  /*!
   * @if jp
   * @brief メモリ領域解放
   * @else
   * @brief Delete of memory allocation
   * @endif
   */
#if __cplusplus < 201703L
  void Allocator::Delete(void* p) throw ()
#else
  void Allocator::Delete(void* p) noexcept
#endif
  {
    operator delete(p);
  }

  /*!
   * @if jp
   * @brief 配列用メモリ領域確保
   * @else
   * @brief Create of array memory allocation
   * @endif
   */
#if __cplusplus < 201703L
  void* Allocator::NewArray(size_t t) throw (std::bad_alloc)
#else
  void* Allocator::NewArray(size_t t)
#endif
  {
    return operator new[](t);
  }

  /*!
   * @if jp
   * @brief 配列用メモリ領域解放
   * @else
   * @brief Delete of array memory allocation
   * @endif
   */
#if __cplusplus < 201703L
  void Allocator::DeleteArray(void* p) throw ()
#else
  void Allocator::DeleteArray(void* p) noexcept
#endif
  {
    operator delete[](p);
  }
};  // namespace coil

