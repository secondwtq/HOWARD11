//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-06
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "JSFunctions.hxx"

#include "thirdpt/mozjs.hxx"

#include <stdio.h>
#include <string>

std::string readfile(const std::string& filename) {
    FILE *fp = fopen(filename.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
    std::unique_ptr<char> ret(new char[fsize + 1]);
    fread(ret.get(), static_cast<size_t>(fsize), 1, fp);
    ret.get()[fsize] = '\0';
    std::string str_ret(ret.get(), fsize);
    fclose(fp);
    return str_ret;
}

bool js_print(JSContext *context, unsigned int argc, JS::Value *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    for (size_t i = 0; i < args.length(); i++) {
        const char *t = JS_EncodeString(context, JS::ToString(context, args[i]));
        printf("%s", t);
        JS_free(context, const_cast<char *>(t));
    }
    printf("\n");
    args.rval().setUndefined();

    return true;
}
