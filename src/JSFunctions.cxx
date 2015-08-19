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

using namespace xoundation;

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

void js_collectgarbage(xoundation::spd::context_reference ctx) {
    JS_GC(JS_GetRuntime(ctx));
}

bool js_cast(JSContext *ctx, unsigned int argc, JS::Value *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    JS::RootedObject tocast(ctx, args[0].toObjectOrNull());
    spd::lifetime_base *lt = reinterpret_cast<spd::lifetime_base *>(JS_GetPrivate(tocast));
    if (!lt || lt->is_intrusive) {
        args.rval().setObject(*tocast.get());
        return true;
    }

    JS::RootedObject org(ctx, args[1].toObjectOrNull());
    JS::RootedObject proto(ctx, JS_GetReservedSlot(org, 1).toObjectOrNull());

    const JSClass *klass = JS_GetClass(tocast);
    JSObject *o = JS_NewObjectWithGivenProto(
            ctx, klass, proto, JS::NullPtr());
    JS_SetPrivate(o, lt->raw_copy());
    args.rval().setObject(*o);
    return true;
}
