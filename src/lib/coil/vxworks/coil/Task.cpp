// -*- C++ -*-
/*!
 * @file Task_vxworks.cpp
 * @brief Task class
 * @date $Date$
 * @author Nobuhiko Miyamoto <n-miyamoto@aist.go.jp>
 *
 * Copyright (C) 2017
 *     Nobuhiko Miyamoto
 *     Robot Innovation Research Center
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#include <coil/Task.h>

#ifdef __RTP__
#define DEFAULT_PRIORITY 110
#define DEFAULT_STACKSIZE 60000
#endif

namespace coil
{

  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  Task::Task()
    : m_count(0)
#ifdef __RTP__
    ,m_priority(DEFAULT_PRIORITY)
    ,m_stacksize(DEFAULT_STACKSIZE)
#endif
  {
#ifdef __RTP__
#else
    ::pthread_attr_init(&m_attr);
#endif
  }

  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  Task::~Task()
  {
    m_count = 0;
  }

  /*!
   * @if jp
   * @brief �����������ץ�
   * @else
   * @brief Task open
   * @endif
   */
  int Task::open(void* args)
  {
    return 0;
  }

  /*!
   * @if jp
   * @brief ��������������
   * @else
   * @brief Task close
   * @endif
   */
  int Task::close(unsigned long flags)
  {
    return 0;
  }

  /*!
   * @if jp
   * @brief ����åɤ�¹Ԥ���
   * @else
   * @brief Execute thread
   * @endif
   */
  int Task::svc()
  {
    return 0;
  }

  /*!
   * @if jp
   * @brief ����åɤ���������
   * @else
   * @brief Create a thread
   * @endif
   */
  void Task::activate()
  {
    if (m_count == 0)
      {
#ifdef __RTP__
        m_tid = taskSpawn(
                         0,
                         m_priority,
                         VX_FP_TASK | VX_NO_STACK_FILL,
                         m_stacksize,
                         (FUNCPTR)Task::svc_run,
                         (int)this,
                         0,0,0,0,0,0,0,0,0
                         );
#else
        ::pthread_create(&m_thread,
                         &m_attr,
                         (void* (*)(void*))Task::svc_run,
                         this);
#endif
        ++m_count;
      };
  }

  /*!
   * @if jp
   * @brief ����åɽ�λ���Ԥ�
   * @else
   * @brief Waiting for the thread terminate
   * @endif
   */
  int Task::wait(void)
  {
    if (m_count > 0)
      {
#ifdef __RTP__
        taskExit(m_tid);
#else
        void* retval;
        ::pthread_join(m_thread, &retval);
#endif
      }
    return 0;
  }

  /*!
   * @if jp
   * @brief �������¹Ԥ����Ǥ���
   * @else
   * @brief Suspending the task
   * @endif
   */
  int Task::suspend(void)
  {
    return 0;
  }

  /*!
   * @if jp
   * @brief ���Ǥ���Ƥ��륿������Ƴ�����
   * @else
   * @brief Resuming the suspended task
   * @endif
   */
  int Task::resume(void)
  {
    return 0;
  }

  /*!
   * @if jp
   * @brief ���������ꥻ�å�
   * @else
   * @brief Reset of task count
   * @endif
   */
  void Task::reset()
  {
    m_count = 0;
  }

  /*!
   * @if jp
   * @brief �������¹Ԥ�λ����
   * @else
   * @brief Finalizing the task
   * @endif
   */
  void Task::finalize()
  {
    reset();
  }

  /*!
   * @if jp
   * @brief ����åɼ¹Ԥ򳫻Ϥ���
   * @else
   * @brief Start thread Execution
   * @endif
   */
#ifdef __RTP__
  extern "C" void* Task::svc_run(void* args)
#else
  void* Task::svc_run(void* args)
#endif
  {
    Task* t = (coil::Task*)args;
    int status;
    status = t->svc();
    t->finalize();
    return 0;
  }


#ifdef __RTP__
  /*!
   * @if jp
   *
   * @brief ��������ͥ���٤�����
   *
   *
   * @param priority ͥ����
   *
   * @else
   *
   * @brief 
   *
   *
   * @param priority 
   *
   * @endif
   */
  void Task::set_priority(int priority)
  {
    m_priority = priority;
  }
  /*!
   * @if jp
   *
   * @brief �����å�������������
   *
   *
   * @param stacksize �����å�������
   *
   * @else
   *
   * @brief 
   *
   *
   * @param stacksize 
   *
   * @endif
   */
  void Task::set_stacksize(int stacksize)
  {
    m_stacksize = stacksize;
  }
#endif
};

