namespace QP {

/// @file
/// command-line macros and macros for porting QP

/// The preprocessor switch to disable checking assertions
///
/// When defined, Q_NASSERT disables the following macros #Q_ASSERT,
/// #Q_REQUIRE, #Q_ENSURE, #Q_INVARIANT, #Q_ERROR as well as
/// #Q_ASSERT_ID, #Q_REQUIRE_ID, #Q_ENSURE_ID, #Q_INVARIANT_ID, and
/// #Q_ERROR_ID do NOT evaluate the test condition passed as the
/// argument to these macros.
///
/// @note One notable exception is the macro #Q_ALLEGE, that still
/// evaluates the test condition, but does not report assertion
/// failures when the switch Q_NASSERT is defined.
#define Q_NASSERT

/// The preprocessor switch to activate the event-constructors
/// and destructors
///
/// When Q_EVT_CTOR is defined (typically in the qep_port.hpp header file),
/// QP::QEvt becomes a class with constructor and virtual destructor.
/// More importantly, the subclasses of QEvt (your custom events) can have
/// non-default constructors and destructors. These constructors are then
/// called when events are created (e.g., with Q_NEW()) and the destrucor
/// is invoked before recycling the event with QP::QF::gc().
#define Q_EVT_CTOR

/// The preprocessor switch to activate the QS software tracing
/// instrumentation in the code
///
/// When defined, Q_SPY activates the QS software tracing instrumentation.
/// When Q_SPY is not defined, the QS instrumentation in the code does
/// not generate any code.
#define Q_SPY

/// The preprocessor switch to activate the QUTest unit testing
/// instrumentation in the code */
///
/// @note
/// This macro requires that #Q_SPY be defined as well.
#define Q_UTEST

/// This macro defines the type of the OS-Object used for blocking
/// the native QF event queue when the queue is empty
///
/// In QK, the OS object is used to hold the per-thread flags, which might
/// be used, for example, to remember the thread attributes (e.g.,
/// if the thread uses a floating point co-processor). The OS object value
/// is set on per-thread basis in QActive::start(). Later, the extended
/// context switch macros (QK_EXT_SAVE() and QK_EXT_RESTORE()) might use
/// the per-thread flags to determine what kind of extended context switch
/// this particular thread needs (e.g., the thread might not be using the
/// coprocessor or might be using a different one).
#define QF_OS_OBJECT_TYPE  uint8_t

/// This macro defines the type of the thread handle used for the
/// active objects. This macro depends on the QP port.
#define QF_THREAD_TYPE     void *

/// Platform-dependent macro defining how QF should block the calling
/// task when the QF native queue is empty
///
/// @note This is just an example of #QACTIVE_EQUEUE_WAIT_ for the QK-port
/// of QF. QK never activates a task that has no events to process, so in this
/// case the macro asserts that the queue is not empty. In other QF ports you
// need to define the macro appropriately for the underlying kernel/OS you're
/// using.
#define QACTIVE_EQUEUE_WAIT_(me_) \
    Q_ASSERT((me_)->m_eQueue.m_frontEvt != static_cast<QEvt const *>(0))

/// Platform-dependent macro defining how QF should signal the
/// active object task that an event has just arrived.
///
/// The macro is necessary only when the native QF event queue is used.
/// The signaling of task involves unblocking the task if it is blocked.
///
/// @note #QACTIVE_EQUEUE_SIGNAL_ is called from a critical section.
/// It might leave the critical section internally, but must restore
/// the critical section before exiting to the caller.
///
/// @note This is just an example of #QACTIVE_EQUEUE_SIGNAL_ for the QK-port
/// of QF. In other QF ports you need to define the macro appropriately for
/// the underlying kernel/OS you're using.
#define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
    QK_readySet_.insert((me_)->m_prio); \
    if (QK_intNest_ == static_cast<uint8_t>(0)) { \
        uint8_t p = QK_schedPrio_(); \
        if (p != static_cast<uint8_t>(0)) { \
            QK_sched_(p); \
        } \
    } \
} while (false)

/// This macro defines the type of the event pool used in this QF port.
///
/// \note This is a specific implementation for the QK-port of QF.
/// In other QF ports you need to define the macro appropriately for
/// the underlying kernel/OS you're using.
#define QF_EPOOL_TYPE_              QMPool

/// This macro enables calling the QK context-switch callback
/// QK_onContextSw()
#define QK_ON_CONTEXT_SW

/// This macro enables calling the QXK context-switch callback
/// QXK_onContextSw()
#define QXK_ON_CONTEXT_SW

/// Platform-dependent macro defining the event pool initialization
///
/// \note This is a specific implementation for the QK-port of QF.
/// In other QF ports you need to define the macro appropriately for
/// the underlying kernel/OS you're using.
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (p_).init((poolSto_), (poolSize_), static_cast<QMPoolSize>(evtSize_))

/// Platform-dependent macro defining how QF should obtain the
/// event pool block-size
///
/// \note This is a specific implementation for the QK-port of QF.
/// In other QF ports you need to define the macro appropriately for
/// the underlying kernel/OS you're using.
#define QF_EPOOL_EVENT_SIZE_(p_) static_cast<uint32_t>((p_).getBlockSize())

/// Platform-dependent macro defining how QF should obtain an event
/// \a e_ from the event pool \a p_
///
/// \note This is a specific implementation for the QK-port of QF.
/// In other QF ports you need to define the macro appropriately for
/// the underlying kernel/OS you're using.
#define QF_EPOOL_GET_(p_, e_, m_) \
    ((e_) = static_cast<QEvt *>((p_).get((m_))))

/// Platform-dependent macro defining how QF should return an event
/// \a e_ to the event pool \a p_
///
/// \note This is a specific implementation for the QK-port of QF.
/// In other QF ports you need to define the macro appropriately for
/// the underlying kernel/OS you're using.
#define QF_EPOOL_PUT_(p_, e_)   ((p_).put(e_))

/// Macro that should be defined (typically on the compiler's command line)
/// in the Win32-GUI applications that use the @ref win32 or @ref win32-qv
/// ports.
#define WIN32_GUI

} // namespace QP