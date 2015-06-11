//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-06-06
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_HASSERT_HXX
#define HOWARD11_HASSERT_HXX

#define HOWARD_DEBUG

#include <stdio.h>
#include <stdlib.h>

inline bool hassert_internal(const char *file, int line, const char *exp, const char *msg) {
    static const char *default_msg = "No extra information provided.";

    char buf[1024];
    if (msg == NULL) {
        msg = default_msg; }
    sprintf(buf, "[FATAL] ASSERTION FAILED - %s:%d: %s - %s\n", file, line, exp, msg);
    printf("%s\n", buf);

    abort();
    return true; // we should never reach here
}

#if (!defined(HOWARD_DEBUG))

#define ASSERT(e) ((void) 0)
#define ASSERT_FOUNDATION ((void) 0)

#define ASSERT_MSG(e, msg) ((void) 0)

#else

#define ASSERT(e) ((void) ((!!(e)) || hassert_internal(__FILE__, __LINE__, #e, NULL)))
#define ASSERT_FOUNDATION_MSG(msg) ((void) (hassert_internal(__FILE__, __LINE__, (""), (msg))))
#define ASSERT_FOUNDATION() ((void) (ASSERT_FOUNDATION_MSG(NULL)))

#define ASSERT_MSG(e, msg) ((void) ((!!(e)) || hassert_internal(__FILE__, __LINE__, #e, (msg))))

#endif

#endif // HOWARD11_HASSERT_HXX
