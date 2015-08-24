
declare function print(... args: any[]) : void;
declare function collectgarbage(): void;
declare function cast<From, To>(org: From, proto: { new (... _args): To }): To;
