#pragma once

/*===================================//
//   Forward Declaration Shortcuts   //
//===================================*/
#define MGSL_CLASS_PTR(className)                                     \
class className;                                                      \
using className ## UPtr = MGSL::Shared::UniquePtr<className>;         \
using className ## Ptr  = MGSL::Shared::SharedPtr<className>;         \
using className ## WPtr = MGSL::Shared::WeakPtr<className>;

/*===========================//
//     Class Constraints     //
//===========================*/
#define MGSL_DISABLE_COPY(classType)                                  \
    classType(const classType&) = delete;                             \
    classType& operator=(const classType&) = delete;

#define MGSL_DISABLE_MOVE(classType)                                  \
    classType(classType&&) = delete;                                  \
    classType& operator=(classType&&) = delete;

/*===============================//
//     Singleton Declaration     //
//===============================*/
#define MGSL_DECLARE_SINGLE(classType)                                \
private:                                                              \
    MGSL_DISABLE_COPY(classType)                                      \
    MGSL_DISABLE_MOVE(classType)                                      \
                                                                      \
public:                                                               \
    static classType& Instance()                                      \
    {                                                                 \
        static classType* s_instance = new classType();               \
        return *s_instance;                                           \
    }

