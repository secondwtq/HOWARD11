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

#include <stdio.h>
#include <stdlib.h>

inline void hassert_internal(const char *file, int line, const char *exp, const char *msg) {
    char buf[1024];
    sprintf(buf, "");
    printf("%s\n", buf);

    abort();
}

#if (!defined(HOWARD_DEBUG))

#define ASSERT(e) ((void) 0)
#define ASSERT_FOUNDATION ((void) 0)

#define ASSERT_MSG()

#else

#define ASSERT(e)
#define ASSERT_FOUNDATION(e)

#define ASSERT_MSG()

#endif

#endif // HOWARD11_HASSERT_HXX
